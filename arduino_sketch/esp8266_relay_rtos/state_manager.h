#pragma once
#include <Arduino.h>
#include "config.h"
#include "storage_db.h"

class StateManager {
public:
  StateManager();

  void begin();
  void tickOneSecond();
  void handleTouchSensor();

  // Control Actions
  bool triggerCycle(const char* source = "MANUAL");
  bool cancelCycle(const char* source = "MANUAL");
  bool resetLockout(const char* source = "ADMIN");

  // Configuration updates
  bool updateTimers(float onHours, float lockoutDays);

  // Status Getters
  SystemMode getMode() const { return _state.mode; }
  const char* getModeString() const;
  bool isRelayActive() const { return _relayActive; }
  bool isTouchAllowed() const { return _state.mode == MODE_IDLE; }
  bool isLockoutActive() const { return _state.mode == MODE_LOCKOUT; }
  
  uint32_t getOnRemainingSec() const { return _state.onRemainingSec; }
  uint32_t getTotalOnSec() const { return _state.totalOnSec; }
  uint32_t getLockoutRemainingSec() const { return _state.lockoutRemainingSec; }
  uint32_t getTotalLockoutSec() const { return _state.totalLockoutSec; }
  
  float getOnHours() const { return _settings.onHours; }
  float getLockoutDays() const { return _settings.lockoutDays; }
  uint32_t getUptime() const { return _uptimeSeconds; }

  // JSON summary for API
  String getStatusJson(const char* currentUsername = nullptr, const char* userRole = nullptr);

private:
  SystemSettings _settings;
  PersistentState _state;
  bool _relayActive;

  // Touch debouncing
  bool _lastTouchReading;
  unsigned long _lastDebounceTime;
  bool _touchTriggered;

  // Persistence timing
  unsigned long _lastPersistMillis;
  uint32_t _uptimeSeconds;

  void applyRelayHardware(bool active);
  void transitionTo(SystemMode newMode);
  void persistState();
};

extern StateManager stateManager;
