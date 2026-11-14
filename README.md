# CYBER-RELAY RTOS // ESP8266 Relay Management & Firebase Cloud Hub

[![Platform: ESP8266](https://img.shields.io/badge/Platform-ESP8266-red.svg)](https://espressif.com/)
[![Android App](https://img.shields.io/badge/Android-Native%20APK%20(Signed%20%26%20Aligned)-green.svg)](#-android-application--crash-fixes)
[![Cloud: Firebase](https://img.shields.io/badge/Cloud-Firebase%20RTDB%20Worldwide-orange.svg)](#-firebase-realtime-database-setup--cloud-architecture)
[![Architecture: 3-Tier Security](https://img.shields.io/badge/Security-3--Tier%20RBAC%20%2B%20Permanent%20Session-purple.svg)](#-3-tier-user-security-matrix)
[![UI: Obsidian Amber Glassy Prism](https://img.shields.io/badge/UI-Obsidian%20Amber%20Prism-gold.svg)](#-mobile-responsive-ui--compact-header)

An industrial-grade **RTOS-based IoT relay controller** for ESP8266 featuring capacitive touch activation, high-duty duration timers (7–8 hours), an unskippable countdown lockout cooldown (5–7 days), an inbuilt **3-tier user architecture**, real-time **Admin confirmation popups**, **monthly on/off switch analytics**, **Firebase Realtime Database worldwide cloud control**, and a dedicated, crash-resistant **Android Mobile Application** with permanent session storage.

---

## ⚡ 3-Tier User Security Matrix & Master Recovery

| User Profile | Default Credentials | Privilege Level | Access Policy |
|---|---|---|---|
| 🛡️ **Tier 1: Super Admin** | `Yash_Gajjar`<br>`Iamyashacker@17` | **Master Control** | Full system read/write, change timer calibrations (7–8h ON, 5–7d Lockout), manual triggers, emergency cutoff, lockout bypass, **extend running timers** (+15m, +30m, +1h, +2h), **temporarily suspend/enable user accounts**, and grant/deny 1-time operator requests. |
| ⚡ **Tier 2: Guarded Operator** | `Mahi_Gajjar`<br>`MahiSecure#2026` | **Guarded 1-Time Session** | Can view states and request triggers, but **every action requires instant Admin approval**. An interactive popup appears on `Yash_Gajjar`'s phone. Once authorized with the admin password, the action executes **one time only** and the session token is burned immediately. |
| 👁️ **Tier 3: Guest Analytics** | `Guest`<br>`Guest@123` | **Strict Read-Only** | Hardware controls and calibration sliders are completely locked. Accesses the **Monthly Switch Runtime & Analysis Dashboard** showing total switch ON hours, OFF hours, duty cycle percentage, and completed cycle counts. |

### 🔑 In-App Forgot Password & Flash Recovery:
- If a user forgets their password, they can reset it directly from the app login screen by tapping **"Forgot Password?"**.
- The app verifies authorization against the ESP8266 chip database (`/users.json` on LittleFS flash):
  - **Operator & Guest Accounts:** Can be reset using the **Super Admin Password** (`Iamyashacker@17`) or the Hardware Master Recovery PIN.
  - **Super Admin Account (`Yash_Gajjar`):** Can be reset using the **Hardware Master Recovery PIN**: `CYBER-8266-ROOT` or `cyber8266`.
- Password updates can be performed locally via HTTP (`/api/auth/reset_password`) or remotely via Firebase Cloud (`RESET_PASSWORD` action).

---

## ☁️ Firebase Realtime Database Setup & Cloud Architecture

To monitor, trigger, calibrate, and manage your relay from anywhere in the world over cellular data (4G/5G) or remote Wi-Fi, set up a free Google Firebase Realtime Database (RTDB):

### Step 1: Create a Free Firebase Project
1. Open the [Firebase Console](https://console.firebase.google.com/) and sign in with your Google account.
2. Click **Add project** (or **Create a project**).
3. Name your project (e.g. `cyber-relay-rtos`), disable Google Analytics (optional), and click **Create Project**.

### Step 2: Create the Realtime Database Instance
1. In the left sidebar of the Firebase Console, navigate to **Build > Realtime Database**.
2. Click **Create Database**.
3. Choose a server location closest to your region (e.g., `United States (us-central1)` or `Singapore (asia-southeast1)`) and click **Next**.
4. Select **Start in test mode** and click **Enable**.
5. Copy your **Database URL** displayed at the top of the database viewer:
   ```text
   https://your-project-id-default-rtdb.firebaseio.com/
   ```

### Step 3: Configure Database Security Rules
In the **Rules** tab of your Realtime Database, paste the following rules and click **Publish**:

```json
{
  "rules": {
    "esp8266_relay": {
      ".read": true,
      ".write": true
    }
  }
}
```

> [!TIP]
> If you wish to use Database Secrets / Auth Tokens for authenticated access:
> 1. Go to **Project Settings (gear icon) > Service accounts > Database secrets**.
> 2. Click **Add secret** or copy the existing secret token.
> 3. Paste the secret token into `config.h` (`FIREBASE_AUTH`) and into the Android App's Cloud Config modal.

### Step 4: Configure the ESP8266 Firmware
Open [`firmware/include/config.h`](firmware/include/config.h) or [`arduino_sketch/esp8266_relay_rtos/config.h`](arduino_sketch/esp8266_relay_rtos/config.h):
1. Enable internet uplink and set your home/office Wi-Fi router credentials:
   ```cpp
   #define ENABLE_INTERNET_WIFI      true
   #define WIFI_STA_SSID             "Your_WiFi_Router_Name"
   #define WIFI_STA_PASSWORD         "Your_WiFi_Password"
   ```
2. Set your Firebase host without `https://` and without trailing slashes:
   ```cpp
   #define FIREBASE_HOST             "your-project-id-default-rtdb.firebaseio.com"
   #define FIREBASE_AUTH             "" // Leave empty for test mode or enter secret
   ```

### Step 5: Configure the Android App (No Recompilation Needed!)
You do **not** need to recompile the APK to change Firebase credentials:
1. Open the **CyberRelay RTOS** app on your phone.
2. Go to the **Firebase Cloud & Logs** tab (or tap the **Cloud Log** button in the header).
3. Tap **Network Settings** to open the Cloud Config modal.
4. Paste your **Firebase Realtime Database Host URL** and optional Auth Secret.
5. Tap **Test Ping** to verify connectivity (`Firebase Ping OK (xx ms)`).
6. Tap **Save Settings** — settings persist forever in Android `SharedPreferences`.

### Step 6: Real-Time Cloud Data Structure
```text
/esp8266_relay
  ├── telemetry/
  │     ├── mode: "IDLE" | "RELAY_ON" | "LOCKOUT"
  │     ├── relayState: true | false
  │     ├── onRemainingSec: 28800
  │     ├── lockoutRemainingSec: 0
  │     ├── activeSessionDuration: 28800
  │     ├── wifiRssi: -58
  │     ├── freeHeap: 27940
  │     ├── monthlyAnalytics: { totalOnHours: 124.5, dutyCyclePercent: 16.8, ... }
  │     └── pendingRequest: { active: true, username: "Mahi_Gajjar", action: "TRIGGER" }
  ├── commands/
  │     ├── action: "TRIGGER" | "CANCEL" | "RESET_LOCKOUT" | "EXTEND" | "RESET_PASSWORD" | "SET_USER_STATUS"
  │     ├── requestedBy: "Yash_Gajjar"
  │     └── timestamp: 1731580000
  └── accounts_status/
        ├── Mahi_Gajjar: "active" | "disabled"
        └── Guest: "active" | "disabled"
```

---

## 🔌 Hardware Wiring & Pinout Connection

```
          ┌──────────────────────────────────────────────┐
          │             ESP8266 NodeMCU V2/V3            │
          │                                              │
          │  [3V3] [GND] [D1/GPIO5] [D2/GPIO4] [VIN/5V]  │
          └───┬──────┬─────────┬─────────┬────────┬──────┘
              │      │         │         │        │
              │      │         │         │        └──────────── +5V VCC to Relay Module
              │      │         │         └───────────────────── SIG (I/O) to TTP223 Touch
              │      │         └─────────────────────────────── IN to 5V Relay Module
              │      └───────────────────────────────────────── GND (Common Ground)
              └──────────────────────────────────────────────── +3.3V VCC to TTP223 Touch
```

### Pin Mapping Table:

| Component | Component Pin | ESP8266 NodeMCU Pin | ESP-01 / ESP-12E Pin | Description |
|---|---|---|---|---|
| **5V Relay Module** | `VCC` | `VIN` or `5V` | `5V` External Power | Powered by 5V supply |
| **5V Relay Module** | `GND` | `GND` | `GND` | Common ground |
| **5V Relay Module** | `IN` | `D1` | `GPIO5` | Active-Low Relay Control Signal |
| **TTP223 Capacitive Touch** | `VCC` | `3V3` | `3.3V` | 3.3V regulated power |
| **TTP223 Capacitive Touch** | `GND` | `GND` | `GND` | Common ground |
| **TTP223 Capacitive Touch** | `SIG` / `OUT` | `D2` | `GPIO4` | Active-High Touch Trigger Interrupt |

> [!CAUTION]
> **Mains Voltage Safety:** When wiring the relay terminals (COM, NO, NC) to high-voltage AC loads (110V–240V AC), ensure the device is completely disconnected from mains power before touching wires. Use a properly rated enclosure.

---

## 📶 Dual Connection Modes: Local AP vs Firebase Cloud

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
  │ Gateway: http://192.168.4.1  │ Latency: ~80–250 ms          │
  │ Captive Portal: Port 53 DNS  │ Range: Anywhere in the World │
  └──────────────────────────────┴──────────────────────────────┘
```

- **Local AP Mode:** Connect your phone's Wi-Fi directly to the ESP8266 hotspot `CYBER-RELAY-HUB` (`http://192.168.4.1/`). Operates even in remote areas with zero internet coverage.
- **Local Wi-Fi Protection Modal:** If the app is set to Local AP mode but your phone is disconnected from `CYBER-RELAY-HUB`, a warning dialog automatically prompts you to connect to the ESP Wi-Fi or switch to Firebase Cloud before attempting hardware actions.
- **Firebase Cloud Mode:** Tap **"Cloud"** in the top navigation bar. Operates over cellular mobile data (4G/5G) or remote Wi-Fi, allowing you to monitor countdowns and trigger hardware from anywhere in the world!
- **Real-Time Telemetry & True Ping:** The header displays live round-trip latency (`⚡ 14 ms`) and real Wi-Fi RSSI signal strength (`📶 -52 dBm`).

---

## 📱 Mobile-Responsive UI & Compact Header

The user interface has been engineered with a luxury **Obsidian Amber Apple Glassy Prism** theme, tailored specifically for mobile phone screens:

1. **Expansive Max-Width Login Card:**
   - Full-width layout on mobile devices (`max-width: 100%`) with edge-to-edge padding and spacious typography.
   - Quick-select account buttons for instant role testing.
   - Built-in **"Forgot Password?"** recovery linking directly to ESP flash memory.
2. **Compact Single-Row Header:**
   - Optimized for standard mobile viewports (360px–412px wide) with a fixed 50px height.
   - Removed verbose multiline subtitles; keeps only essential controls:
     - Sidebar hamburger navigation toggle.
     - Dynamic view title.
     - Micro connection switcher (`AP` / `Cloud`).
     - Live ping telemetry indicator (`⚡ 12 ms`).
     - Cloud log launcher icon.
     - Pulsing connection status pill.
3. **Sidebar Multi-View Navigation:**
   - **⚡ Relay Control:** Large radial progress ring, countdown timers, and quick trigger actions.
   - **⏱️ Timer Calibrations:** Sliders for Active Duration (1–24h) and Lockout Cooldown (1–14d).
   - **📊 Monthly Analytics:** Duty cycle percentage, total ON/OFF hours, and monthly cycle totals.
   - **👥 User Accounts:** Active account management and Admin disable/enable toggles.
   - **☁️ Firebase Cloud & Logs:** Real-time event terminal, connection diagnostics, and worldwide test pings.
   - **⚙️ Device Diagnostics:** Hardware memory, flash LittleFS space, uptime, and RSSI metrics.

---

## 🔄 Core Cycle & Lockout State Machine

```
  [ IDLE / READY ]
         │
         │  (Touch Sensor Pressed or Authorized App Trigger)
         ▼
  [ RELAY ACTIVE ] ───────────► Relay turns ON for 7.0–8.0 hours (Configurable)
         │                     Active countdown displays in App & Web
         │                     Admin can extend running timer (+15m, +30m, etc.)
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
│   │   ├── res/                      <-- Themes, colors, network security config, launcher icons
│   │   └── AndroidManifest.xml       <-- Permissions & hardware flags
│   └── build.gradle.kts
├── arduino_sketch/
│   └── esp8266_relay_rtos/           <-- Open in Arduino IDE (Single Folder Sketch)
│       ├── esp8266_relay_rtos.ino    <-- Main firmware & RTOS setup
│       ├── config.h                  <-- Pins, timers, Wi-Fi & Firebase settings
│       ├── state_manager.h/.cpp      <-- 3-phase cycle & lockout countdown engine
│       ├── storage_db.h/.cpp         <-- 3-Tier User DB, LittleFS & Monthly Analytics
│       ├── firebase_client.h/.cpp    <-- Firebase Cloud Worldwide Sync & Commands
│       ├── web_server_api.h/.cpp     <-- REST API & Captive Portal
│       ├── rtos_tasks.h/.cpp         <-- RTOS task scheduler
│       └── web_assets.h              <-- Embedded PROGMEM Web GUI
├── firmware/                         <-- PlatformIO Project
│   ├── platformio.ini                <-- Configured for NodeMCU / ESP-01
│   ├── include/ & src/               <-- Modular C++ firmware source
│   └── data/index.html               <-- Inlined web app for LittleFS
├── web_app/
│   ├── index.html                    <-- Source Single Page App (Login & Dashboard)
│   ├── style.css                     <-- Obsidian Amber Glassy Apple Prism styling
│   └── app.js                        <-- Reactive client controller & Firebase REST engine
├── scripts/
│   ├── build_apk.py                  <-- Automated Java 8, D8, Zipalign & Apksigner script
│   ├── generate_app_icons.ps1        <-- Launcher icon generator for all mipmap buckets
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
  3. Select your board (`NodeMCU 1.0 (ESP-12E Module)`) and click **Upload**.
- **Using PlatformIO:**
  Run `pio run -t upload` inside the `firmware/` folder.

### 2. Install the Android App:
1. Transfer **`CyberRelay-RTOS.apk`** to your Android phone.
2. Open the file, allow installation from unknown sources, and tap **Install**.
3. Launch **CyberRelay RTOS**.
4. Select your account (e.g., Quick Select `Yash_Gajjar`), keep **"Remember session forever"** checked, and tap **SIGN IN & CONNECT**.
5. Switch effortlessly between **AP** (Local) and **Cloud** (Worldwide Firebase)!
