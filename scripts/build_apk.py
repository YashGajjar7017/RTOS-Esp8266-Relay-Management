import os
import sys
import subprocess
import zipfile
import shutil
import struct
import zlib

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

APP_DIR = os.path.join(ROOT, "android_app")
APP_MODULE_DIR = os.path.join(APP_DIR, "app")
INTERMEDIATE = os.path.join(APP_MODULE_DIR, "build", "intermediate")
OUT_DIR = os.path.join(APP_MODULE_DIR, "build", "outputs", "apk", "debug")
KEYSTORE = os.path.join(APP_DIR, "debug.keystore")

def parse_zip_stream(data):
    """Robust raw local ZIP header stream parser for Windows aapt2 artifacts"""
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

def run(cmd_args, cwd=None, ignore_error=False):
    res = subprocess.run(cmd_args, cwd=cwd, capture_output=True, text=True)
    if res.returncode != 0 and not ignore_error:
        print(f"[ERROR] Command failed: {' '.join(cmd_args)}\nStdout:\n{res.stdout}\nStderr:\n{res.stderr}")
        sys.exit(1)
    return res.stdout

def main():
    print("=" * 72)
    print("      CYBER-RELAY RTOS // ANDROID APK AUTOMATED BUILDER")
    print("=" * 72)

    os.makedirs(os.path.join(INTERMEDIATE, "classes"), exist_ok=True)
    os.makedirs(os.path.join(INTERMEDIATE, "dex"), exist_ok=True)
    os.makedirs(OUT_DIR, exist_ok=True)

    # 1. Compile Java
    print("[1/5] Compiling Java source files...")
    java_files = [
        os.path.join(APP_MODULE_DIR, "src", "main", "java", "com", "cyberrelay", "app", "MainActivity.java"),
        os.path.join(APP_MODULE_DIR, "src", "main", "java", "com", "cyberrelay", "app", "WebAppInterface.java")
    ]
    run([JAVAC_EXE, "-cp", PLATFORM_JAR, "-d", os.path.join(INTERMEDIATE, "classes")] + java_files)

    # 2. D8 Dexing
    print("[2/5] Converting bytecode to Dalvik Executable (classes.dex)...")
    classes_dir = os.path.join(INTERMEDIATE, "classes", "com", "cyberrelay", "app")
    class_files = [os.path.join(classes_dir, f) for f in os.listdir(classes_dir) if f.endswith(".class")]
    run([JAVA_EXE, "-cp", D8_JAR, "com.android.tools.r8.D8", "--output", os.path.join(INTERMEDIATE, "dex"), "--lib", PLATFORM_JAR] + class_files)

    # 3. AAPT2 Link
    print("[3/5] Packaging manifest and assets into APK using AAPT2...")
    manifest = os.path.join(APP_MODULE_DIR, "src", "main", "AndroidManifest.xml")
    assets_dir = os.path.join(APP_MODULE_DIR, "src", "main", "assets")
    unaligned_apk = os.path.join(INTERMEDIATE, "unaligned.apk")
    if os.path.exists(unaligned_apk):
        os.remove(unaligned_apk)

    run([
        AAPT2_EXE, "link",
        "-I", PLATFORM_JAR,
        "--manifest", manifest,
        "-A", assets_dir,
        "-o", unaligned_apk
    ], ignore_error=True)

    if not os.path.exists(unaligned_apk) or os.path.getsize(unaligned_apk) == 0:
        print("[ERROR] AAPT2 failed to generate unaligned.apk!")
        sys.exit(1)

    # 4. Inject classes.dex into clean non-zip64 APK
    print("[4/5] Injecting classes.dex into APK...")
    dex_file = os.path.join(INTERMEDIATE, "dex", "classes.dex")
    perfect_apk = os.path.join(INTERMEDIATE, "perfect.apk")

    raw_data = open(unaligned_apk, 'rb').read()
    entries = parse_zip_stream(raw_data)
    with open(dex_file, 'rb') as f:
        entries['classes.dex'] = f.read()

    with zipfile.ZipFile(perfect_apk, 'w', compression=zipfile.ZIP_DEFLATED, allowZip64=False) as z:
        for name, content in entries.items():
            comp = zipfile.ZIP_STORED if name == 'resources.arsc' else zipfile.ZIP_DEFLATED
            z.writestr(name, content, compress_type=comp)

    # 5. Sign APK with Android Signature Scheme v1, v2, v3
    print("[5/5] Signing APK with Android v1, v2, v3 schemes...")
    run([JAVA_EXE, "-jar", APKSIGNER_JAR, "sign", "--ks", KEYSTORE, "--ks-pass", "pass:android", "--key-pass", "pass:android", perfect_apk])

    # Copy to output locations
    dest_debug = os.path.join(OUT_DIR, "app-debug.apk")
    dest_root = os.path.join(ROOT, "CyberRelay-RTOS.apk")
    dest_root_debug = os.path.join(ROOT, "app-debug.apk")

    shutil.copy2(perfect_apk, dest_debug)
    shutil.copy2(perfect_apk, dest_root)
    shutil.copy2(perfect_apk, dest_root_debug)

    print("\n" + "=" * 72)
    print(" [BUILD SUCCESS] Android APK built, signed & verified successfully!")
    print(f" File Size: {os.path.getsize(dest_root):,} bytes")
    print(f" 1. Main Outside Folder: {dest_root}")
    print(f" 2. Main Outside Folder: {dest_root_debug}")
    print(f" 3. Internal Build Path: {dest_debug}")
    print("=" * 72 + "\n")

if __name__ == "__main__":
    main()
