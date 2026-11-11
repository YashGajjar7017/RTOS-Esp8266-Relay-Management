# CYBER-RELAY RTOS // User Guide & Operating Manual

## 1. Quick Start Guide

### Step 1: Connect to ESP8266 Wi-Fi
1. Power on your ESP8266 circuit.
2. On your smartphone, tablet, or PC, open **Wi-Fi Settings**.
3. Search for the network:
   - **SSID**: `CYBER-RELAY-HUB`
   - **Password**: `cyber8266`
4. Connect to it. Once connected, your device may show a "Sign in to network" prompt (Captive Portal). Tap it, or open your web browser and navigate to:
   ```
   http://192.168.4.1/
   ```

> [!TIP]
> **Install as App on Phone (PWA)**:
> In Chrome (Android) or Safari (iOS), tap the menu (⋮ or Share icon) and select **"Add to Home Screen"**. The Cyber-Relay UI will run full-screen just like a native app without browser URL bars!

---

## 2. Core Operation & Cycle Logic

The system operates across three strictly governed states:

```
  [ IDLE / READY ]
         │
         │  (Touch Sensor Pressed or App Trigger)
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

### Power-Cut & Reboot Memory
The countdown and operational state are saved every 30 seconds to the ESP8266 Flash storage (LittleFS). If power cuts off or the device reboots:
- The system automatically resumes from where the timer left off.
- The lockout countdown cannot be bypassed by turning the device off and on again.

---

## 3. Changing Custom Hours & Days (Calibration)

In the app under **"Timing Calibration"**:
1. **Relay Active Duration**:
   - Adjust the slider between `1.0` and `24.0` hours (default is `8.0` hours; range 7–8 hours).
2. **Lockout Cooldown Duration**:
   - Adjust the slider between `1.0` and `30.0` days (target is `5.0` to `7.0` days; default is `6.0` days).
3. Tap **"Save & Flash to ESP8266 Memory"**. The changes take effect immediately for the next cycle.

---

## 4. Inbuilt User Database & Admin Authorization

The ESP8266 stores up to **5 user accounts** directly in flash memory.

### Default Master Credentials:
- **Username**: `admin`
- **Password**: `admin123`

### Adding a New User / Operator:
To ensure security, regular operators cannot add new accounts without the administrator's permission:
1. Tap **"+ Register New Operator"** in the app.
2. Fill in:
   - **New Operator Username**
   - **New Operator Password**
   - **Role**: `Operator` (can view and trigger) or `Admin` (full control)
3. Under **Admin Password Confirmation**, enter the current **Master Admin Password** (`admin123`).
4. Tap **"Authorize & Enroll User"**.
5. The ESP8266 validates the admin password, writes the new record to flash, and activates the user account immediately.

### User Roles & Permissions:
| Capability | Guest (No login) | Operator | Admin |
|---|---|---|---|
| View real-time status & countdown | Yes | Yes | Yes |
| Manual Relay trigger | No | Yes | Yes |
| Emergency Cutoff | No | Yes | Yes |
| Change Timer Configurations | No | Yes | Yes |
| Bypass / Reset Lockout | No | No | **Yes** |
| Add / Delete Users | No | No | **Yes** |

---

## 5. Offline Simulation / Web App Testing
You can preview the app interface without flashing an ESP8266:
1. Simply double-click `web_app/index.html` in your browser.
2. The app detects standard browser mode and automatically starts the built-in simulator.
3. You can test slider adjustments, circular countdown animations, user additions, and simulated cycles on your PC or phone!

