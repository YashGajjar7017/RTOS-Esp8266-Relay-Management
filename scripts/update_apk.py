import os
import sys
import struct
import zlib
import base64
import hashlib
import zipfile
from cryptography.hazmat.primitives.serialization import pkcs12, pkcs7
from cryptography.hazmat.primitives import hashes, serialization
from cryptography.hazmat.primitives.asymmetric import padding
from cryptography import x509

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
KEYSTORE_PATH = os.path.join(ROOT, "android_app", "debug.keystore")
SRC_APK = "/tmp/orig.apk" if os.path.exists("/tmp/orig.apk") else os.path.join(ROOT, "CyberRelay-RTOS.apk")

OUT_APKS = [
    os.path.join(ROOT, "CyberRelay-RTOS.apk"),
    os.path.join(ROOT, "app-debug.apk"),
    os.path.join(ROOT, "android_app", "app", "build", "outputs", "apk", "debug", "app-debug.apk")
]

ASSETS = {
    'assets/app.js': os.path.join(ROOT, 'web_app', 'app.js'),
    'assets/index.html': os.path.join(ROOT, 'web_app', 'index.html'),
    'assets/style.css': os.path.join(ROOT, 'web_app', 'style.css')
}

def parse_zip_entries(apk_data):
    """Parses zip entries in order from Central Directory"""
    entries = []
    eocd_pos = apk_data.rfind(b'PK\x05\x06')
    if eocd_pos == -1:
        raise ValueError("Invalid ZIP/APK: no EOCD")
    
    cd_offset = struct.unpack('<I', apk_data[eocd_pos+16:eocd_pos+20])[0]
    cd_pos = cd_offset
    while cd_pos < eocd_pos:
        sig = apk_data[cd_pos:cd_pos+4]
        if sig != b'PK\x01\x02':
            break
        v_made, v_need, flags, comp_method, mod_time, mod_date, crc32, comp_size, uncomp_size, name_len, extra_len, comment_len, disk_num, int_attr, ext_attr, local_offset = struct.unpack('<HHHHHHIIIHHHHHII', apk_data[cd_pos+4:cd_pos+46])
        name = apk_data[cd_pos+46:cd_pos+46+name_len].decode('utf-8', errors='ignore')
        
        # Read local header to get uncompressed data
        lfh_sig = apk_data[local_offset:local_offset+4]
        assert lfh_sig == b'PK\x03\x04', f"Corrupt LFH at {local_offset} for {name}"
        lfh_name_len, lfh_extra_len = struct.unpack('<HH', apk_data[local_offset+26:local_offset+30])
        header_len = 30 + lfh_name_len + lfh_extra_len
        file_bytes = apk_data[local_offset+header_len : local_offset+header_len+comp_size]
        
        if comp_method == 0:
            decomp = file_bytes
        elif comp_method == 8:
            decomp = zlib.decompress(file_bytes, -15)
        else:
            decomp = file_bytes
            
        entries.append((name, decomp, comp_method))
        cd_pos += 46 + name_len + extra_len + comment_len
        
    return entries

def compute_chunked_root_hash(sec1, sec2, sec3):
    CHUNK = 1048576  # 1MB
    chunks = []
    for s in [sec1, sec2, sec3]:
        for i in range(0, len(s), CHUNK):
            chunks.append(s[i:i+CHUNK])
            
    # Each chunk digest has prefix 0xa5: SHA-256(0xa5 || uint32(len) || chunk)
    chunk_digests = [hashlib.sha256(b'\xa5' + struct.pack('<I', len(c)) + c).digest() for c in chunks]
    
    # Root digest has prefix 0x5a: SHA-256(0x5a || uint32(num_chunks) || chunk_digests)
    root_digest = hashlib.sha256(b'\x5a' + struct.pack('<I', len(chunks)) + b''.join(chunk_digests)).digest()
    return root_digest

def verify_apk(apk_bytes):
    """Verifies ZIP archive, v2 signature, and v3 signature"""
    magic_idx = apk_bytes.rfind(b'APK Sig Block 42')
    if magic_idx == -1:
        return False, "No APK Sig Block 42 found"
    
    size_end = struct.unpack('<Q', apk_bytes[magic_idx-8:magic_idx])[0]
    block_start = magic_idx + 16 - size_end - 8
    size_start = struct.unpack('<Q', apk_bytes[block_start:block_start+8])[0]
    if size_start != size_end:
        return False, f"APK Sig Block size mismatch: {size_start} vs {size_end}"

    sec1 = apk_bytes[:block_start]
    eocd_pos = apk_bytes.rfind(b'PK\x05\x06')
    sec2 = apk_bytes[block_start + 8 + size_end : eocd_pos]
    sec3 = bytearray(apk_bytes[eocd_pos:])
    struct.pack_into('<I', sec3, 16, block_start)

    root_digest = compute_chunked_root_hash(sec1, sec2, bytes(sec3))

    # Parse blocks
    pos = block_start + 8
    blocks = {}
    while pos < magic_idx - 8:
        pair_len = struct.unpack('<Q', apk_bytes[pos:pos+8])[0]
        pair_id = struct.unpack('<I', apk_bytes[pos+8:pos+12])[0]
        val = apk_bytes[pos+12:pos+8+pair_len]
        blocks[pair_id] = val
        pos += 8 + pair_len

    if 0x7109871a not in blocks:
        return False, "Missing v2 signature block"
    if 0xf05368c0 not in blocks:
        return False, "Missing v3 signature block"

    # Verify v2
    v2_val = blocks[0x7109871a]
    v2_signer = v2_val[8:8+struct.unpack('<I', v2_val[4:8])[0]]
    v2_sd_len = struct.unpack('<I', v2_signer[:4])[0]
    v2_sd = v2_signer[4:4+v2_sd_len]
    v2_offset = 4 + v2_sd_len
    v2_sigs_len = struct.unpack('<I', v2_signer[v2_offset:v2_offset+4])[0]
    v2_sigs = v2_signer[v2_offset+4:v2_offset+4+v2_sigs_len]
    v2_offset += 4 + v2_sigs_len
    v2_pk_len = struct.unpack('<I', v2_signer[v2_offset:v2_offset+4])[0]
    v2_pk_bytes = v2_signer[v2_offset+4:v2_offset+4+v2_pk_len]

    v2_sig_len = struct.unpack('<I', v2_sigs[8:12])[0]
    v2_sig_bytes = v2_sigs[12:12+v2_sig_len]
    v2_pubkey = serialization.load_der_public_key(v2_pk_bytes)
    v2_pubkey.verify(v2_sig_bytes, v2_sd, padding.PKCS1v15(), hashes.SHA256())

    # Verify v3
    v3_val = blocks[0xf05368c0]
    v3_signer = v3_val[8:8+struct.unpack('<I', v3_val[4:8])[0]]
    v3_sd_len = struct.unpack('<I', v3_signer[:4])[0]
    v3_sd = v3_signer[4:4+v3_sd_len]
    v3_offset = 4 + v3_sd_len
    v3_min_sdk, v3_max_sdk = struct.unpack('<II', v3_signer[v3_offset:v3_offset+8])
    v3_offset += 8
    v3_sigs_len = struct.unpack('<I', v3_signer[v3_offset:v3_offset+4])[0]
    v3_sigs = v3_signer[v3_offset+4:v3_offset+4+v3_sigs_len]
    v3_offset += 4 + v3_sigs_len
    v3_pk_len = struct.unpack('<I', v3_signer[v3_offset:v3_offset+4])[0]
    v3_pk_bytes = v3_signer[v3_offset+4:v3_offset+4+v3_pk_len]

    v3_sig_len = struct.unpack('<I', v3_sigs[8:12])[0]
    v3_sig_bytes = v3_sigs[12:12+v3_sig_len]
    v3_pubkey = serialization.load_der_public_key(v3_pk_bytes)
    v3_pubkey.verify(v3_sig_bytes, v3_sd, padding.PKCS1v15(), hashes.SHA256())

    return True, f"ALL SIGNATURES VALID! Root digest: {root_digest.hex()[:16]}... (v2 OK, v3 OK)"

def main():
    print("=" * 68)
    print("      CYBER-RELAY RTOS // ROBUST APK ASSET UPDATER & SIGNER")
    print("=" * 68)

    # 1. Load private key & cert
    print("[1/6] Loading Android debug key and certificates...")
    with open(KEYSTORE_PATH, 'rb') as f:
        key, cert, _ = pkcs12.load_key_and_certificates(f.read(), b'android')
    cert_der = cert.public_bytes(serialization.Encoding.DER)
    pk_der = key.public_key().public_bytes(serialization.Encoding.DER, serialization.PublicFormat.SubjectPublicKeyInfo)
    print(f"      Cert DER length: {len(cert_der)} bytes, Public Key length: {len(pk_der)} bytes")

    # 2. Read existing APK
    print(f"[2/6] Reading base APK from: {SRC_APK}")
    with open(SRC_APK, 'rb') as f:
        apk_data = f.read()

    entries = parse_zip_entries(apk_data)
    
    # Filter out old META-INF signatures
    filtered_entries = []
    for name, data, comp in entries:
        if name.startswith('META-INF/'):
            continue
        if name in ASSETS:
            print(f"      Injecting updated: {name} ({os.path.getsize(ASSETS[name]):,} bytes)")
            with open(ASSETS[name], 'rb') as af:
                data = af.read()
            comp = 8
        filtered_entries.append((name, data, comp))

    # 3. Generate v1 JAR Signature (MANIFEST.MF, ANDROIDD.SF, ANDROIDD.RSA)
    print("[3/6] Generating standard v1 JAR signature...")
    manifest_lines = [
        b"Manifest-Version: 1.0",
        b"Built-By: Generated-by-ADT",
        b"Created-By: CyberRelay-APK-Signer",
        b""
    ]
    
    entry_digests = {}
    for name, data, _ in filtered_entries:
        digest = base64.b64encode(hashlib.sha256(data).digest()).decode('ascii')
        entry_digests[name] = digest
        manifest_lines.append(f"Name: {name}".encode('utf-8'))
        manifest_lines.append(f"SHA-256-Digest: {digest}".encode('utf-8'))
        manifest_lines.append(b"")
    manifest_bytes = b"\r\n".join(manifest_lines) + b"\r\n"

    sf_lines = [
        b"Signature-Version: 1.0",
        b"Created-By: 1.0 (Android)",
        f"SHA-256-Digest-Manifest: {base64.b64encode(hashlib.sha256(manifest_bytes).digest()).decode('ascii')}".encode('utf-8'),
        b""
    ]
    for name, digest in entry_digests.items():
        entry_str = f"Name: {name}\r\nSHA-256-Digest: {digest}\r\n\r\n".encode('utf-8')
        entry_hash = base64.b64encode(hashlib.sha256(entry_str).digest()).decode('ascii')
        sf_lines.append(f"Name: {name}".encode('utf-8'))
        sf_lines.append(f"SHA-256-Digest: {entry_hash}".encode('utf-8'))
        sf_lines.append(b"")
    sf_bytes = b"\r\n".join(sf_lines) + b"\r\n"

    # Sign SF with PKCS#7
    builder = pkcs7.PKCS7SignatureBuilder().set_data(sf_bytes)
    builder = builder.add_signer(cert, key, hashes.SHA256())
    rsa_bytes = builder.sign(serialization.Encoding.DER, [pkcs7.PKCS7Options.DetachedSignature])

    # Append signature files
    filtered_entries.append(('META-INF/MANIFEST.MF', manifest_bytes, 8))
    filtered_entries.append(('META-INF/ANDROIDD.SF', sf_bytes, 8))
    filtered_entries.append(('META-INF/ANDROIDD.RSA', rsa_bytes, 8))

    # 4. Pack into aligned ZIP stream (sec1)
    print("[4/6] Packing entries into ZIP with Android 0xd935 4-byte alignment...")
    zip_bytes = bytearray()
    cd_entries = []
    
    for name, content, comp_method in filtered_entries:
        offset = len(zip_bytes)
        comp_type = 0 if (name == 'resources.arsc') else 8
        crc = zlib.crc32(content) & 0xffffffff
        uncomp_size = len(content)
        name_bytes = name.encode('utf-8')

        if comp_type == 8:
            comp_obj = zlib.compressobj(zlib.Z_BEST_COMPRESSION, zlib.DEFLATED, -15)
            comp_data = comp_obj.compress(content) + comp_obj.flush()
            comp_size = len(comp_data)
            extra = b''
            cd_extra = b''
        else:
            comp_data = content
            comp_size = uncomp_size
            # Android alignment extra field: 0xd935 header
            # Data offset must be a multiple of 4
            # offset + 30 (LFH fixed) + len(name_bytes) + 6 (header 4 + align 2) + pad
            pad = (4 - ((offset + 30 + len(name_bytes) + 6) % 4)) % 4
            extra_len = 2 + pad
            extra = struct.pack('<HHH', 0xd935, extra_len, 4) + (b'\x00' * pad)
            cd_extra = b'\x00' # CD extra tag

        lfh = struct.pack('<4sHHHHHIIIHH', b'PK\x03\x04', 20, 0, comp_type, 0, 0, crc, comp_size, uncomp_size, len(name_bytes), len(extra))
        zip_bytes.extend(lfh)
        zip_bytes.extend(name_bytes)
        zip_bytes.extend(extra)
        zip_bytes.extend(comp_data)

        cd = struct.pack('<4sHHHHHHIIIHHHHHII', b'PK\x01\x02', 20, 20, 0, comp_type, 0, 0, crc, comp_size, uncomp_size, len(name_bytes), len(cd_extra), 0, 0, 0, 0, offset)
        cd_entries.append((cd, name_bytes, cd_extra))

    # Zero-pad sec1 so that the APK Signing Block starts on a 4096-byte page boundary
    pad_sec1 = (4096 - (len(zip_bytes) % 4096)) % 4096
    zip_bytes.extend(b'\x00' * pad_sec1)
    sec1 = bytes(zip_bytes)
    assert len(sec1) % 4096 == 0, "sec1 not 4096-byte aligned"

    # Assemble Central Directory (sec2)
    cd_bytes = bytearray()
    for cd, nb, ex in cd_entries:
        cd_bytes.extend(cd)
        cd_bytes.extend(nb)
        cd_bytes.extend(ex)
    sec2 = bytes(cd_bytes)
    cd_size = len(sec2)

    # 5. Compute APK Signature Scheme v2 & v3 blocks
    print("[5/6] Generating APK Signature Scheme v2 & v3 blocks...")
    eocd_modified = struct.pack('<4sHHHHIIH', b'PK\x05\x06', 0, 0, len(cd_entries), len(cd_entries), cd_size, len(sec1), 0)
    
    root_digest = compute_chunked_root_hash(sec1, sec2, eocd_modified)
    print(f"      Calculated Root SHA-256 Digest: {root_digest.hex()}")

    # Digest sequence (common to v2 and v3)
    digest_elem = struct.pack('<I', 4 + 4 + len(root_digest)) + struct.pack('<I', 0x0103) + struct.pack('<I', len(root_digest)) + root_digest
    digests_seq = struct.pack('<I', len(digest_elem)) + digest_elem

    # Certificate sequence
    cert_elem = struct.pack('<I', len(cert_der)) + cert_der
    certs_seq = struct.pack('<I', len(cert_elem)) + cert_elem

    # --- Construct v2 Block ---
    # v2 Additional attributes contains stripping protection pointing to v3:
    # 0xbeeff00d (STRIPPING_PROTECTION_ATTR_ID) = 3
    v2_attrs = bytes.fromhex('0c000000080000000df0efbe0300000000000000')
    v2_signed_data = digests_seq + certs_seq + v2_attrs
    v2_sig_bytes = key.sign(v2_signed_data, padding.PKCS1v15(), hashes.SHA256())
    v2_sig_elem = struct.pack('<I', 4 + 4 + len(v2_sig_bytes)) + struct.pack('<I', 0x0103) + struct.pack('<I', len(v2_sig_bytes)) + v2_sig_bytes
    v2_sigs_seq = struct.pack('<I', len(v2_sig_elem)) + v2_sig_elem
    v2_signer = struct.pack('<I', len(v2_signed_data)) + v2_signed_data + v2_sigs_seq + struct.pack('<I', len(pk_der)) + pk_der
    v2_signers_seq = struct.pack('<I', 4 + len(v2_signer)) + struct.pack('<I', len(v2_signer)) + v2_signer
    v2_pair = struct.pack('<Q', 4 + len(v2_signers_seq)) + struct.pack('<I', 0x7109871a) + v2_signers_seq

    # --- Construct v3 Block ---
    min_sdk = 24
    max_sdk = 0x7fffffff
    v3_signed_data = digests_seq + certs_seq + struct.pack('<II', min_sdk, max_sdk) + struct.pack('<I', 0)
    v3_sig_bytes = key.sign(v3_signed_data, padding.PKCS1v15(), hashes.SHA256())
    v3_sig_elem = struct.pack('<I', 4 + 4 + len(v3_sig_bytes)) + struct.pack('<I', 0x0103) + struct.pack('<I', len(v3_sig_bytes)) + v3_sig_bytes
    v3_sigs_seq = struct.pack('<I', len(v3_sig_elem)) + v3_sig_elem
    v3_signer = struct.pack('<I', len(v3_signed_data)) + v3_signed_data + struct.pack('<II', min_sdk, max_sdk) + v3_sigs_seq + struct.pack('<I', len(pk_der)) + pk_der
    v3_signers_seq = struct.pack('<I', 4 + len(v3_signer)) + struct.pack('<I', len(v3_signer)) + v3_signer
    v3_pair = struct.pack('<Q', 4 + len(v3_signers_seq)) + struct.pack('<I', 0xf05368c0) + v3_signers_seq

    # --- Assemble Signing Block with 4096-byte alignment ---
    raw_block_content = v2_pair + v3_pair
    # Total block size = 8 (size) + len(raw_block_content) + 8 (size) + 16 (magic) = len(raw_block_content) + 32
    total_block_size = len(raw_block_content) + 32
    target_block_size = ((total_block_size + 4095) // 4096) * 4096
    pad_needed = target_block_size - total_block_size
    if pad_needed > 0:
        assert pad_needed >= 12, "Padding size must be at least 12 bytes"
        padding_pair = struct.pack('<Q', pad_needed - 8) + struct.pack('<I', 0x42726577) + (b'\x00' * (pad_needed - 12))
        raw_block_content += padding_pair

    block_size = len(raw_block_content) + 24
    apk_signing_block = struct.pack('<Q', block_size) + raw_block_content + struct.pack('<Q', block_size) + b"APK Sig Block 42"
    assert (len(sec1) + len(apk_signing_block)) % 4096 == 0, "Central Directory offset not 4096-byte aligned"

    # Update CD offset in EOCD
    final_cd_offset = len(sec1) + len(apk_signing_block)
    final_eocd = struct.pack('<4sHHHHIIH', b'PK\x05\x06', 0, 0, len(cd_entries), len(cd_entries), cd_size, final_cd_offset, 0)

    final_apk = sec1 + apk_signing_block + sec2 + final_eocd

    # 6. Verify and Save APK
    print("[6/6] Verifying output APK signatures and ZIP integrity...")
    is_valid, msg = verify_apk(final_apk)
    if not is_valid:
        print(f" [ERROR] Verification failed: {msg}")
        sys.exit(1)
    print(f"      [VERIFIED] {msg}")

    for out_apk in OUT_APKS:
        os.makedirs(os.path.dirname(out_apk), exist_ok=True)
        with open(out_apk, 'wb') as out_f:
            out_f.write(final_apk)
        print(f"      [OK] Saved: {out_apk} ({len(final_apk):,} bytes)")

    print("\n" + "=" * 68)
    print(" [SUCCESS] CyberRelay APK generated and signed with v1 + v2 + v3!")
    print("=" * 68)

if __name__ == '__main__':
    main()
