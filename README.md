# CYBER-RELAY RTOS // ESP8266 Relay Management & Firebase Cloud Hub

[![Platform: ESP8266](https://img.shields.io/badge/Platform-ESP8266-red.svg)](https://espressif.com/)
[![Android App](https://img.shields.io/badge/Android-Native%20Studio%20Project-green.svg)](#android-application)
[![Cloud: Firebase](https://img.shields.io/badge/Cloud-Firebase%20RTDB%20Worldwide-orange.svg)](#firebase-cloud-control)
[![Architecture: 3-Tier Security](https://img.shields.io/badge/Security-3--Tier%20RBAC%20%2B%201--Time%20Popups-purple.svg)](#3-tier-user-architecture)
[![UI: Cyberpunk Glassmorphism](https://img.shields.io/badge/UI-Cyberpunk%20Glassmorphism-cyan.svg)](#features)

An industrial-grade **RTOS-based IoT relay controller** for ESP8266 featuring capacitive touch activation, high-duty duration timers (7–8 hours), an unskippable countdown lockout cooldown (5–7 days), an inbuilt **3-tier user architecture**, real-time **Admin confirmation popups**, **monthly on/off switch analytics**, **Firebase Realtime Database worldwide cloud control**, and a dedicated **Android Application**.

---

## ⚡ 3-Tier User Security Matrix

| User | Credentials | Privilege Level | Action Policy |
|---|---|---|---|
| **Tier 1: Super Admin** | `Yash_Gajjar`<br>`Iamyashacker@17` | **Master Control** | Full system read/write, change timer calibration (7–8h ON, 5–7d Lockout), manual triggers, emergency cutoff, lockout bypass, and receives instant popups to grant/deny 1-time requests. |
| **Tier 2: Guarded Operator** | `Mahi_Gajjar`<br>`MahiSecure#2026` | **Guarded 1-Time Session** | Can request triggers or calibrations, but **every action requires instant Admin approval**. An interactive popup appears on `Yash_Gajjar`'s app. Once approved, the action executes **one time only** and the session/token is destroyed immediately. |
| **Tier 3: Guest Analytics** | `Guest`<br>`Guest@123` | **Strict Read-Only** | Hardware controls and calibration sliders are locked. Accesses the **Monthly Switch Runtime & Analysis Dashboard** showing total switch ON hours, OFF hours, duty cycle %, and completed cycles. |

---

## 📱 Android Application

The project includes a complete native Android Studio project in [`android_app/`](android_app/):
- **Full Offline & Local Control**: Connects directly to the ESP8266 Access Point (`CYBER-RELAY-HUB` at `http://192.168.4.1/`).
- **Worldwide Firebase Cloud Mode**: When away from home on mobile data (4G/5G), switches automatically to Firebase Realtime Database for worldwide monitoring and control!
- **Haptic Vibration & Popups**: Triggers vibration alerts whenever Admin approval is requested.
- **How to Build**: Open `android_app/` in Android Studio and click **Run** or **Build APK**.

---

## ☁️ Firebase Cloud Worldwide Integration

Control and view your ESP8266 from anywhere in the world!
- Bidirectional real-time telemetry sync with **Firebase Realtime Database**.
- Low-latency command execution (`/esp8266_relay/commands`).
- Real-time authorization queue (`/esp8266_relay/pendingRequest`).
- See [docs/FIREBASE_SETUP_GUIDE.md](docs/FIREBASE_SETUP_GUIDE.md) for complete 5-minute setup steps.

---

## 🔄 Core Cycle & Lockout Behavior

```
  [ IDLE / READY ]
         │
         │  (Touch Sensor Pressed or Authorized App Trigger)
         ▼
  [ RELAY ACTIVE ] ───────────► Relay turns ON for 7–8 hours (Configurable)
         │                     Active countdown displays in App
         │
         │  (Duty cycle timer reaches 00:00:00)
         ▼
  [ LOCKOUT COOLDOWN ] ───────► Relay turns OFF
         │                     LOCKOUT ACTIVE for 5–7 days (Configurable)
         │                     **ANY TOUCH SENSOR PRESS IS STRICTLY IGNORED**
         │
         │  (Lockout countdown reaches 00:00:00)
         ▼
  [ IDLE / READY ] ───────────► Touch sensor re-enabled for next cycle
```

- **Power-Loss Memory**: The countdown seconds and state are saved to Flash NVRAM every 30 seconds. A reboot or power outage will never reset or cheat the cooldown lockout!

---

## 📁 Repository Structure

```
RTOS-Esp8266-Relay-Management/
├── android_app/                      <-- Native Android Studio Application
│   ├── app/src/main/
│   │   ├── java/com/cyberrelay/app/  <-- MainActivity.kt & WebAppInterface.kt
│   │   ├── assets/                   <-- Embedded offline cyberpunk UI
│   │   └── AndroidManifest.xml
│   ├── build.gradle.kts & settings.gradle.kts
├── arduino_sketch/
│   └── esp8266_relay_rtos/           <-- Open in Arduino IDE
│       ├── esp8266_relay_rtos.ino    <-- Main firmware & setup
│       ├── config.h                  <-- Pins, timers, Wi-Fi & Firebase settings
│       ├── state_manager.h/.cpp      <-- 3-phase cycle & lockout engine
│       ├── storage_db.h/.cpp         <-- 3-Tier User DB & Monthly Analytics
│       ├── firebase_client.h/.cpp    <-- Firebase Cloud Worldwide Sync
│       ├── web_server_api.h/.cpp     <-- REST API & Captive Portal
│       ├── rtos_tasks.h/.cpp         <-- RTOS task scheduler
│       └── web_assets.h              <-- Embedded PROGMEM Web GUI
├── firmware/
│   ├── platformio.ini                <-- Configured for NodeMCU / ESP-01
│   ├── include/ & src/               <-- Modular C++ firmware source
│   └── data/index.html               <-- Inlined web app for LittleFS
├── web_app/
│   ├── index.html                    <-- Standalone source HTML
│   ├── style.css                     <-- Cyberpunk glassmorphic styling
│   └── app.js                        <-- Reactive client controller & simulator
├── docs/
│   ├── CIRCUIT_DIAGRAM.md            <-- Schematics, pinouts & wiring guide
│   ├── USER_GUIDE.md                 <-- Full operating and admin manual
│   └── FIREBASE_SETUP_GUIDE.md       <-- Firebase Realtime Database setup
└── README.md                         <-- Project documentation
```

---

## 🚀 Flashing & Quick Start

### 1. Hardware Connections
- Relay Signal: **GPIO5 / D1** (or **GPIO1 / TX** via `#define USE_PHYSICAL_GPIO1_FOR_RELAY true`).
- Touch Sensor: **GPIO4 / D2**.
- Power: 5V DC to VIN/Relay, 3.3V to Touch sensor, common ground.

### 2. Flashing
1. Open [`arduino_sketch/esp8266_relay_rtos/esp8266_relay_rtos.ino`](arduino_sketch/esp8266_relay_rtos/esp8266_relay_rtos.ino) in the Arduino IDE.
2. Install **ArduinoJson** (v6.x) via Library Manager.
3. Select **NodeMCU 1.0 (ESP-12E Module)** and your COM Port, then click **Upload**.

### 3. Log In
Connect to Wi-Fi `CYBER-RELAY-HUB` (password: `cyber8266`) and open `http://192.168.4.1/`:
- **Admin**: `Yash_Gajjar` / `Iamyashacker@17`
- **Guarded Operator**: `Mahi_Gajjar` / `MahiSecure#2026`
- **Guest**: `Guest` / `Guest@123`

---

## 🛡️ License

MIT License.
