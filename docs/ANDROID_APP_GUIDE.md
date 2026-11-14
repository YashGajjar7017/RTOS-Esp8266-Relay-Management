# CYBER-RELAY RTOS // Android Application & Permanent Session Guide

## 1. Overview & Key Enhancements

The Cyber-Relay RTOS Android Application (`CyberRelay-RTOS.apk`) provides a full native interface for managing your ESP8266 relay system.

### What Was Fixed (Addressing App Crashes):
1. **Bytecode Compatibility Fix:**
   - Previous builds compiled Java with Java 21 bytecode targets (`major version 65`), causing instant runtime crashes (`VerifyError` / `UnsupportedClassVersionError`) on Android phones.
   - Now explicitly compiled with `-source 1.8 -target 1.8` and desugared with D8 targeting `--min-api 21`, ensuring 100% compatibility across Android 5.0 through Android 15+.
2. **4-Byte Zip Alignment (Zipalign):**
   - Android OS rejects or crashes on unaligned APKs.
   - The build pipeline now enforces strict **4-byte alignment** on all uncompressed resources and `classes.dex`, fully verified by `zipalign -c 4`.
3. **CORS & Universal File Access:**
   - Enabled `setAllowUniversalAccessFromFileURLs(true)` and `setAllowFileAccessFromFileURLs(true)`. This prevents the Chromium WebView from blocking `fetch()` requests from `file:///android_asset` to `http://192.168.4.1` and Firebase Cloud.
4. **Global Crash Interceptor:**
   - Installed `Thread.setDefaultUncaughtExceptionHandler` to log errors gracefully without force-closing the application.
5. **Cleartext Traffic & Network Security:**
   - Configured `android:usesCleartextTraffic="true"` and network security rules allowing HTTP traffic to local subnets (`192.168.4.1`).

---

## 2. Dedicated Login & Permanent Session Management

The app features a dedicated **Sign-In & Registration Portal**:

### Permanent Session Storage (Stored Forever on Phone):
- When logging in or registering, the checkbox **"Remember session forever on this device"** is enabled by default.
- The session is stored in two persistent storage layers:
  1. **Android Native `SharedPreferences`** (`cyberrelay_app_prefs`) via `AndroidNative.saveSession()`.
  2. **WebView `localStorage`** (`cyberrelay_session`).
- When the app is closed, swiped away, or the phone is rebooted:
  - On launch, the app reads the stored credentials from native storage.
  - It automatically validates the user, skips the login portal, and opens the **Live Dashboard** immediately!
- The user remains logged in **forever** until they explicitly tap the **`[ ⏻ Logout ]`** button in the header.

### Enrolled Predefined User Profiles:
| Profile | Username | Default Password | Role & Permissions |
|---|---|---|---|
| 🛡️ **Super Admin** | `Yash_Gajjar` | `Iamyashacker@17` | Level 1: Full hardware control, lockout bypass, timer calibrations, and receives popups to authorize Level 2 requests. |
| ⚡ **Guarded Operator** | `Mahi_Gajjar` | `MahiSecure#2026` | Level 2: Guarded operator. Triggers require 1-time admin approval popups. Session destroys after 1 use. |
| 👁️ **Guest Analytics** | `Guest` | `Guest@123` | Level 3: Strict read-only access to view live countdowns and Monthly Switch Usage Analytics. |

### Registering New Operators:
- Tap the **"Register New"** tab on the login portal.
- Enter desired username, password, and select privilege tier (Super Admin, Guarded Operator, or Guest).
- The account is saved into phone storage and persists across launches.

---

## 3. Dual Connection Mode: Local AP vs Firebase Cloud

### Mode 1: Local Access Point (`192.168.4.1`)
- Connect your phone's Wi-Fi to **`CYBER-RELAY-HUB`** (Password: `cyber8266`).
- Open the app. It automatically connects to `http://192.168.4.1/api/status`.
- Low-latency real-time control without needing internet.

### Mode 2: Firebase Worldwide Cloud
- Tap **"Firebase Cloud"** in the top navigation bar.
- The app syncs with Google Firebase Realtime Database over **cellular mobile data (4G/5G)** or any remote Wi-Fi network worldwide!
- Tap the **`[ ⚙ Settings ]`** button in the header to view or customize your Firebase Database Host URL or secret key.

---

## 4. Installing the Prebuilt APK onto Your Phone

Prebuilt and signed APK files are located in:
1. **`CyberRelay-RTOS.apk`** (Root directory)
2. **`app-debug.apk`** (Root directory)
3. **`android_app/app/build/outputs/apk/debug/app-debug.apk`**

### Steps to Install:
1. Transfer `CyberRelay-RTOS.apk` to your Android device via USB cable, WhatsApp, Google Drive, or local file transfer.
2. Tap the APK file on your phone.
3. If prompted with *"Install unknown apps"*, tap **Settings** and toggle **"Allow from this source"**.
4. Tap **Install**.
5. Open **CyberRelay RTOS**.
6. Select your user account (or tap Quick Select: `Yash (Admin)`), tap **SIGN IN & CONNECT**, and your session will be saved permanently on your device!

---

## 5. Rebuilding the APK from Source

To rebuild the APK at any time:

### Method 1: Using the Automated Build Script (Fastest, no Android Studio required)
Run from the repository root:
```powershell
python scripts/build_apk.py
```
This script automatically:
1. Compiles Java source files to Java 8 bytecode.
2. Generates `classes.dex` with D8 targeting `min-api 21`.
3. Packages manifest and assets with AAPT2.
4. Performs 4-byte zip alignment.
5. Signs the package with `apksigner` (v1, v2, v3 schemes).
6. Copies the finished APK to the project root.

### Method 2: In Android Studio
1. Open Android Studio.
2. Select **Open** and choose the `android_app/` folder.
3. Wait for Gradle sync to complete.
4. Click **Build > Build Bundle(s) / APK(s) > Build APK(s)** or click the green **Run** button to deploy directly to a connected phone or emulator.

