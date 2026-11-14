# CYBER-RELAY RTOS // Connection & Server Architecture Manual

This comprehensive guide covers all physical, local Wi-Fi, and cloud connections between the **ESP8266 Hardware Controller**, the **Web Server & Captive Portal**, the **Android Application**, and **Firebase Realtime Cloud Database**.

---

## 1. Network Topology Overview

Cyber-Relay RTOS operates in **Dual Wi-Fi Mode (`WIFI_AP_STA`)**, enabling simultaneous local offline access and worldwide cloud control.

```
                    ┌──────────────────────────────────────────────┐
                    │               WORLDWIDE INTERNET             │
                    │         (Cellular 4G/5G / Remote Wi-Fi)      │
                    └──────────────────────┬───────────────────────┘
                                           │ HTTPS (REST API)
                                           ▼
                    ┌──────────────────────────────────────────────┐
                    │     GOOGLE FIREBASE REALTIME DATABASE        │
                    │   cyber-relay-rtos-default-rtdb.firebaseio   │
                    └──────────────────────┬───────────────────────┘
                                           │ HTTPS Sync (Every 2s)
                                           ▼
┌──────────────────┐  HTTP 192.168.4.1   ┌────────────────────────────────┐
│   ANDROID APP    │ ◄─────────────────► │      ESP8266 RTOS HUB          │
│ (Offline / Local)│  SoftAP Channel 1   │  SoftAP: CYBER-RELAY-HUB       │
└──────────────────┘                     │  Station: Connected to Router  │
                                         └───────────────┬────────────────┘
                                                         │ GPIO Control
                                                         ▼
                                         ┌────────────────────────────────┐
                                         │ Hardware Relay & TTP223 Touch  │
                                         └────────────────────────────────┘
```

---

## 2. Local Access Point (SoftAP) Connection

When powered on, the ESP8266 automatically broadcasts a secure local Wi-Fi Access Point:

| Parameter | Default Value | Configuration File |
|---|---|---|
| **SSID** | `CYBER-RELAY-HUB` | `config.h` (`WIFI_AP_SSID`) |
| **Password** | `cyber8266` | `config.h` (`WIFI_AP_PASSWORD`) |
| **Gateway / IP** | `192.168.4.1` | Hardcoded in `main.cpp` SoftAP config |
| **Subnet Mask** | `255.255.255.0` | Default Class C subnet |
| **Wi-Fi Channel** | `1` | `config.h` (`WIFI_AP_CHANNEL`) |
| **Max Concurrent Clients** | `8` | `config.h` (`WIFI_AP_MAX_CONN`) |
| **HTTP Web Server Port** | `80` | Standard HTTP |
| **DNS Server Port** | `53` | Built-in Captive Portal DNS |

### How to Connect Locally from Phone or Laptop:
1. Turn on Wi-Fi on your smartphone or PC.
2. Search for available networks and tap **`CYBER-RELAY-HUB`**.
3. Enter password: **`cyber8266`**.
4. Once connected, open any browser and navigate to: **`http://192.168.4.1`**.
5. The ESP8266 serves the embedded Single Page Application directly from Flash PROGMEM (`web_assets.h`).

---

## 3. Captive Portal Auto-Redirection

The firmware features an integrated DNS server listening on UDP Port 53.
Whenever a smartphone connects to `CYBER-RELAY-HUB`:
- Captive probe requests (`/generate_204`, `/hotspot-detect.html`, `/ncsi.txt`) return an immediate HTTP `302 Found` redirecting straight to `http://192.168.4.1/`.
- The phone automatically displays the **"Sign into Wi-Fi network"** notification, opening the Cyber-Relay dashboard instantly without needing to type the IP manually.

---

## 4. Internet Station Mode (Home Router Connection)

To enable worldwide control through Firebase, the ESP8266 simultaneously connects to your home Wi-Fi router.

In `firmware/include/config.h` (or `arduino_sketch/esp8266_relay_rtos/config.h`):
```cpp
#define ENABLE_INTERNET_WIFI      true
#define WIFI_STA_SSID             "Your_Home_WiFi_SSID"      // Replace with your router name
#define WIFI_STA_PASSWORD         "Your_Home_WiFi_Password"  // Replace with your router password
```

### Auto-Reconnection & Heap Protection:
- The firmware configures `WiFi.setAutoReconnect(true)` to automatically restore connection if your router reboots.
- RTOS Task 4 monitors connection status every 5 seconds.
- `client.setBufferSizes(1024, 512)` is applied to SSL sockets to prevent ESP8266 out-of-memory heap crashes during HTTPS communication.

---

## 5. Web Server REST API Specification

The ESP8266 web server handles HTTP requests with full Cross-Origin Resource Sharing (CORS) headers, permitting requests from the Android WebView and desktop browsers.

### CORS Headers Sent on All Responses:
```http
Access-Control-Allow-Origin: *
Access-Control-Allow-Methods: GET, POST, OPTIONS, PUT, DELETE
Access-Control-Allow-Headers: Content-Type, Authorization, X-Requested-With
```

---

### Endpoint 1: System Telemetry & Status
- **Method:** `GET`
- **URL:** `/api/status`
- **Description:** Returns the complete live operational state, remaining countdown seconds, configuration, analytics, and active pending authorization requests.
- **Example Request:**
  ```bash
  curl -X GET http://192.168.4.1/api/status
  ```
- **Example Response (HTTP 200 OK):**
  ```json
  {
    "mode": "IDLE",
    "relayActive": false,
    "touchActive": true,
    "lockoutActive": false,
    "onRemainingSec": 0,
    "totalOnSec": 28800,
    "lockoutRemainingSec": 0,
    "totalLockoutSec": 518400,
    "uptimeSeconds": 142,
    "config": {
      "onHours": 8.0,
      "lockoutDays": 6.0
    },
    "analytics": {
      "month": "November 2026",
      "totalOnHours": 48.5,
      "totalOffHours": 240.2,
      "totalCycles": 6
    },
    "pendingRequest": {
      "active": false,
      "requestId": "",
      "username": "",
      "action": ""
    }
  }
  ```

---

### Endpoint 2: Hardware Relay Control
- **Method:** `POST`
- **URL:** `/api/control`
- **Headers:** `Content-Type: application/json`
- **Actions:**
  - `trigger`: Starts 8.0h duty cycle.
  - `cancel`: Cuts off active cycle immediately and enters lockout.
  - `reset_lockout`: Bypasses lockout cooldown (restricted to `Yash_Gajjar`).
- **Example Request (Trigger Cycle by Admin):**
  ```bash
  curl -X POST http://192.168.4.1/api/control \
    -H "Content-Type: application/json" \
    -d '{"action":"trigger","userRole":"admin","oneTimeToken":""}'
  ```
- **Example Response (HTTP 200 OK):**
  ```json
  {
    "success": true,
    "message": "Relay cycle triggered"
  }
  ```
- **Guarded Operator Rule (Mahi_Gajjar):**
  If `userRole` is `guarded_operator` without a valid `oneTimeToken`, returns `HTTP 403 Forbidden`:
  ```json
  {
    "error": true,
    "message": "One-Time Admin Authorization required for this operation."
  }
  ```

---

### Endpoint 3: Timing Calibration Parameters
- **Method:** `POST`
- **URL:** `/api/config`
- **Headers:** `Content-Type: application/json`
- **Description:** Updates relay active duration (0.5h to 24h) and lockout cooldown (0.5d to 30d). Persists directly into Flash memory.
- **Example Request:**
  ```bash
  curl -X POST http://192.168.4.1/api/config \
    -H "Content-Type: application/json" \
    -d '{"onHours":8.0,"lockoutDays":6.0,"userRole":"admin"}'
  ```
- **Example Response (HTTP 200 OK):**
  ```json
  {
    "success": true,
    "message": "Timing configuration updated and saved"
  }
  ```

---

### Endpoint 4: Authentication Verification
- **Method:** `POST`
- **URL:** `/api/auth/login`
- **Description:** Verifies user credentials against the enrolled 3-Tier database.
- **Example Request:**
  ```bash
  curl -X POST http://192.168.4.1/api/auth/login \
    -H "Content-Type: application/json" \
    -d '{"username":"Yash_Gajjar","password":"Iamyashacker@17"}'
  ```
- **Example Response (HTTP 200 OK):**
  ```json
  {
    "success": true,
    "username": "Yash_Gajjar",
    "role": "admin",
    "token": "esp-auth-1-84210"
  }
  ```

---

### Endpoint 5: One-Time Session Grant Workflow (Mahi_Gajjar)
1. **Submit Request:** `POST /api/auth/request_grant`
   ```json
   { "username": "Mahi_Gajjar", "action": "TRIGGER_RELAY" }
   ```
2. **Admin Approves:** `POST /api/auth/approve_grant`
   ```json
   { "adminPassword": "Iamyashacker@17" }
   ```
   Returns `oneTimeToken`. The session destroys immediately after one action execution.
3. **Admin Rejects:** `POST /api/auth/reject_grant`
   Dismisses and clears the active request.

---

### Endpoint 6: Monthly Analytics
- **Method:** `GET`
- **URL:** `/api/analytics`
- **Description:** Returns total switch ON duration in seconds, OFF duration in seconds, completed cycle counts, and duty percentage.

---

## 6. Firebase Realtime Worldwide Cloud Architecture

When controlling outside your home Wi-Fi, the system uses Firebase Realtime Database:

### Telemetry Document (`/esp8266_relay/telemetry.json`):
- ESP8266 uploads live telemetry via HTTP `PUT` every 2000 ms.
- Mobile App reads `/telemetry.json` and renders live gauges, countdowns, and pending admin notifications.

### Commands Queue (`/esp8266_relay/commands.json`):
- When the App sends a command, it writes:
  ```json
  {
    "action": "TRIGGER",
    "requestedBy": "Yash_Gajjar",
    "userRole": "admin",
    "oneTimeToken": "",
    "timestamp": 1731580000
  }
  ```
- ESP8266 polls `/commands.json` every 2000 ms.
- Once executed, ESP8266 resets `/commands.json` to `null`.

---

## 7. Connection Troubleshooting Checklist

| Issue | Root Cause | Solution |
|---|---|---|
| Cannot open `192.168.4.1` | Phone switched to mobile data | Turn off mobile data temporarily while connecting to `CYBER-RELAY-HUB` so the phone doesn't route traffic away from the local AP. |
| Firebase Cloud says "Offline" | Wi-Fi Station not connected | Verify `WIFI_STA_SSID` and `WIFI_STA_PASSWORD` in `config.h`. Check serial monitor at 115200 baud to see assigned local router IP. |
| CORS or Network Error in App | Universal file access disabled | The fixed Android App enables `setAllowUniversalAccessFromFileURLs(true)`. Rebuild with `python scripts/build_apk.py`. |
| Relay triggers repeatedly | Electrical noise or relay bounce | Ensure relay module VCC is powered with adequate current (5V at 1A) and touch module ground is common with ESP8266 ground. |

