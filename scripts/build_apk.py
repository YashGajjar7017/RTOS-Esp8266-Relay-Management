import os
import sys
import subprocess
import struct
import zlib
import shutil

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
SDK_DIR = r"C:\Program Files (x86)\Android\android-sdk"
BUILD_TOOLS = os.path.join(SDK_DIR, "build-tools", "35.0.0")
PLATFORM_JAR = os.path.join(SDK_DIR, "platforms", "android-36", "android.jar")
JAVA_BIN = r"C:\Users\Admin\scoop\apps\openjdk21\current\bin"
JAVA_EXE = os.path.join(JAVA_BIN, "java.exe")
JAVAC_EXE = os.path.join(JAVA_BIN, "javac.exe")

D8_JAR = os.path.join(BUILD_TOOLS, "lib", "d8.jar")
APKSIGNER_JAR = os.path.join(BUILD_TOOLS, "lib", "apksigner.jar")
AAPT2_EXE = os.path.join(BUILD_TOOLS, "aapt2.exe")
ZIPALIGN_EXE = os.path.join(BUILD_TOOLS, "zipalign.exe")

APP_DIR = os.path.join(ROOT, "android_app")
APP_MODULE_DIR = os.path.join(APP_DIR, "app")
INTERMEDIATE = os.path.join(APP_MODULE_DIR, "build", "intermediate")
OUT_DIR = os.path.join(APP_MODULE_DIR, "build", "outputs", "apk", "debug")
KEYSTORE = os.path.join(APP_DIR, "debug.keystore")

def parse_zip_stream(data):
    """Robust raw local ZIP header stream parser for Windows AAPT2 output"""
    entries = {}
    pos = 0
    while pos < len(data):
        sig = data[pos:pos+4]
        if sig != b'PK\x03\x04':
            break
        version, flags, comp_method, mod_time, mod_date, crc32, comp_size, uncomp_size, name_len, extra_len = struct.unpack('<HHHHHIIIHH', data[pos+4:pos+30])
        name = data[pos+30:pos+30+name_len].decode('utf-8', errors='ignore')
        header_len = 30 + name_len + extra_len
        file_data = data[pos+header_len : pos+header_len+comp_size]
        
        if comp_method == 0:
            decomp = file_data
        elif comp_method == 8:
            decomp = zlib.decompress(file_data, -15)
        else:
            decomp = file_data
        entries[name] = decomp
        pos = pos + header_len + comp_size
    return entries

def create_aligned_zip(entries, out_path):
    """Creates a standard ZIP file with 4-byte alignment on uncompressed entries and classes.dex"""
    with open(out_path, 'wb') as f:
        central_dir = []
        for name, content in entries.items():
            offset = f.tell()
            # Stored without compression for resources.arsc (required by Android)
            comp_type = 0 if name == 'resources.arsc' else 8
            crc = zlib.crc32(content) & 0xffffffff
            uncomp_size = len(content)

            if comp_type == 8:
                comp_obj = zlib.compressobj(zlib.Z_DEFAULT_COMPRESSION, zlib.DEFLATED, -15)
                comp_data = comp_obj.compress(content) + comp_obj.flush()
                comp_size = len(comp_data)
                extra = b''
            else:
                comp_data = content
                comp_size = uncomp_size
                # 4-byte boundary alignment for uncompressed data
                name_len = len(name.encode('utf-8'))
                padding = (4 - ((offset + 30 + name_len) % 4)) % 4
                extra = b'\x00' * padding

            name_bytes = name.encode('utf-8')
            lfh = struct.pack('<4sHHHHHIIIHH', b'PK\x03\x04', 20, 0, comp_type, 0, 0, crc, comp_size, uncomp_size, len(name_bytes), len(extra))
            f.write(lfh)
            f.write(name_bytes)
            f.write(extra)
            f.write(comp_data)

            cd = struct.pack('<4sHHHHHHIIIHHHHHII', b'PK\x01\x02', 20, 20, 0, comp_type, 0, 0, crc, comp_size, uncomp_size, len(name_bytes), len(extra), 0, 0, 0, 0, offset)
            central_dir.append((cd, name_bytes, extra))

        cd_offset = f.tell()
        for cd, nb, ex in central_dir:
            f.write(cd)
            f.write(nb)
            f.write(ex)
        cd_size = f.tell() - cd_offset

        eocd = struct.pack('<4sHHHHIIH', b'PK\x05\x06', 0, 0, len(central_dir), len(central_dir), cd_size, cd_offset, 0)
        f.write(eocd)

def run(cmd_args, cwd=None, ignore_error=False):
    res = subprocess.run(cmd_args, cwd=cwd, capture_output=True, text=True)
    if res.returncode != 0 and not ignore_error:
        print(f"[ERROR] Command failed: {' '.join(cmd_args)}\nStdout:\n{res.stdout}\nStderr:\n{res.stderr}")
        sys.exit(1)
    return res.stdout

def main():
    print("=" * 72)
    print("      CYBER-RELAY RTOS // ROBUST ANDROID APK BUILDER")
    print("=" * 72)

    classes_out = os.path.join(INTERMEDIATE, "classes")
    dex_out = os.path.join(INTERMEDIATE, "dex")
    if os.path.exists(classes_out):
        shutil.rmtree(classes_out)
    if os.path.exists(dex_out):
        shutil.rmtree(dex_out)

    os.makedirs(classes_out, exist_ok=True)
    os.makedirs(dex_out, exist_ok=True)
    os.makedirs(OUT_DIR, exist_ok=True)

    # 1. Compile Java source code to Java 8 bytecode
    print("[1/5] Compiling Java source files (Java 8 target for 100% Android compatibility)...")
    java_files = [
        os.path.join(APP_MODULE_DIR, "src", "main", "java", "com", "cyberrelay", "app", "MainActivity.java"),
        os.path.join(APP_MODULE_DIR, "src", "main", "java", "com", "cyberrelay", "app", "WebAppInterface.java")
    ]
    run([
        JAVAC_EXE,
        "-source", "1.8",
        "-target", "1.8",
        "-cp", PLATFORM_JAR,
        "-d", classes_out
    ] + java_files)

    # 2. D8 Dexing to Dalvik Executable with min-api 21
    print("[2/5] Converting bytecode to Dalvik Executable (classes.dex, min-api 21)...")
    classes_dir = os.path.join(classes_out, "com", "cyberrelay", "app")
    class_files = [os.path.join(classes_dir, f) for f in os.listdir(classes_dir) if f.endswith(".class")]
    run([
        JAVA_EXE,
        "-cp", D8_JAR,
        "com.android.tools.r8.D8",
        "--min-api", "21",
        "--output", dex_out,
        "--lib", PLATFORM_JAR
    ] + class_files)

    # 3. AAPT2 Compile Resources and Link Manifest + Assets
    print("[3/5] Compiling resources and packaging manifest/assets with AAPT2...")
    res_dir = os.path.join(APP_MODULE_DIR, "src", "main", "res")
    compiled_flat_dir = os.path.join(INTERMEDIATE, "compiled_flat")
    if os.path.exists(compiled_flat_dir):
        shutil.rmtree(compiled_flat_dir)
    os.makedirs(compiled_flat_dir, exist_ok=True)

    run([
        AAPT2_EXE, "compile",
        "--dir", res_dir,
        "-o", compiled_flat_dir
    ])

    flat_files = [os.path.join(compiled_flat_dir, f) for f in os.listdir(compiled_flat_dir) if f.endswith('.flat')]

    manifest = os.path.join(APP_MODULE_DIR, "src", "main", "AndroidManifest.xml")
    assets_dir = os.path.join(APP_MODULE_DIR, "src", "main", "assets")
    unaligned_raw = os.path.join(INTERMEDIATE, "raw_stream.apk")
    if os.path.exists(unaligned_raw):
        os.remove(unaligned_raw)

    link_cmd = [
        AAPT2_EXE, "link",
        "-I", PLATFORM_JAR,
        "--manifest", manifest,
        "-A", assets_dir,
        "-o", unaligned_raw,
        "--auto-add-overlay"
    ] + flat_files

    run(link_cmd, ignore_error=True)

    if not os.path.exists(unaligned_raw) or os.path.getsize(unaligned_raw) == 0:
        print("[ERROR] AAPT2 failed to generate raw_stream.apk!")
        sys.exit(1)

    # 4. Parse stream and inject classes.dex with 4-byte alignment
    print("[4/5] Building perfectly aligned 4-byte APK package with classes.dex...")
    raw_data = open(unaligned_raw, 'rb').read()
    entries = parse_zip_stream(raw_data)
    
    dex_file = os.path.join(dex_out, "classes.dex")
    with open(dex_file, 'rb') as f:
        entries['classes.dex'] = f.read()

    aligned_apk = os.path.join(INTERMEDIATE, "aligned.apk")
    create_aligned_zip(entries, aligned_apk)

    # Verify zipalign with SDK tool
    align_ver = run([ZIPALIGN_EXE, "-c", "-v", "4", aligned_apk])
    print(f"      Zipalign Check:\n{align_ver.strip()}")

    # 5. Sign APK with apksigner (v1, v2, v3 schemes)
    print("[5/5] Signing APK with apksigner (Android v1, v2, v3 schemes)...")
    run([
        JAVA_EXE,
        "-jar", APKSIGNER_JAR,
        "sign",
        "--ks", KEYSTORE,
        "--ks-pass", "pass:android",
        "--key-pass", "pass:android",
        aligned_apk
    ])

    # 6. Verify APK signature
    verify_out = run([
        JAVA_EXE,
        "-jar", APKSIGNER_JAR,
        "verify",
        "-v",
        aligned_apk
    ])
    print(f"      Signature Verification:\n{verify_out.strip()}")

    # Copy output APKs to destination paths
    dest_debug = os.path.join(OUT_DIR, "app-debug.apk")
    dest_root = os.path.join(ROOT, "CyberRelay-RTOS.apk")
    dest_root_debug = os.path.join(ROOT, "app-debug.apk")

    shutil.copy2(aligned_apk, dest_debug)
    shutil.copy2(aligned_apk, dest_root)
    shutil.copy2(aligned_apk, dest_root_debug)

    print("\n" + "=" * 72)
    print(" [BUILD SUCCESS] Android APK built, 4-byte aligned & signed successfully!")
    print(f" File Size: {os.path.getsize(dest_root):,} bytes")
    print(f" 1. Main Outside Folder: {dest_root}")
    print(f" 2. Debug Outside Folder: {dest_root_debug}")
    print(f" 3. Module Build Output:  {dest_debug}")
    print("=" * 72 + "\n")

if __name__ == "__main__":
    main()
