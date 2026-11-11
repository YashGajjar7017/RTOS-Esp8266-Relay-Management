#include "web_server_api.h"
#include "state_manager.h"
#include "storage_db.h"
#include "web_assets.h"
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

WebServerAPI webServerAPI;
const byte DNS_PORT = 53;

WebServerAPI::WebServerAPI() : _server(HTTP_SERVER_PORT) {}

void WebServerAPI::begin() {
  IPAddress apIP = WiFi.softAPIP();
  _dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  _dnsServer.start(DNS_PORT, "*", apIP);

  setupRoutes();
  _server.begin();
  Serial.printf("[WEB] Web Server & Captive Portal listening on http://%s\n", apIP.toString().c_str());
}

void WebServerAPI::handleClient() {
  _dnsServer.processNextRequest();
  _server.handleClient();
}

void WebServerAPI::setupRoutes() {
  auto sendCORS = [this]() {
    _server.sendHeader("Access-Control-Allow-Origin", "*");
    _server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS, DELETE");
    _server.sendHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
    _server.send(204);
  };

  _server.on("/api/status", HTTP_OPTIONS, sendCORS);
  _server.on("/api/config", HTTP_OPTIONS, sendCORS);
  _server.on("/api/control", HTTP_OPTIONS, sendCORS);
  _server.on("/api/auth/login", HTTP_OPTIONS, sendCORS);
  _server.on("/api/auth/request_grant", HTTP_OPTIONS, sendCORS);
  _server.on("/api/auth/approve_grant", HTTP_OPTIONS, sendCORS);
  _server.on("/api/auth/reject_grant", HTTP_OPTIONS, sendCORS);
  _server.on("/api/analytics", HTTP_OPTIONS, sendCORS);

  // Main UI route
  _server.on("/", HTTP_GET, std::bind(&WebServerAPI::handleRoot, this));

  // Captive Portal detectors
  _server.on("/generate_204", HTTP_GET, std::bind(&WebServerAPI::handleCaptivePortal, this));
  _server.on("/hotspot-detect.html", HTTP_GET, std::bind(&WebServerAPI::handleCaptivePortal, this));
  _server.on("/ncsi.txt", HTTP_GET, std::bind(&WebServerAPI::handleCaptivePortal, this));

  // REST API routes
  _server.on("/api/status", HTTP_GET, std::bind(&WebServerAPI::handleGetStatus, this));
  _server.on("/api/config", HTTP_POST, std::bind(&WebServerAPI::handlePostConfig, this));
  _server.on("/api/control", HTTP_POST, std::bind(&WebServerAPI::handlePostControl, this));
  _server.on("/api/auth/login", HTTP_POST, std::bind(&WebServerAPI::handleAuthLogin, this));
  
  // Level 2 One-Time Grant Endpoints
  _server.on("/api/auth/request_grant", HTTP_POST, [this]() {
    if (!_server.hasArg("plain")) {
      sendError(400, "Missing payload");
      return;
    }
    StaticJsonDocument<256> doc;
    deserializeJson(doc, _server.arg("plain"));
    const char* user = doc["username"] | "";
    const char* action = doc["action"] | "TRIGGER_RELAY";

    String reqId, err;
    if (storageDB.createOneTimeRequest(user, action, reqId, err)) {
      StaticJsonDocument<128> resp;
      resp["success"] = true;
      resp["requestId"] = reqId;
      resp["message"] = "One-time access request submitted. Waiting for Admin confirmation popup.";
      String out;
      serializeJson(resp, out);
      sendJsonResponse(200, out);
    } else {
      sendError(400, err);
    }
  });

  _server.on("/api/auth/approve_grant", HTTP_POST, [this]() {
    if (!_server.hasArg("plain")) {
      sendError(400, "Missing payload");
      return;
    }
    StaticJsonDocument<256> doc;
    deserializeJson(doc, _server.arg("plain"));
    const char* adminPassword = doc["adminPassword"] | "";

    String token, err;
    if (storageDB.approveOneTimeRequest(adminPassword, token, err)) {
      StaticJsonDocument<256> resp;
      resp["success"] = true;
      resp["oneTimeToken"] = token;
      resp["message"] = "Admin authorized 1-time grant successfully";
      String out;
      serializeJson(resp, out);
      sendJsonResponse(200, out);
    } else {
      sendError(403, err);
    }
  });

  _server.on("/api/auth/reject_grant", HTTP_POST, [this]() {
    storageDB.rejectOneTimeRequest();
    sendJsonResponse(200, "{\"success\":true,\"message\":\"Request dismissed\"}");
  });

  // Level 3 Monthly Analytics Endpoint
  _server.on("/api/analytics", HTTP_GET, [this]() {
    sendJsonResponse(200, storageDB.getAnalyticsJson());
  });

  _server.on("/api/users", HTTP_GET, [this]() {
    sendJsonResponse(200, storageDB.getUsersJson());
  });

  _server.onNotFound(std::bind(&WebServerAPI::handleNotFound, this));
}

void WebServerAPI::sendJsonResponse(int code, const String& json) {
  _server.sendHeader("Access-Control-Allow-Origin", "*");
  _server.sendHeader("Content-Type", "application/json");
  _server.send(code, "application/json", json);
}

void WebServerAPI::sendError(int code, const String& message) {
  StaticJsonDocument<128> doc;
  doc["error"] = true;
  doc["message"] = message;
  String output;
  serializeJson(doc, output);
  sendJsonResponse(code, output);
}

void WebServerAPI::handleRoot() {
  if (LittleFS.exists("/index.html")) {
    File f = LittleFS.open("/index.html", "r");
    _server.streamFile(f, "text/html");
    f.close();
  } else {
    _server.sendHeader("Content-Type", "text/html; charset=utf-8");
    _server.send_P(200, "text/html", PAGE_INDEX);
  }
}

void WebServerAPI::handleCaptivePortal() {
  _server.sendHeader("Location", String("http://") + WiFi.softAPIP().toString() + "/", true);
  _server.send(302, "text/plain", "");
}

void WebServerAPI::handleGetStatus() {
  String username, role;
  bool isAuth = extractSessionUser(username, role);
  String json = stateManager.getStatusJson(isAuth ? username.c_str() : nullptr, isAuth ? role.c_str() : nullptr);
  sendJsonResponse(200, json);
}

void WebServerAPI::handlePostConfig() {
  if (!_server.hasArg("plain")) {
    sendError(400, "Missing JSON payload");
    return;
  }

  StaticJsonDocument<256> doc;
  DeserializationError err = deserializeJson(doc, _server.arg("plain"));
  if (err) {
    sendError(400, "Invalid JSON payload");
    return;
  }

  // Check role: Guest is strictly blocked
  const char* callerRole = doc["userRole"] | "";
  if (strcmp(callerRole, "guest_analytics") == 0) {
    sendError(403, "Access Denied: Guest accounts are read-only.");
    return;
  }

  float onHours = doc["onHours"] | -1.0f;
  float lockoutDays = doc["lockoutDays"] | -1.0f;

  if (onHours < MIN_ON_HOURS || onHours > MAX_ON_HOURS ||
      lockoutDays < MIN_LOCKOUT_DAYS || lockoutDays > MAX_LOCKOUT_DAYS) {
    sendError(400, "Values out of allowed range");
    return;
  }

  if (stateManager.updateTimers(onHours, lockoutDays)) {
    StaticJsonDocument<128> resp;
    resp["success"] = true;
    resp["message"] = "Timing configuration updated and saved";
    String out;
    serializeJson(resp, out);
    sendJsonResponse(200, out);
  } else {
    sendError(500, "Failed to persist configuration");
  }
}

void WebServerAPI::handlePostControl() {
  if (!_server.hasArg("plain")) {
    sendError(400, "Missing command payload");
    return;
  }

  StaticJsonDocument<256> doc;
  deserializeJson(doc, _server.arg("plain"));
  const char* action = doc["action"] | "";
  const char* userRole = doc["userRole"] | "";
  const char* oneTimeToken = doc["oneTimeToken"] | "";

  // 3-Tier Rule Check:
  // Tier 3: Guest is strictly prohibited from executing controls
  if (strcmp(userRole, "guest_analytics") == 0) {
    sendError(403, "Access Denied: Guest account cannot trigger hardware controls.");
    return;
  }

  // Tier 2: Mahi_Gajjar requires 1-time token
  if (strcmp(userRole, "guarded_operator") == 0) {
    if (!oneTimeToken || strlen(oneTimeToken) == 0) {
      sendError(403, "One-Time Admin Authorization required for this operation.");
      return;
    }
  }

  if (strcmp(action, "trigger") == 0) {
    if (stateManager.triggerCycle("APP", oneTimeToken)) {
      sendJsonResponse(200, "{\"success\":true,\"message\":\"Relay cycle triggered\"}");
    } else {
      sendError(400, "Cannot trigger: system is not in IDLE state, or 1-time token invalid.");
    }
  } 
  else if (strcmp(action, "cancel") == 0) {
    stateManager.cancelCycle("APP");
    sendJsonResponse(200, "{\"success\":true,\"message\":\"Cycle cancelled\"}");
  } 
  else if (strcmp(action, "reset_lockout") == 0) {
    // Only Yash_Gajjar (admin) can bypass lockout
    if (strcmp(userRole, "admin") != 0) {
      sendError(403, "Only Super Administrator (Yash_Gajjar) can bypass lockout.");
      return;
    }

    if (stateManager.resetLockout("ADMIN_APP")) {
      sendJsonResponse(200, "{\"success\":true,\"message\":\"Lockout bypassed by Admin\"}");
    } else {
      sendError(400, "Lockout reset failed: system is not in lockout");
    }
  } 
  else {
    sendError(400, "Unknown action");
  }
}

void WebServerAPI::handleAuthLogin() {
  if (!_server.hasArg("plain")) {
    sendError(400, "Missing login credentials");
    return;
  }

  StaticJsonDocument<256> doc;
  deserializeJson(doc, _server.arg("plain"));
  const char* username = doc["username"] | "";
  const char* password = doc["password"] | "";

  UserRecord matchedUser;
  if (storageDB.verifyCredentials(username, password, matchedUser)) {
    StaticJsonDocument<256> resp;
    resp["success"] = true;
    resp["username"] = matchedUser.username;
    resp["role"] = matchedUser.role;
    resp["token"] = String("esp-auth-") + matchedUser.id + "-" + millis();
    String out;
    serializeJson(resp, out);
    sendJsonResponse(200, out);
  } else {
    sendError(401, "Invalid username or password");
  }
}

bool WebServerAPI::extractSessionUser(String& username, String& role) {
  if (_server.hasHeader("Authorization")) {
    String auth = _server.header("Authorization");
    if (auth.startsWith("Bearer ")) {
      username = "Yash_Gajjar";
      role = "admin";
      return true;
    }
  }
  return false;
}

void WebServerAPI::handleNotFound() {
  String host = _server.hostHeader();
  if (host != WiFi.softAPIP().toString()) {
    handleCaptivePortal();
    return;
  }
  sendError(404, "Endpoint not found");
}
