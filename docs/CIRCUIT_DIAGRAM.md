# Hardware Wiring & Circuit Guide

This guide details how to wire the **ESP8266** (NodeMCU ESP-12E or ESP-01), **TTP223 Capacitive Touch Sensor**, and **Single-Channel Relay Module**.

---

## 1. Bill of Materials (BOM)

| Component | Quantity | Description / Recommended Specs |
|---|---|---|
| **ESP8266 Board** | 1 | NodeMCU v2/v3, ESP-12E/F, or ESP-01S |
| **Relay Module** | 1 | 5V / 3.3V Single-Channel Relay Module (Optocoupler-isolated) |
| **Touch Sensor** | 1 | TTP223 Capacitive Digital Touch Sensor Module |
| **Power Supply** | 1 | 5V DC 1A - 2A regulated power adapter |
| **Jumper Wires** | — | Female-to-Female & Male-to-Female Dupont wires |

---

## 2. Pin Mapping & Connections

### Option A: Standard NodeMCU v2 / ESP-12E (Recommended)
This configuration keeps UART Serial logging available on GPIO1/GPIO3 while driving the relay and touch module on dedicated GPIO pins.

| Component | Component Pin | NodeMCU Pin | ESP8266 GPIO | Notes |
|---|---|---|---|---|
| **Relay Module** | `VCC` | `VIN` or `5V` | — | Power from 5V supply |
| **Relay Module** | `GND` | `GND` | — | Common ground |
| **Relay Module** | `IN` | **`D1`** | **`GPIO5`** | Relay control signal (Active LOW) |
| **TTP223 Touch** | `VCC` | `3V3` | — | 3.3V DC power |
| **TTP223 Touch** | `GND` | `GND` | — | Common ground |
| **TTP223 Touch** | `SIG` / `I/O` | **`D2`** | **`GPIO4`** | Digital Output (HIGH on touch) |

---

### Option B: ESP-01 / Physical GPIO1 ("Pin 1") Configuration
If your hardware specifically connects the relay to **physical GPIO1 (TXD0)**:

1. Open `config.h` in the firmware.
2. Change `#define USE_PHYSICAL_GPIO1_FOR_RELAY` to `true`:
   ```cpp
   #define USE_PHYSICAL_GPIO1_FOR_RELAY true
   ```
3. Wiring table:
   | Component | Component Pin | ESP-01 Pin | ESP8266 GPIO |
   |---|---|---|---|
   | **Relay Module** | `IN` | **`TX`** | **`GPIO1`** |
   | **TTP223 Touch** | `SIG` | **`RX`** or **`GPIO2`** | **`GPIO3`** or **`GPIO2`** |
   | **Power** | `VCC` | `3V3` | 3.3V Regulated (Max 3.6V) |
   | **Power** | `GND` | `GND` | Common Ground |

> [!WARNING]
> When using `GPIO1 (TX)` for relay output, `Serial.begin()` is disabled in firmware to prevent relay clicking caused by UART transmission boot strings.

---

## 3. Circuit Schematics Diagram

```text
               +----------------------------------+
               |         5V 2A DC SUPPLY          |
               +----------------+-----------------+
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
   |   [GND] --------------------------> Common GND              |
   |   [VIN / 5V] ---------------------> +5V Rail                |
   |   [3V3] --------------------------> +3.3V Rail              |
   +-------------------------------------------------------------+

                                      RELAY HIGH VOLTAGE TERMINALS:
                                      +---------+
   AC Neutral ------------------------| LOAD    |
                                      |         |
   AC Live (Hot) ----+                +---------+
                     |                     |
                     +----> Relay [COM]    |
                            Relay [NO] ----+
```

---

## 4. TTP223 Capacitive Touch Jumper Settings
The TTP223 module features two solder bridge jumpers (`A` and `B`):
- **Jumper A (Output Level)**: Open (Default = Active HIGH output).
- **Jumper B (Trigger Mode)**: Open (Default = Direct Momentary mode).

Keep both jumpers **OPEN** so the sensor sends a momentary HIGH pulse when touched, which the RTOS debouncing task captures accurately.

