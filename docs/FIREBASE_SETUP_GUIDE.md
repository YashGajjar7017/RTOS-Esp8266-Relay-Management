# Firebase Realtime Database Worldwide Cloud Control Setup

This guide explains how to connect your **ESP8266** and **Android / Web App** to **Firebase Realtime Database (RTDB)** so you can monitor, trigger, and manage your relay from anywhere in the world over mobile data (4G/5G) or external Wi-Fi!

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
5. Copy your **Database URL** from the top of the page. It will look like:
   ```
   https://cyber-relay-rtos-default-rtdb.firebaseio.com/
   ```

---

## 3. Configure Database Security Rules

In the **Rules** tab of the Realtime Database, paste the following rules and click **Publish**:

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

---

## 4. Add Database URL to ESP8266 Firmware

Open [`firmware/include/config.h`](../firmware/include/config.h) or [`arduino_sketch/esp8266_relay_rtos/config.h`](../arduino_sketch/esp8266_relay_rtos/config.h):

1. Set your Home/Office Wi-Fi SSID and Password so the ESP8266 has internet access:
   ```cpp
   #define ENABLE_INTERNET_WIFI      true
   #define WIFI_STA_SSID             "Your_Home_WiFi_Name"
   #define WIFI_STA_PASSWORD         "Your_Home_WiFi_Password"
   ```
2. Paste your Firebase host URL without `https://` and without trailing slashes:
   ```cpp
   #define FIREBASE_HOST             "cyber-relay-rtos-default-rtdb.firebaseio.com"
   #define FIREBASE_AUTH             "" // Leave empty for test mode or paste Database Secret
   ```
3. Flash the firmware to your ESP8266!

---

## 5. How Worldwide Cloud Sync Works

```
  [ ESP8266 Relay Hardware ]
             │
             │ (Syncs every 2 seconds via HTTPS REST)
             ▼
  [ Firebase Realtime Database ]
  /esp8266_relay
    ├── telemetry/ (Live countdown, relay status, lockout timer, monthly analytics)
    ├── commands/  (Trigger, emergency cutoff, admin lockout bypass)
    └── pendingRequest/ (Level 2 1-time authorization queue for Mahi_Gajjar)
             ▲
             │ (WebSockets / Realtime SDK)
  [ Android App on Phone (Anywhere in the World over 4G/5G) ]
```

- When you are **near the ESP8266**: The app can talk directly to the SoftAP at `192.168.4.1`.
- When you are **away from home**: The app connects through Firebase Cloud, letting you monitor live countdowns, view monthly switch statistics, and approve 1-time requests from anywhere in the world!
