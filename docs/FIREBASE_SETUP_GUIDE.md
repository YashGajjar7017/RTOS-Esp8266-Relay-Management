# Firebase Realtime Database Worldwide Cloud Control Setup

This guide explains how to connect your **ESP8266** and the **Android Application** to **Google Firebase Realtime Database (RTDB)** so you can monitor, trigger, calibrate, and manage your relay from anywhere in the world over mobile data (4G/5G) or remote Wi-Fi!

---

## 1. Create a Free Firebase Project

1. Go to the [Firebase Console](https://console.firebase.google.com/).
2. Click **Add project** (or **Create a project**).
3. Name it (e.g. `cyber-relay-rtos`), disable Google Analytics (optional), and click **Create Project**.

---

## 2. Set Up Firebase Realtime Database

1. In the left navigation menu of the Firebase Console, go to **Build** -> **Realtime Database**.
2. Click **Create Database**.
3. Choose your database location (e.g., `United States` or `Singapore`) and click **Next**.
4. Select **Start in test mode** (allows read and write during setup), then click **Enable**.
5. Copy your **Database URL** from the top of the database viewer. It looks like:
   ```
   https://cyber-relay-rtos-default-rtdb.firebaseio.com/
   ```

---

## 3. Configure Database Security Rules

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
> If you wish to use Database Secrets or Auth Tokens, retrieve your Database Secret under **Project Settings > Service Accounts > Database Secrets**, and paste it into the ESP8266 `config.h` (`FIREBASE_AUTH`) and into the Android App's Cloud Config modal.

---

## 4. Configure the ESP8266 Firmware

Open [`firmware/include/config.h`](../firmware/include/config.h) or [`arduino_sketch/esp8266_relay_rtos/config.h`](../arduino_sketch/esp8266_relay_rtos/config.h):

1. **Set your Home Wi-Fi Router SSID and Password** so the ESP8266 can connect to the internet:
   ```cpp
   #define ENABLE_INTERNET_WIFI      true
   #define WIFI_STA_SSID             "Your_Home_WiFi_Name"
   #define WIFI_STA_PASSWORD         "Your_Home_WiFi_Password"
   ```
2. **Paste your Firebase host URL** without `https://` and without trailing slashes:
   ```cpp
   #define FIREBASE_HOST             "your-project-default-rtdb.firebaseio.com"
   #define FIREBASE_AUTH             "" // Leave empty for test mode or paste secret
   ```
3. Flash the firmware to your ESP8266 using PlatformIO or Arduino IDE!

---

## 5. Configure the Android App (No Recompilation Required!)

You do NOT need to rebuild the APK to point it to your Firebase database!
1. Open the **CyberRelay RTOS** app on your phone.
2. Sign in with your account.
3. In the top navigation bar, tap the **`[ ⚙ ]` (Settings)** button.
4. Enter your **Firebase Realtime Database Host URL** (e.g. `your-project-default-rtdb.firebaseio.com`).
5. (Optional) Enter your Database Secret / Auth Key if required by your rules.
6. Tap **Test Firebase Ping**. You will see:
   `Firebase Ping OK (xxx ms) - Connected!`
7. Tap **Save Settings**.
8. Tap **"Firebase Cloud"** in the top navigation bar. The status pill will glow green:
   `FIREBASE CLOUD (ONLINE)`

---

## 6. How Worldwide Cloud Sync Works

```
  [ ESP8266 Hardware Relay ]
             │
             │ (Syncs every 2 seconds via HTTPS REST)
             ▼
  [ Firebase Realtime Database ]
  /esp8266_relay
    ├── telemetry/ (Live countdown, relay status, lockout timer, monthly analytics)
    ├── commands/  (Trigger, emergency cutoff, admin lockout bypass)
    └── pendingRequest/ (Level 2 1-time authorization queue for Mahi_Gajjar)
             ▲
             │ (Direct HTTPS REST Fetch)
  [ Android App on Phone (Anywhere in the World over 4G/5G Cellular Data) ]
```

### Telemetry Sync:
- The ESP8266 periodically uploads live status to `/esp8266_relay/telemetry.json`.
- The Android App polls this document every 1.5 seconds.

### Remote Worldwide Commands:
- When you tap **"Trigger Relay Cycle"**, **"Emergency Cutoff"**, or **"Bypass Lockout"** on your phone, the app writes a JSON payload to `/esp8266_relay/commands.json`.
- The ESP8266 RTOS Task 4 receives the command, triggers the hardware relay, and clears the command.

### Worldwide 1-Time Session Approvals:
- When **`Mahi_Gajjar`** requests permission from her phone, a request is posted to `/esp8266_relay/telemetry/pendingRequest`.
- An instant notification and authorization popup appears on **`Yash_Gajjar`'s** phone, regardless of where he is in the world!
- Once approved, the relay engages and the 1-time token is destroyed.
