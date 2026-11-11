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
/**
 * User Requirement: Single relay on "p1" & touch module.
 *
 * NOTE ON ESP8266 PIN MAPPING:
 * - On ESP8266 / NodeMCU:
 *   - Pin GPIO1 is TXD0 (UART Serial TX). If using GPIO1 as relay output,
 *     Serial.begin() must either not be used or pins remapped.
 *   - Alternatively, GPIO5 (NodeMCU Pin D1) is the standard relay pin.
 *
 * Here we define default pins and provide a macro flag to switch between
 * physical GPIO1 (TX) and NodeMCU D1 (GPIO5).
 */

#define USE_PHYSICAL_GPIO1_FOR_RELAY false // Set to true if physically wired to ESP-01 / TX (GPIO1)

#if USE_PHYSICAL_GPIO1_FOR_RELAY
  #define PIN_RELAY          1    // GPIO1 (TXD0)
#else
  #define PIN_RELAY          5    // GPIO5 (NodeMCU D1) - recommended for safety & serial logs
#endif

// Touch Sensor Pin (TTP223 Capacitive Touch Module)
// Active HIGH by default on touch detection
#define PIN_TOUCH            4    // GPIO4 (NodeMCU D2)

// Relay Logic (Active LOW or Active HIGH relay board)
// Most 5V optocoupler relay boards are Active LOW (0 = Relay ON, 1 = Relay OFF)
#define RELAY_ACTIVE_LEVEL   LOW
#define RELAY_INACTIVE_LEVEL HIGH

// Touch Module Trigger Logic (TTP223 default output is HIGH when touched)
#define TOUCH_ACTIVE_LEVEL   HIGH

// Debounce delay for touch sensor in milliseconds
#define TOUCH_DEBOUNCE_MS    80

// -------------------------------------------------------------------------
// 2. TIMING CALIBRATION DEFAULTS
// -------------------------------------------------------------------------
// Relay ON Duration (e.g. 7.0 to 8.0 hours)
#define DEFAULT_ON_HOURS          8.0f     // Default: 8.0 hours
#define MIN_ON_HOURS              0.5f     // Minimum 30 minutes
#define MAX_ON_HOURS              24.0f    // Maximum 24 hours

// Lockout Cooldown Duration (e.g. 5 to 7 days)
#define DEFAULT_LOCKOUT_DAYS      6.0f     // Default: 6.0 days
#define MIN_LOCKOUT_DAYS          0.5f     // Minimum 12 hours (for rapid testing)
#define MAX_LOCKOUT_DAYS          30.0f    // Maximum 30 days

// State Persistence Interval (Save remaining seconds to Flash NVRAM)
#define PERSIST_INTERVAL_MS       30000    // Every 30 seconds

// -------------------------------------------------------------------------
// 3. WI-FI & NETWORK ACCESS POINT CONFIGURATION
// -------------------------------------------------------------------------
// ESP8266 will broadcast its own SoftAP network
#define WIFI_AP_SSID              "CYBER-RELAY-HUB"
#define WIFI_AP_PASSWORD          "cyber8266"      // Min 8 chars (empty for open network)
#define WIFI_AP_CHANNEL           1
#define WIFI_AP_MAX_CONN          8
#define HTTP_SERVER_PORT          80

// Optional: Connect to your Home/Office Wi-Fi Router as Station (STA)
#define ENABLE_WIFI_STA           false
#define WIFI_STA_SSID             "Your_Home_WiFi"
#define WIFI_STA_PASSWORD         "Your_WiFi_Password"

// -------------------------------------------------------------------------
// 4. USER DATABASE CAPACITY
// -------------------------------------------------------------------------
#define MAX_USERS                 5        // Small inbuilt DB: 3 to 5 users
#define DEFAULT_ADMIN_USERNAME    "admin"
#define DEFAULT_ADMIN_PASSWORD    "admin123"

// System Version
#define FIRMWARE_VERSION          "2.4.0-RTOS"
