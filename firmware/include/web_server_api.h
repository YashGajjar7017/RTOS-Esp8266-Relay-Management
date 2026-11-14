#pragma once
#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include "config.h"

class WebServerAPI {
public:
  WebServerAPI();
  void begin();
  void handleClient();

private:
  ESP8266WebServer _server;
  DNSServer _dnsServer;

  void setupRoutes();
  void sendJsonResponse(int code, const String& json);
  void sendError(int code, const String& message);

  // Auth helper
  bool extractSessionUser(String& username, String& role);

  // Route Handlers
  void handleRoot();
  void handleCaptivePortal();
  void handleGetStatus();
  void handlePostConfig();
  void handlePostControl();
  void handleAuthLogin();
  void handleAuthRegister();
  void handleAuthResetPassword();
  void handleDeleteUser();
  void handleNotFound();
};

extern WebServerAPI webServerAPI;

