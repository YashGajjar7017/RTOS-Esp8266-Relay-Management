# CYBER-RELAY RTOS // ESP8266 Relay Management & Firebase Cloud Hub

[![Platform: ESP8266](https://img.shields.io/badge/Platform-ESP8266-red.svg)](https://espressif.com/)
[![Android App](https://img.shields.io/badge/Android-Native%20APK%20(Signed%20%26%20Aligned)-green.svg)](#-android-application--crash-fixes)
[![Cloud: Firebase](https://img.shields.io/badge/Cloud-Firebase%20RTDB%20Worldwide-orange.svg)](#-firebase-cloud-worldwide-control)
[![Architecture: 3-Tier Security](https://img.shields.io/badge/Security-3--Tier%20RBAC%20%2B%20Permanent%20Session-purple.svg)](#-3-tier-user-security-matrix)
[![UI: Cyberpunk Glassmorphism](https://img.shields.io/badge/UI-Cyberpunk%20Glassmorphism-cyan.svg)](#-features)

An industrial-grade **RTOS-based IoT relay controller** for ESP8266 featuring capacitive touch activation, high-duty duration timers (7–8 hours), an unskippable countdown lockout cooldown (5–7 days), an inbuilt **3-tier user architecture**, real-time **Admin confirmation popups**, **monthly on/off switch analytics**, **Firebase Realtime Database worldwide cloud control**, and a dedicated, crash-resistant **Android Mobile Application** with permanent session storage.

---

## ⚡ 3-Tier User Security Matrix

| User Profile | Default Credentials | Privilege Level | Access Policy |
|---|---|---|---|
| 🛡️ **Tier 1: Super Admin** | `Yash_Gajjar`<br>`Iamyashacker@17` | **Master Control** | Full system read/write, change timer calibrations (7–8h ON, 5–7d Lockout), manual triggers, emergency cutoff, lockout bypass, and receives instant real-time popups to grant or deny 1-time operator requests. |
| ⚡ **Tier 2: Guarded Operator** | `Mahi_Gajjar`<br>`MahiSecure#2026` | **Guarded 1-Time Session** | Can view states and request triggers, but **every action requires instant Admin approval**. An interactive popup appears on `Yash_Gajjar`'s phone. Once authorized with the admin password, the action executes **one time only** and the session token is burned immediately. |
| 👁️ **Tier 3: Guest Analytics** | `Guest`<br>`Guest@123` | **Strict Read-Only** | Hardware controls and calibration sliders are completely locked. Accesses the **Monthly Switch Runtime & Analysis Dashboard** showing total switch ON hours, OFF hours, duty cycle percentage, and completed cycle counts. |

---

## 📱 Android Application & Crash Fixes

The Android Application (`CyberRelay-RTOS.apk` and `app-debug.apk`) is packaged in [`android_app/`](android_app/) and built with automated signing tools in [`scripts/build_apk.py`](scripts/build_apk.py).

### Why the App Previously Crashed & How It Was Fixed:
1. **Target Bytecode Incompatibility:**
   - Previous builds compiled Java sources without specifying bytecode compatibility, emitting Java 21 classfiles (`major version 65`) that caused immediate fatal `VerifyError` and `UnsupportedClassVersionError` crashes on Android ART runtimes.
   - **Fix:** Explicitly configured `javac -source 1.8 -target 1.8` and D8 with `--min-api 21`, ensuring rock-solid compatibility across Android 5.0 through Android 15+.
2. **Page Alignment Violation (Missing Zipalign):**
   - Android OS rejects or crashes when loading unaligned APKs.
   - **Fix:** The build pipeline now enforces strict **4-byte boundary alignment** across all uncompressed data and `classes.dex`, fully verified with `zipalign -c 4`.
3. **WebView CORS Blocking:**
   - Android Chromium WebView blocks cross-origin requests from `file:///android_asset` by default.
   - **Fix:** Enabled `setAllowUniversalAccessFromFileURLs(true)` and `setAllowFileAccessFromFileURLs(true)` so the embedded client can communicate freely with `http://192.168.4.1/` and Firebase HTTPS endpoints.
4. **Global Uncaught Exception Handler:**
   - Installed `Thread.setDefaultUncaughtExceptionHandler` to log errors gracefully without force-closing the application.
5. **Cleartext Local Traffic:**
   - Added `network_security_config.xml` and `android:usesCleartextTraffic="true"` to guarantee unhindered communication with local IP subnets (`192.168.4.1`).

### Permanent Session Storage (Stored Forever on Phone):
- Dedicated **Login & Sign-In Portal** with quick-select account chips (`Yash_Gajjar`, `Mahi_Gajjar`, `Guest`) and custom user registration.
- **"Remember session forever on this device"** checkbox (checked by default):
  - Stores credentials in Android Native **`SharedPreferences`** (`cyberrelay_app_prefs`) via the Java Native Interface bridge (`AndroidNative.saveSession()`) and browser `localStorage`.
  - Closing, force-killing, or rebooting the phone retains the session. On next launch, the app automatically skips the login screen and transitions straight to the active dashboard.
  - Users remain signed in forever until they explicitly tap the **`[ ⏻ Logout ]`** button.

---

## 📶 Dual Connection: Local AP vs Firebase Cloud

Switch effortlessly between offline direct hardware communication and worldwide cloud control:

```
  ┌─────────────────────────────────────────────────────────────┐
  │                 CYBER-RELAY CONNECTION MODES                 │
  ├──────────────────────────────┬──────────────────────────────┤
  │       LOCAL AP MODE          │    FIREBASE CLOUD MODE       │
  │     (Offline / Near ESP)     │     (Worldwide / 4G/5G)      │
  ├──────────────────────────────┼──────────────────────────────┤
  │ SSID: CYBER-RELAY-HUB        │ Network: Any Mobile Data     │
  │ Pass: cyber8266              │ Protocol: HTTPS REST Sync    │
  │ URL:  http://192.168.4.1     │ Latency: ~100–300 ms         │
  │ Captive Portal: Port 53 DNS  │ Range: Anywhere in the World │
  └──────────────────────────────┴──────────────────────────────┘
```

- **Local AP Mode:** Connect your phone's Wi-Fi to `CYBER-RELAY-HUB` (`http://192.168.4.1/`). Instant control without requiring an external router or internet connection.
- **Firebase Cloud Mode:** Tap **"Firebase Cloud"** in the top navigation bar. Operates over cellular mobile data (4G/5G) or remote Wi-Fi, allowing you to monitor countdowns and trigger hardware from anywhere in the world!
- **In-App Cloud Settings:** Tap the **`[ ⚙ ]` (Settings)** button in the header to enter your custom Firebase Realtime Database URL and database secret without re-compiling the app!

---

## 🔄 Core Cycle & Lockout State Machine

```
  [ IDLE / READY ]
         │
         │  (Touch Sensor Pressed or Authorized App Trigger)
         ▼
  [ RELAY ACTIVE ] ───────────► Relay turns ON for 7.0–8.0 hours (Configurable)
         │                     Active countdown displays in App & Web
         │
         │  (Active timer reaches 00:00:00)
         ▼
  [ LOCKOUT COOLDOWN ] ───────► Relay turns OFF
         │                     LOCKOUT ACTIVE for 5.0–7.0 days (Configurable)
         │                     **ANY TOUCH SENSOR PRESS IS STRICTLY IGNORED**
         │
         │  (Lockout countdown reaches 00:00:00)
         ▼
  [ IDLE / READY ] ───────────► Touch sensor re-enabled for next cycle
```

- **Power-Loss Memory Protection:** The countdown remaining seconds and operating mode commit to Flash memory every 30 seconds. A power cut or device restart will **never** reset or cheat the cooldown lockout!

---

## 📁 Repository Structure

```
RTOS-Esp8266-Relay-Management/
├── CyberRelay-RTOS.apk               <-- Main Prebuilt, Signed & 4-Byte Aligned Android APK
├── app-debug.apk                     <-- Debug Root APK
├── android_app/                      <-- Native Android Studio Project
│   ├── app/src/main/
│   │   ├── java/com/cyberrelay/app/  <-- MainActivity.java & WebAppInterface.java
│   │   ├── assets/                   <-- Embedded offline cyberpunk UI assets
│   │   ├── res/                      <-- Themes, colors, network security config
│   │   └── AndroidManifest.xml       <-- Permissions & hardware flags
│   └── build.gradle.kts
├── arduino_sketch/
│   └── esp8266_relay_rtos/           <-- Open in Arduino IDE (Single Folder Sketch)
│       ├── esp8266_relay_rtos.ino    <-- Main firmware & RTOS setup
│       ├── config.h                  <-- Pins, timers, Wi-Fi & Firebase settings
│       ├── state_manager.h/.cpp      <-- 3-phase cycle & lockout engine
│       ├── storage_db.h/.cpp         <-- 3-Tier User DB & Monthly Analytics
│       ├── firebase_client.h/.cpp    <-- Firebase Cloud Worldwide Sync
│       ├── web_server_api.h/.cpp     <-- REST API & Captive Portal
│       ├── rtos_tasks.h/.cpp         <-- RTOS task scheduler
│       └── web_assets.h              <-- Embedded PROGMEM Web GUI
├── firmware/                         <-- PlatformIO Project
│   ├── platformio.ini                <-- Configured for NodeMCU / ESP-01
│   ├── include/ & src/               <-- Modular C++ firmware source
│   └── data/index.html               <-- Inlined web app for LittleFS
├── web_app/
│   ├── index.html                    <-- Source Single Page App (Login & Dashboard)
│   ├── style.css                     <-- Cyberpunk glassmorphic styling
│   └── app.js                        <-- Reactive client controller & Firebase REST engine
├── scripts/
│   ├── build_apk.py                  <-- Automated Java 8, D8, Zipalign & Apksigner script
│   └── pack_web.js                   <-- Inlines HTML/CSS/JS into PROGMEM web_assets.h
├── docs/
│   ├── CONNECTION_AND_SERVER_GUIDE.md <-- Complete Wi-Fi, Captive Portal & REST API manual
│   ├── ANDROID_APP_GUIDE.md          <-- APK installation, permanent session & build guide
│   ├── FIREBASE_SETUP_GUIDE.md       <-- Firebase Realtime Database setup & rules
│   ├── CIRCUIT_DIAGRAM.md            <-- Schematics, pinouts & wiring guide
│   └── USER_GUIDE.md                 <-- Full operating and admin manual
└── README.md                         <-- Project documentation
```

---

## 🚀 Quick Start Guide

### 1. Flash the ESP8266 Firmware:
- **Using Arduino IDE:**
  1. Open [`arduino_sketch/esp8266_relay_rtos/esp8266_relay_rtos.ino`](arduino_sketch/esp8266_relay_rtos/esp8266_relay_rtos.ino).
  2. In `config.h`, set your home Wi-Fi (`WIFI_STA_SSID` / `WIFI_STA_PASSWORD`) and your Firebase URL (`FIREBASE_HOST`).
  3. Select your board (e.g. `NodeMCU 1.0 (ESP-12E Module)` or `Generic ESP8266 Module`) and click **Upload**.
- **Using PlatformIO:**
  Run `pio run -t upload` inside the `firmware/` folder.

### 2. Install the Android App:
1. Transfer **`CyberRelay-RTOS.apk`** to your Android phone.
2. Open the file, allow installation from unknown sources, and tap **Install**.
3. Launch **CyberRelay RTOS**.
4. Select your account (or tap Quick Select: `Yash_Gajjar`), keep **"Remember session forever"** checked, and tap **SIGN IN & CONNECT**.
5. Control your relay locally or switch to **Firebase Cloud** for worldwide management!

---

## 📚 Complete Technical Documentation

- 🔌 **[Connection & Server Architecture Guide](docs/CONNECTION_AND_SERVER_GUIDE.md)**: Deep dive into Access Point configuration, Captive Portal redirection, and complete REST API endpoint documentation with curl examples.
- 📱 **[Android Application Guide](docs/ANDROID_APP_GUIDE.md)**: Details on crash resolution, permanent session architecture, and APK building steps.
- ☁️ **[Firebase Cloud Setup Guide](docs/FIREBASE_SETUP_GUIDE.md)**: Step-by-step setup for Firebase Realtime Database, security rules, and testing worldwide controls.
- 📐 **[Circuit & Wiring Schematics](docs/CIRCUIT_DIAGRAM.md)**: Pinout diagrams for NodeMCU, Wemos D1 Mini, and ESP-01 with relay and capacitive touch modules.
- 📖 **[User & Administration Manual](docs/USER_GUIDE.md)**: Complete guide to user privilege levels, lockout policies, and timer calibrations.
