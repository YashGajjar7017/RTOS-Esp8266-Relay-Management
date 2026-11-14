# 🔐 Cyber-Relay RTOS // Account Credentials & Access Matrix

This local reference file contains the enrolled credentials and access permissions for all 3 system accounts.

---

## 📋 Enrolled User Accounts

| Privilege Level | Username | Password | Role Key | Key Capabilities |
|---|---|---|---|---|
| 🛡️ **Level 1: Super Admin** | `Yash_Gajjar` | `Iamyashacker@17` | `admin` | Full Read/Write, Trigger 8h Cycle, Emergency Cutoff, Bypass Lockout, Timing Calibrations, Instant Authorization Popups |
| ⚡ **Level 2: Guarded Operator** | `Mahi_Gajjar` | `MahiSecure#2026` | `guarded_operator` | Can request triggers; requires 1-time admin approval from `Yash_Gajjar`. Session destroys after single use |
| 👁️ **Level 3: Guest Analytics** | `Guest` | `Guest@123` | `guest_analytics` | Strict Read-Only. Hardware controls locked. Full access to Monthly Switch Runtime Analytics |

---

## 🌐 Network & Device Defaults

* **Local SoftAP SSID:** `CYBER-RELAY-HUB`
* **Local SoftAP Password:** `cyber8266`
* **Default IP Address:** `http://192.168.4.1/`
* **Firebase Cloud Host:** `cyber-relay-rtos-default-rtdb.firebaseio.com`

