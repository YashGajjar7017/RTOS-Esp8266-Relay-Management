# CYBER-RELAY RTOS // Complete Operating Manual v3.0

## 1. 3-Tier Security & User Architecture

The system features 3 preconfigured user tiers stored directly in flash memory:

### Tier 1: Super Administrator
- **Username**: `Yash_Gajjar`
- **Password**: `Iamyashacker@17`
- **Role**: `admin`
- **Capabilities**:
  - Full system read/write access.
  - Can change Timing Calibration (7 to 8 hours relay active, 5 to 7 days lockout).
  - Can execute manual trigger, emergency cutoff, and lockout bypass.
  - **Receives real-time confirmation popups** whenever `Mahi_Gajjar` requests 1-time access to trigger or configure the system.
  - Controls user enrollments.

---

### Tier 2: Guarded Operator (1-Time Grant Flow)
- **Username**: `Mahi_Gajjar`
- **Password**: `MahiSecure#2026`
- **Role**: `guarded_operator`
- **Capabilities & Strict Policy**:
  - Has system operational privileges, but **CANNOT trigger the relay or modify settings alone**.
  - When `Mahi_Gajjar` attempts any action, a real-time authorization request is dispatched to Firebase / ESP.
  - **An interactive confirmation popup immediately appears on `Yash_Gajjar`'s app**:
    > ⚠️ *"Mahi_Gajjar is requesting permission to Trigger Relay. Enter Yash_Gajjar Admin Password to grant 1-time access."*
  - When Yash approves with his password, a single-use token is issued, the action executes, and **the session/grant is destroyed immediately**!

---

### Tier 3: Guest Analytics (Read-Only)
- **Username**: `Guest`
- **Password**: `Guest@123`
- **Role**: `guest_analytics`
- **Capabilities & Strict Policy**:
  - Strictly **read-only access**.
  - All hardware controls (Trigger, Cutoff, Lockout Bypass) and calibration sliders are locked/disabled.
  - Dedicated **Monthly Switch Analytics Dashboard**:
    - **Total Hours Switch Was ON This Month** (e.g. `48.5 hours`).
    - **Total Hours Switch Was OFF / Locked This Month** (e.g. `240.2 hours`).
    - **Total Duty Cycle Percentage Ratio** (e.g. `16.8% ON`).
    - **Total Relay Runs / Cycles Completed**.

---

## 2. Android App (Building & Running)

The complete Android Studio project is located in [`android_app/`](../android_app/):

1. Open **Android Studio**.
2. Select **File -> Open...** and select the folder `android_app`.
3. Allow Gradle to sync.
4. Connect your Android phone via USB (or use an Android Virtual Device / Emulator).
5. Click **Run 'app'** (or **Build -> Build Bundle(s) / APK(s) -> Build APK(s)**).
6. Install and open the **CyberRelay** app!

### Features in the Android App:
- Auto-detects local ESP8266 Wi-Fi (`CYBER-RELAY-HUB`) or switches to **Firebase Worldwide Cloud Mode** when on mobile data (4G/5G).
- Haptic vibration alerts and sound when an authorization popup arrives on Admin Yash's screen.
- Immersive dark glassmorphic dashboard.

---

## 3. Firebase Worldwide Cloud Control

When enabled:
- The ESP8266 connects to your home/office Wi-Fi and syncs with **Firebase Realtime Database**.
- In the app header, tap **"Firebase Cloud"** to view and control the relay from anywhere across the globe!
- See [`docs/FIREBASE_SETUP_GUIDE.md`](FIREBASE_SETUP_GUIDE.md) for step-by-step database setup.

---

## 4. Hardware Touch & 5–7 Days Lockout Cooldown

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

- State and countdown seconds are saved to NVRAM every 30 seconds.
- A power cut or reboot will **never** reset or cheat the cooldown lockout!
