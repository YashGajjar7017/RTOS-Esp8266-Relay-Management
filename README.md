# CYBER-RELAY RTOS // ESP8266 Relay & Touch Management Hub

[![ESP8266](https://img.shields.io/badge/Platform-ESP8266-red.svg)](https://espressif.com/)
[![RTOS](https://img.shields.io/badge/Architecture-RTOS%20Multitasking-blue.svg)](https://freertos.org/)
[![GUI](https://img.shields.io/badge/UI-Cyberpunk%20Glassmorphism-cyan.svg)](#app--ui-features)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

An intelligent, secure **RTOS-based relay management system** for ESP8266 featuring capacitive touch activation, high-duty duration timers (7–8 hours), an unskippable countdown lockout cooldown (5–7 days), an inbuilt flash user database (up to 5 users), and an embedded cyberpunk-glassmorphic responsive Web Application.

---

## ⚡ Highlights & Key Features

| Requirement | Implementation Details |
|---|---|
| **1. RTOS on ESP8266** | Priority task scheduling (15ms touch debounce task, 1Hz countdown manager, 10ms network API server, flash persistence task) with non-blocking cooperative execution. |
| **2. Relay on Pin 1 & Touch Module** | Configured for Relay on GPIO5/D1 (or physical GPIO1/TX) and TTP223 capacitive touch sensor on GPIO4/D2 with hardware debouncing. |
| **3. Custom ON & Lockout Timers** | When triggered, the relay turns ON for **7 to 8 hours** (customizable). When expired, the system enters an **unskippable Lockout for 5 to 7 days**, during which **any press on the touch sensor is completely locked out and ignored**. |
| **4. Inbuilt Flash Database** | Micro-database stored in LittleFS NVRAM for **3 to 5 users** with role-based access control (`admin` vs `operator`). |
| **5. High-End "Sexy" App GUI** | Ultra-modern dark-mode glassmorphic single-page application with neon glows, circular SVG countdown gauges, digital clocks, tactile hardware pills, and real-time auto-polling. |
| **6. Admin Approval Popup** | Registration of new operators requires entering the Master Admin password in a confirmation modal before the account is activated. |
| **7. Power-Loss Persistence** | Remaining ON and Lockout countdown seconds are saved to NVRAM so power cuts or reboots will not reset or bypass the lockout cooldown! |

---

## 📐 Circuit & Hardware Connections

```
                +---------------------------------+
                |        5V 2A POWER SUPPLY       |
                +----------------+----------------+
                                 |  +5V
                                 +---------> Relay Module VCC
                                 |  GND
                                 +---------> Common Ground (All devices)
                                 |
                          [ 3.3V Regulator ]
                                 |
                                 +---------> ESP8266 3V3 Pin
                                 +---------> TTP223 Touch VCC

    +-------------------------------------------------------------+
    |                        ESP8266 NodeMCU                      |
    |                                                             |
    |   [GPIO5 / D1] -------------------> Relay Module [IN]       |
    |   [GPIO4 / D2] <------------------- TTP223 Touch [SIG]      |
    |                                                             |
    |   [GND] --------------------------> Common Ground           |
    |   [VIN / 5V] ---------------------> +5V Rail                |
    |   [3V3] --------------------------> +3.3V Rail              |
    +-------------------------------------------------------------+
```

> See [docs/CIRCUIT_DIAGRAM.md](docs/CIRCUIT_DIAGRAM.md) for full schematic details and ESP-01 GPIO1 pin mapping notes.

---

## 📁 Repository Structure

```
RTOS-Esp8266-Relay-Management/
├── arduino_sketch/
│   └── esp8266_relay_rtos/           <-- Ready to open in Arduino IDE
│       ├── esp8266_relay_rtos.ino    <-- Main Arduino sketch entry point
│       ├── config.h                  <-- Pins, timers, and Wi-Fi credentials
│       ├── state_manager.h/.cpp      <-- 3-phase state machine & persistence
│       ├── storage_db.h/.cpp         <-- LittleFS User Database (Max 5 users)
│       ├── web_server_api.h/.cpp     <-- REST API & Captive Portal
│       ├── rtos_tasks.h/.cpp         <-- RTOS task scheduler
│       └── web_assets.h              <-- Embedded PROGMEM Web GUI
├── firmware/
│   ├── platformio.ini                <-- Ready for PlatformIO (VS Code)
│   ├── include/ & src/               <-- Modular C++ firmware source
│   └── data/index.html               <-- Inlined web app for LittleFS
├── web_app/
│   ├── index.html                    <-- Standalone source HTML
│   ├── style.css                     <-- Cyberpunk glassmorphism styling
│   └── app.js                        <-- Reactive client controller & simulator
├── scripts/
│   └── pack_web.js                   <-- Auto-packer script (Web -> C++ header)
├── docs/
│   ├── CIRCUIT_DIAGRAM.md            <-- Schematics and pinout options
│   └── USER_GUIDE.md                 <-- Comprehensive operating manual
└── README.md                         <-- Project documentation
```

---

## 🚀 How to Flash & Run

### Method 1: Using Arduino IDE
1. Install [Arduino IDE](https://www.arduino.cc/en/software) (v1.8.x or 2.x).
2. Go to **File -> Preferences**, and add this URL to **Additional Boards Manager URLs**:
   ```
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
3. Open **Tools -> Board -> Boards Manager**, search for `esp8266`, and install the **ESP8266** board package.
4. Go to **Tools -> Manage Libraries**, search for `ArduinoJson`, and install **ArduinoJson by Benoît Blanchon** (v6.x).
5. Open `arduino_sketch/esp8266_relay_rtos/esp8266_relay_rtos.ino`.
6. Select your board (e.g. **NodeMCU 1.0 (ESP-12E Module)**) and COM Port.
7. Click **Upload**!

### Method 2: Using PlatformIO (VS Code)
1. Open the `firmware/` directory in VS Code with the PlatformIO extension installed.
2. Click the PlatformIO **Build** button (✓) and **Upload** button (→).
3. (Optional) Run `Upload Filesystem Image` to flash LittleFS partition.

---

## 📱 How to Connect to the App

1. Power on the ESP8266.
2. Connect to the Wi-Fi Access Point:
   - **SSID**: `CYBER-RELAY-HUB`
   - **Password**: `cyber8266`
3. Open any web browser and go to:
   ```
   http://192.168.4.1/
   ```
4. **Login**:
   - Default Master Username: `admin`
   - Default Master Password: `admin123`

---

## 🛡️ License

This project is licensed under the MIT License.

