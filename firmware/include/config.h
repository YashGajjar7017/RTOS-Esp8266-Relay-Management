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

// Relay Logic (Active LOW or Active HIGH)
#define RELAY_ACTIVE_LEVEL   LOW
#define RELAY_INACTIVE_LEVEL HIGH

// Touch Module Trigger Logic (TTP223 default output is HIGH when touched)
#define TOUCH_ACTIVE_LEVEL   HIGH
#define TOUCH_DEBOUNCE_MS    80

// -------------------------------------------------------------------------
// 2. TIMING CALIBRATION DEFAULTS
// -------------------------------------------------------------------------
#define DEFAULT_ON_HOURS          8.0f     // Default: 8.0 hours (configurable 7 to 8h)
#define MIN_ON_HOURS              0.5f     // Minimum 30 mins
#define MAX_ON_HOURS              24.0f    // Maximum 24 hours

#define DEFAULT_LOCKOUT_DAYS      6.0f     // Default: 6.0 days (configurable 5 to 7d)
#define MIN_LOCKOUT_DAYS          0.5f     // Minimum 12 hours
#define MAX_LOCKOUT_DAYS          30.0f    // Maximum 30 days

#define PERSIST_INTERVAL_MS       30000    // Commit state to Flash every 30s

// -------------------------------------------------------------------------
// 3. WI-FI ACCESS POINT & INTERNET STATION CONFIGURATION
// -------------------------------------------------------------------------
#define WIFI_AP_SSID              "CYBER-RELAY-HUB"
#define WIFI_AP_PASSWORD          "cyber8266"      // SoftAP password
#define WIFI_AP_CHANNEL           1
#define WIFI_AP_MAX_CONN          8
#define HTTP_SERVER_PORT          80

// Station mode to connect ESP8266 to Home/Internet Wi-Fi for Firebase Cloud Control
#define ENABLE_INTERNET_WIFI      true
#define WIFI_STA_SSID             "Home_WiFi_Name"  // Set your local router SSID
#define WIFI_STA_PASSWORD         "Home_WiFi_Pass"  // Set your local router Password

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
