#pragma once
#include <Arduino.h>
#include "config.h"

class FirebaseClient {
public:
  FirebaseClient();
  void begin();
  void update(); // Called by RTOS task or loop

  bool isConnected() const { return _wifiConnected && _cloudSynced; }
  unsigned long getLastSyncMillis() const { return _lastSyncMillis; }

private:
  bool _wifiConnected;
  bool _cloudSynced;
  unsigned long _lastSyncMillis;
  unsigned long _lastWifiCheckMillis;

  void checkWiFiConnection();
  void syncToFirebase();
  void pollFirebaseCommands();
};

extern FirebaseClient firebaseClient;
