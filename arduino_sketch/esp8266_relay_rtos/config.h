#pragma once
#include <Arduino.h>

/**
 * =========================================================================
 *                   CYBER-RELAY RTOS FIRMWARE CONFIGURATION
 * =========================================================================
 */

// -------------------------------------------------------------------------
// 1. HARDWARE PIN DEFINITIONS
// -------------------------------------------------------------------------
#define USE_PHYSICAL_GPIO1_FOR_RELAY false // Set to true if physically wired to ESP-01 / TX (GPIO1)

#if USE_PHYSICAL_GPIO1_FOR_RELAY
  #define PIN_RELAY          1    // GPIO1 (TXD0)
#else
  #define PIN_RELAY          5    // GPIO5 (NodeMCU D1) - standard safe pin
#endif

// Touch Sensor Pin (TTP223 Capacitive Touch Module)
#define PIN_TOUCH            4    // GPIO4 (NodeMCU D2)

// Buzzer Output Pin (NodeMCU D8 = GPIO15)
#define PIN_BUZZER           15   // GPIO15 (NodeMCU D8) - Beep on Relay ON/OFF switch
#define MAX_RELAYS           4    // Dynamic multi-relay capacity (Relay 1 to 4)

// Relay Logic (Active LOW or Active HIGH)
#define RELAY_ACTIVE_LEVEL   LOW
#define RELAY_INACTIVE_LEVEL HIGH

// Touch Module Trigger Logic (TTP223 default output is HIGH when touched)
#define TOUCH_ACTIVE_LEVEL   HIGH
#define TOUCH_DEBOUNCE_MS    80

// -------------------------------------------------------------------------
// 2. TIMING CALIBRATION DEFAULTS
// -------------------------------------------------------------------------
#define DEFAULT_ON_HOURS          8.0f     // Default: 8.0 hours charging countdown
#define MIN_ON_HOURS              0.1f     // Minimum 6 mins (for testing/customizing)
#define MAX_ON_HOURS              48.0f    // Maximum 48 hours

#define DEFAULT_LOCKOUT_DAYS      7.0f     // Default: 7.0 days cooldown timer
#define MIN_LOCKOUT_DAYS          0.01f    // Minimum (~15 mins for fast testing)
#define MAX_LOCKOUT_DAYS          30.0f    // Maximum 30 days

#define PERSIST_INTERVAL_MS       30000    // Commit state to Flash every 30s

// -------------------------------------------------------------------------
// 3. WI-FI ACCESS POINT & INTERNET STATION CONFIGURATION
// -------------------------------------------------------------------------
#define WIFI_AP_SSID              "Xenithra_Tech_Wifi"
#define WIFI_AP_PASSWORD          "XenOS@123"      // SoftAP password
#define WIFI_AP_CHANNEL           1
#define WIFI_AP_MAX_CONN          8
#define HTTP_SERVER_PORT          80

// Station mode to connect ESP8266 to Home/Internet Wi-Fi for Firebase Cloud Control
#define ENABLE_INTERNET_WIFI      true
#define WIFI_STA_SSID             "Asha"  // Set your local router SSID
#define WIFI_STA_PASSWORD         "1111122418"  // Set your local router Password

// -------------------------------------------------------------------------
// 4. FIREBASE CLOUD REALTIME DATABASE CONFIGURATION
// -------------------------------------------------------------------------
// Allows controlling and viewing the ESP8266 from anywhere in the world!
#define FIREBASE_HOST             "cyber-relay-rtos-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH             "YOUR_FIREBASE_DATABASE_SECRET" // Or Firebase Auth Token
#define FIREBASE_SYNC_INTERVAL_MS 2000 // Cloud sync frequency (every 2s)

// -------------------------------------------------------------------------
// 5. PREDEFINED USER CREDENTIALS & 3-TIER ARCHITECTURE
// -------------------------------------------------------------------------
// Tier 1: Master Admin (Full Read/Write, System configs, Permission grants)
#define USER1_NAME                "Yash_Gajjar"
#define USER1_PASS                "Iamyashacker@17"
#define USER1_ROLE                "admin"

// Tier 2: Guarded Operator (System access requiring Admin confirmation popup; 1-time session)
#define USER2_NAME                "Mahi_Gajjar"
#define USER2_PASS                "MahiSecure#2026"
#define USER2_ROLE                "guarded_operator"

// Tier 3: Guest Analytics (Read-only, can only view Monthly On/Off Switch Analytics)
#define USER3_NAME                "Guest"
#define USER3_PASS                "Guest@123"
#define USER3_ROLE                "guest_analytics"

#define MAX_USERS                 5
#define FIRMWARE_VERSION          "3.0.0-FIREBASE-RTOS"
