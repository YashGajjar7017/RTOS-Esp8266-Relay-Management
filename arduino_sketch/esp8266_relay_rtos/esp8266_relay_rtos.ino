#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "config.h"
#include "storage_db.h"
#include "state_manager.h"
#include "web_server_api.h"
#include "rtos_tasks.h"

/**
 * =========================================================================
 *                  TASK HANDLERS FOR RTOS SCHEDULER
 * =========================================================================
 */

// Task 1: High frequency capacitive touch sensor polling & debouncing (15 ms)
void taskTouchSensor() {
  stateManager.handleTouchSensor();
}

// Task 2: 1 Hz Clock Tick & Countdown decrements (1000 ms)
void taskCountdownTick() {
  stateManager.tickOneSecond();
}

// Task 3: Network Web Server & Captive Portal requests (10 ms)
void taskNetworkServer() {
  webServerAPI.handleClient();
}

// Task 4: Background diagnostic logger (60000 ms)
void taskDiagnosticLog() {
  Serial.printf("[DIAG] Heap: %u bytes | Mode: %s | Relay: %d | Up: %u s\n",
    ESP.getFreeHeap(),
    stateManager.getModeString(),
    stateManager.isRelayActive(),
    stateManager.getUptime()
  );
}

/**
 * =========================================================================
 *                       SETUP & INITIALIZATION
 * =========================================================================
 */
void setup() {
  #if !USE_PHYSICAL_GPIO1_FOR_RELAY
    Serial.begin(115200);
    delay(200);
    Serial.println();
    Serial.println(F("=================================================="));
    Serial.println(F("   CYBER-RELAY RTOS // ESP8266 MANAGEMENT HUB    "));
    Serial.println(F("=================================================="));
  #endif

  // 1. Initialize Storage & Inbuilt User Database
  storageDB.begin();

  // 2. Initialize Hardware Relay & Touch State Manager
  stateManager.begin();

  // 3. Configure Wi-Fi SoftAP Mode (and optional STA connection)
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(
    IPAddress(192, 168, 4, 1),
    IPAddress(192, 168, 4, 1),
    IPAddress(255, 255, 255, 0)
  );

  bool apStarted = WiFi.softAP(WIFI_AP_SSID, strlen(WIFI_AP_PASSWORD) ? WIFI_AP_PASSWORD : NULL, WIFI_AP_CHANNEL, false, WIFI_AP_MAX_CONN);
  if (apStarted) {
    Serial.printf("[WIFI] Access Point '%s' initialized.\n", WIFI_AP_SSID);
    Serial.printf("[WIFI] Connect to Wi-Fi and browse to: http://%s\n", WiFi.softAPIP().toString().c_str());
  } else {
    Serial.println(F("[WIFI] ERROR: SoftAP failed to start!"));
  }

  #if ENABLE_WIFI_STA
    WiFi.begin(WIFI_STA_SSID, WIFI_STA_PASSWORD);
    Serial.printf("[WIFI] Connecting to Home Wi-Fi: %s\n", WIFI_STA_SSID);
  #endif

  // 4. Start Web Server & Captive Portal
  webServerAPI.begin();

  // 5. Register RTOS Priority Tasks
  rtosScheduler.registerTask("TouchSensor", 15, taskTouchSensor);
  rtosScheduler.registerTask("NetworkServer", 10, taskNetworkServer);
  rtosScheduler.registerTask("CountdownTick", 1000, taskCountdownTick);
  rtosScheduler.registerTask("Diagnostics", 60000, taskDiagnosticLog);
  rtosScheduler.begin();

  Serial.println(F("[SYSTEM] Setup completed. System ready for operation."));
}

/**
 * =========================================================================
 *                         MAIN RTOS RUNNER LOOP
 * =========================================================================
 */
void loop() {
  rtosScheduler.run();
}
