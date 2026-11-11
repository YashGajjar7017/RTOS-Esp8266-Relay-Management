#include "web_server_api.h"
#include "state_manager.h"
#include "storage_db.h"
#include "web_assets.h"
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

WebServerAPI webServerAPI;

// DNS port for captive portal
const byte DNS_PORT = 53;

WebServerAPI::WebServerAPI() : _server(HTTP_SERVER_PORT) {}

void WebServerAPI::begin() {
  // 1. Setup captive portal DNS Server (redirect all domain queries to ESP's SoftAP IP)
  IPAddress apIP = WiFi.softAPIP();
  _dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  _dnsServer.start(DNS_PORT, "*", apIP);

  // 2. Setup HTTP Server Endpoints
  setupRoutes();
  _server.begin();
  Serial.printf("[WEB] Web Server & Captive Portal listening on http://%s\n", apIP.toString().c_str());
}

void WebServerAPI::handleClient() {
  _dnsServer.processNextRequest();
  _server.handleClient();
}

void WebServerAPI::setupRoutes() {
  // CORS & Preflight handling
  _server.on("/api/status", HTTP_OPTIONS, [this]() {
    _server.sendHeader("Access-Control-Allow-Origin", "*");
    _server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    _server.sendHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
    _server.send(204);
  });
  _server.on("/api/config", HTTP_OPTIONS, [this]() {
    _server.sendHeader("Access-Control-Allow-Origin", "*");
    _server.sendHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
    _server.sendHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
    _server.send(204);
  });
  _server.on("/api/control", HTTP_OPTIONS, [this]() {
    _server.sendHeader("Access-Control-Allow-Origin", "*");
    _server.sendHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
    _server.sendHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
    _server.send(204);
  });
  _server.on("/api/auth/login", HTTP_OPTIONS, [this]() {
    _server.sendHeader("Access-Control-Allow-Origin", "*");
    _server.sendHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
    _server.sendHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
    _server.send(204);
  });
  _server.on("/api/auth/register", HTTP_OPTIONS, [this]() {
    _server.sendHeader("Access-Control-Allow-Origin", "*");
    _server.sendHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
    _server.sendHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
    _server.send(204);
  });

  // Main UI route
  _server.on("/", HTTP_GET, std::bind(&WebServerAPI::handleRoot, this));

  // Captive Portal common detector URLs
  _server.on("/generate_204", HTTP_GET, std::bind(&WebServerAPI::handleCaptivePortal, this)); // Android
  _server.on("/hotspot-detect.html", HTTP_GET, std::bind(&WebServerAPI::handleCaptivePortal, this)); // Apple
  _server.on("/ncsi.txt", HTTP_GET, std::bind(&WebServerAPI::handleCaptivePortal, this)); // Windows

  // REST API routes
  _server.on("/api/status", HTTP_GET, std::bind(&WebServerAPI::handleGetStatus, this));
  _server.on("/api/config", HTTP_POST, std::bind(&WebServerAPI::handlePostConfig, this));
  _server.on("/api/control", HTTP_POST, std::bind(&WebServerAPI::handlePostControl, this));
  _server.on("/api/auth/login", HTTP_POST, std::bind(&WebServerAPI::handleAuthLogin, this));
  _server.on("/api/auth/register", HTTP_POST, std::bind(&WebServerAPI::handleAuthRegister, this));
  _server.on("/api/users", HTTP_GET, [this]() {
    sendJsonResponse(200, storageDB.getUsersJson());
  });

  // Fallback 404 / Captive redirect
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
  // If index.html exists in LittleFS, serve that; otherwise serve embedded PROGMEM
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

  StaticJsonDocument<128> doc;
  deserializeJson(doc, _server.arg("plain"));
  const char* action = doc["action"] | "";

  if (strcmp(action, "trigger") == 0) {
    if (stateManager.triggerCycle("WEB_APP")) {
      sendJsonResponse(200, "{\"success\":true,\"message\":\"Relay cycle triggered\"}");
    } else {
      sendError(400, "Cannot trigger: system is not in IDLE state");
    }
  } 
  else if (strcmp(action, "cancel") == 0) {
    stateManager.cancelCycle("WEB_APP");
    sendJsonResponse(200, "{\"success\":true,\"message\":\"Cycle cancelled\"}");
  } 
  else if (strcmp(action, "reset_lockout") == 0) {
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

// Requirement 4 & 6: Inbuilt DB (3-5 users) & Admin Password Popup confirmation
void WebServerAPI::handleAuthRegister() {
  if (!_server.hasArg("plain")) {
    sendError(400, "Missing registration payload");
    return;
  }

  StaticJsonDocument<384> doc;
  DeserializationError err = deserializeJson(doc, _server.arg("plain"));
  if (err) {
    sendError(400, "Malformed registration JSON");
    return;
  }

  const char* username = doc["username"] | "";
  const char* password = doc["password"] | "";
  const char* role = doc["role"] | "operator";
  const char* adminPassword = doc["adminPassword"] | "";

  if (strlen(username) < 2 || strlen(password) < 4) {
    sendError(400, "Username must be >= 2 chars, password >= 4 chars");
    return;
  }

  if (strlen(adminPassword) == 0) {
    sendError(403, "Master Admin Password is required to approve new users");
    return;
  }

  String errorMsg;
  if (storageDB.registerUserWithAdminApproval(username, password, role, adminPassword, errorMsg)) {
    StaticJsonDocument<256> resp;
    resp["success"] = true;
    resp["message"] = "User successfully authorized by Admin and registered in database";
    resp["username"] = username;
    resp["role"] = role;
    String out;
    serializeJson(resp, out);
    sendJsonResponse(201, out);
  } else {
    sendError(403, errorMsg);
  }
}

bool WebServerAPI::extractSessionUser(String& username, String& role) {
  if (_server.hasHeader("Authorization")) {
    String auth = _server.header("Authorization");
    if (auth.startsWith("Bearer ")) {
      username = "admin";
      role = "admin";
      return true;
    }
  }
  return false;
}

void WebServerAPI::handleNotFound() {
  // If it's a captive portal browser probe, redirect to root
  String host = _server.hostHeader();
  if (host != WiFi.softAPIP().toString()) {
    handleCaptivePortal();
    return;
  }
  sendError(404, "Endpoint not found");
}
