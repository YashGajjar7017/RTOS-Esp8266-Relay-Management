#include "firebase_client.h"
#include "state_manager.h"
#include "storage_db.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

FirebaseClient firebaseClient;

FirebaseClient::FirebaseClient() :
  _wifiConnected(false),
  _cloudSynced(false),
  _lastSyncMillis(0),
  _lastWifiCheckMillis(0)
{}

void FirebaseClient::begin() {
  #if ENABLE_INTERNET_WIFI
    Serial.println(F("[FIREBASE] Initializing Firebase Realtime Cloud integration..."));
    WiFi.setAutoReconnect(true);
    checkWiFiConnection();
  #else
    Serial.println(F("[FIREBASE] Internet Wi-Fi station disabled. Running local SoftAP only."));
  #endif
}

void FirebaseClient::checkWiFiConnection() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!_wifiConnected) {
      _wifiConnected = true;
      Serial.printf("[FIREBASE] Connected to Internet Wi-Fi! Local IP: %s\n", WiFi.localIP().toString().c_str());
      stateManager.addSystemLog("WIFI", (String("STA Connected! IP: ") + WiFi.localIP().toString()).c_str());
    }
  } else {
    if (_wifiConnected) {
      stateManager.addSystemLog("WIFI_DISC", "STA Wi-Fi disconnected");
    }
    _wifiConnected = false;
  }
}

void FirebaseClient::update() {
  #if !ENABLE_INTERNET_WIFI
    return;
  #endif

  unsigned long currentMillis = millis();

  if (currentMillis - _lastWifiCheckMillis >= 5000) {
    _lastWifiCheckMillis = currentMillis;
    checkWiFiConnection();
  }

  if (!_wifiConnected) return;

  if (currentMillis - _lastSyncMillis >= FIREBASE_SYNC_INTERVAL_MS) {
    _lastSyncMillis = currentMillis;
    syncToFirebase();
    pollFirebaseCommands();
  }
}

void FirebaseClient::syncToFirebase() {
  WiFiClientSecure client;
  client.setInsecure(); // Allows fast HTTPS without maintaining dynamic root certificates
  client.setBufferSizes(1024, 512); // CRITICAL ESP8266 RAM optimization to prevent TLS out-of-memory crash
  
  HTTPClient http;
  http.setTimeout(4000);

  String url = String("https://") + FIREBASE_HOST + "/esp8266_relay/telemetry.json";
  if (strlen(FIREBASE_AUTH) > 0 && strcmp(FIREBASE_AUTH, "YOUR_FIREBASE_DATABASE_SECRET") != 0) {
    url += "?auth=" + String(FIREBASE_AUTH);
  }

  if (http.begin(client, url)) {
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<1024> doc;
    doc["mode"] = stateManager.getModeString();
    doc["sequencePhase"] = (stateManager.getMode() == MODE_RELAY_ON) ? "CHARGING" : ((stateManager.getMode() == MODE_LOCKOUT) ? "COOLDOWN" : "READY");
    doc["chargingAllowed"] = stateManager.isChargingAllowed();
    doc["relayActive"] = stateManager.isRelayActive();
    doc["touchActive"] = stateManager.isTouchAllowed();
    doc["lockoutActive"] = stateManager.isLockoutActive();
    doc["onRemainingSec"] = stateManager.getOnRemainingSec();
    doc["totalOnSec"] = stateManager.getTotalOnSec();
    doc["lockoutRemainingSec"] = stateManager.getLockoutRemainingSec();
    doc["totalLockoutSec"] = stateManager.getTotalLockoutSec();
    doc["uptimeSeconds"] = stateManager.getUptime();
    doc["lastSync"] = millis() / 1000;

    JsonObject cfg = doc.createNestedObject("config");
    cfg["onHours"] = stateManager.getOnHours();
    cfg["lockoutDays"] = stateManager.getLockoutDays();

    // Monthly Analytics
    const MonthlyAnalytics& an = storageDB.getAnalytics();
    JsonObject anObj = doc.createNestedObject("analytics");
    anObj["month"] = an.currentMonthYear;
    anObj["totalOnHours"] = serialized(String((float)an.totalOnSeconds / 3600.0f, 1));
    anObj["totalOffHours"] = serialized(String((float)an.totalOffSeconds / 3600.0f, 1));
    anObj["totalCycles"] = an.totalCyclesCount;

    // Level 2 Pending Request for Worldwide Admin Popup
    if (storageDB.hasActiveOneTimeRequest()) {
      const OneTimeAccessRequest& req = storageDB.getOneTimeRequest();
      JsonObject pObj = doc.createNestedObject("pendingRequest");
      pObj["active"] = true;
      pObj["requestId"] = req.requestId;
      pObj["username"] = req.username;
      pObj["action"] = req.requestedAction;
    } else {
      doc["pendingRequest"]["active"] = false;
    }

    // Handshake ACK & System Logs for Cloud View
    JsonObject ackObj = doc.createNestedObject("ack");
    ackObj["verified"] = stateManager.isAckVerified();
    ackObj["bootReason"] = ESP.getResetReason();
    ackObj["wifiConnected"] = (WiFi.status() == WL_CONNECTED);
    ackObj["softApSsid"] = WIFI_AP_SSID;
    ackObj["softApIp"] = WiFi.softAPIP().toString();
    ackObj["staIp"] = (WiFi.status() == WL_CONNECTED) ? WiFi.localIP().toString() : "0.0.0.0";
    ackObj["buzzerChimes"] = 3;

    JsonArray logsArr = doc.createNestedArray("systemLogs");
    int logCount = stateManager.getSystemLogCount();
    for (int i = 0; i < logCount; i++) {
      const SystemLogEntry* entry = stateManager.getSystemLog(i);
      if (entry) {
        JsonObject lObj = logsArr.createNestedObject();
        lObj["uptime"] = entry->uptime;
        lObj["tag"] = entry->tag;
        lObj["msg"] = entry->msg;
      }
    }

    String payload;
    serializeJson(doc, payload);

    int httpCode = http.PUT(payload);
    if (httpCode == 200) {
      if (!_cloudSynced && !stateManager.isAckVerified()) {
        stateManager.setAckVerified(true);
        stateManager.addSystemLog("ACK", "Cloud pipeline verified: AP + WiFi + Firebase OK -> 3-Beep Buzzer Sounded.");
        stateManager.soundAckBuzzer();
      }
      if (!_cloudSynced) {
        stateManager.addSystemLog("FIREBASE_OK", "Cloud Synced OK (HTTP 200)");
      }
      _cloudSynced = true;
    } else {
      _cloudSynced = false;
      static int lastLoggedCode = 0;
      if (httpCode != lastLoggedCode) {
        lastLoggedCode = httpCode;
        if (httpCode == 404) {
          stateManager.addSystemLog("FIREBASE_ERR", "404 Not Found: Database not created in Firebase Console yet!");
        } else if (httpCode == 401 || httpCode == 403) {
          stateManager.addSystemLog("FIREBASE_ERR", "401/403 Denied: Check RTDB Security Rules / Secret");
        } else if (httpCode < 0) {
          stateManager.addSystemLog("FIREBASE_ERR", (String("TLS/Net err: ") + http.errorToString(httpCode)).c_str());
        } else {
          stateManager.addSystemLog("FIREBASE_ERR", (String("HTTP error ") + httpCode).c_str());
        }
      }
    }
    http.end();
  }
}

void FirebaseClient::pollFirebaseCommands() {
  WiFiClientSecure client;
  client.setInsecure();
  client.setBufferSizes(1024, 512); // CRITICAL ESP8266 RAM optimization
  
  HTTPClient http;
  http.setTimeout(4000);

  String url = String("https://") + FIREBASE_HOST + "/esp8266_relay/commands.json";
  if (strlen(FIREBASE_AUTH) > 0 && strcmp(FIREBASE_AUTH, "YOUR_FIREBASE_DATABASE_SECRET") != 0) {
    url += "?auth=" + String(FIREBASE_AUTH);
  }

  if (http.begin(client, url)) {
    int httpCode = http.GET();
    if (httpCode == 200) {
      String resp = http.getString();
      if (resp != "null" && resp.length() > 5) {
        StaticJsonDocument<512> doc;
        DeserializationError err = deserializeJson(doc, resp);
        if (!err && doc.containsKey("action")) {
          const char* action = doc["action"] | "";
          const char* oneTimeToken = doc["oneTimeToken"] | "";
          const char* requestedBy = doc["requestedBy"] | "FIREBASE_CLOUD";

          Serial.printf("[FIREBASE] Received Cloud Command: %s (from %s)\n", action, requestedBy);

          uint8_t relayId = doc["relayId"] | 1;

          if (strcmp(action, "TRIGGER") == 0) {
            stateManager.triggerRelay(relayId, requestedBy, oneTimeToken);
          } else if (strcmp(action, "CANCEL") == 0) {
            stateManager.cancelRelay(relayId, requestedBy);
          } else if (strcmp(action, "TOGGLE") == 0) {
            stateManager.toggleRelay(relayId, requestedBy);
          } else if (strcmp(action, "RESET_LOCKOUT") == 0) {
            stateManager.resetRelayLockout(relayId, requestedBy);
          } else if (strcmp(action, "EXTEND") == 0) {
            int extraMinutes = doc["minutes"] | (doc["seconds"] | 0) / 60;
            if (extraMinutes <= 0) extraMinutes = 15;
            stateManager.extendRelayTimer(relayId, (uint32_t)extraMinutes * 60);
          } else if (strcmp(action, "SET_USER_STATUS") == 0) {
            const char* target = doc["targetUsername"] | "";
            const char* status = doc["status"] | "disabled";
            const char* adminPass = doc["adminPassword"] | "";
            String err;
            storageDB.setUserStatus(target, status, adminPass, err);
          } else if (strcmp(action, "RESET_PASSWORD") == 0) {
            const char* target = doc["targetUsername"] | (doc["targetUser"] | "");
            const char* authKey = doc["authKey"] | "";
            const char* newPassword = doc["newPassword"] | "";
            String err;
            storageDB.resetUserPassword(target, authKey, newPassword, err);
          } else if (strcmp(action, "CHECK_ACK") == 0 || strcmp(action, "SOUND_ACK") == 0) {
            stateManager.soundAckBuzzer();
            stateManager.setAckVerified(true);
            stateManager.addSystemLog("ACK_CLOUD", "Remote cloud verified ACK handshake. 3-beep buzzer fired.");
          } else if (strcmp(action, "UPDATE_CONFIG") == 0) {
            float onHours = doc["onHours"] | -1.0f;
            float lockoutDays = doc["lockoutDays"] | -1.0f;
            if (onHours > 0 && lockoutDays > 0) {
              stateManager.updateTimers(onHours, lockoutDays);
            }
          }

          // Clear executed command in Firebase
          http.end();
          if (http.begin(client, url)) {
            http.PUT("null");
          }
        }
      }
    }
    http.end();
  }
}
