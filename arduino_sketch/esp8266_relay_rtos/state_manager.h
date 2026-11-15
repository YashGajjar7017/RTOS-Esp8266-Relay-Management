#pragma once
#include <Arduino.h>
#include "config.h"
#include "storage_db.h"

#ifndef MAX_RELAYS
#define MAX_RELAYS 4
#endif

/**
 * Multi-Relay Channel Configuration & Runtime State
 * Allows dynamic addition of separate relays with dedicated
 * ESP8266 GPIO pin bindings, independent countdown timers, and states.
 */
struct RelayChannel {
  uint8_t id;             // 1, 2, 3, 4
  char name[24];          // e.g. "Relay 1 (Main)", "Water Pump"
  int8_t pin;             // ESP8266 GPIO pin (e.g. 5, 14, 12, 13, 16)
  uint8_t activeLevel;    // LOW (0) or HIGH (1)
  float onHours;          // Separate ON duration (e.g. 8.0h)
  float lockoutDays;      // Separate Lockout/OFF duration (e.g. 8.0d)
  bool enabled;           // Active channel flag

  // Independent Runtime State
  SystemMode mode;        // MODE_IDLE, MODE_RELAY_ON, MODE_LOCKOUT
  uint32_t onRemainingSec;
  uint32_t lockoutRemainingSec;
  uint32_t totalOnSec;
  uint32_t totalLockoutSec;
  bool relayActive;
};

class StateManager {
public:
  StateManager();

  void begin();
  void tickOneSecond();
  void handleTouchSensor();

  // Backward Compatible Primary Relay (Channel 1) Controls
  bool triggerCycle(const char* source = "MANUAL", const char* oneTimeToken = nullptr);
  bool cancelCycle(const char* source = "MANUAL");
  bool resetLockout(const char* source = "ADMIN");
  bool extendActiveTimer(uint32_t additionalSeconds);
  bool updateTimers(float onHours, float lockoutDays);

  // Dynamic Multi-Relay Controls
  int getRelayCount() const { return _relayCount; }
  const RelayChannel* getRelay(int index) const;
  RelayChannel* getRelayById(uint8_t id);
  bool triggerRelay(uint8_t relayId, const char* source = "MANUAL", const char* oneTimeToken = nullptr);
  bool cancelRelay(uint8_t relayId, const char* source = "MANUAL");
  bool toggleRelay(uint8_t relayId, const char* source = "MANUAL");
  bool resetRelayLockout(uint8_t relayId, const char* source = "ADMIN");
  bool extendRelayTimer(uint8_t relayId, uint32_t additionalSeconds);

  // Dynamic Relay Addition & Configuration in Settings
  bool addRelay(const char* name, int8_t pin, uint8_t activeLevel, float onHours, float lockoutDays, String& errorMsg);
  bool updateRelay(uint8_t relayId, const char* name, int8_t pin, uint8_t activeLevel, float onHours, float lockoutDays, String& errorMsg);
  bool deleteRelay(uint8_t relayId, String& errorMsg);

  // Hardware Buzzer Feedback on NodeMCU D8 (GPIO15)
  void soundBuzzer(bool turningOn);

  // Status Getters (Primary Channel 1)
  SystemMode getMode() const { return _relays[0].mode; }
  const char* getModeString() const;
  const char* getRelayModeString(uint8_t relayId) const;
  bool isRelayActive() const { return _relays[0].relayActive; }
  bool isTouchAllowed() const { return _relays[0].mode == MODE_IDLE; }
  bool isLockoutActive() const { return _relays[0].mode == MODE_LOCKOUT; }
  
  uint32_t getOnRemainingSec() const { return _relays[0].onRemainingSec; }
  uint32_t getTotalOnSec() const { return _relays[0].totalOnSec; }
  uint32_t getLockoutRemainingSec() const { return _relays[0].lockoutRemainingSec; }
  uint32_t getTotalLockoutSec() const { return _relays[0].totalLockoutSec; }
  
  float getOnHours() const { return _relays[0].onHours; }
  float getLockoutDays() const { return _relays[0].lockoutDays; }
  uint32_t getUptime() const { return _uptimeSeconds; }

  // JSON summary for API and Firebase
  String getStatusJson(const char* currentUsername = nullptr, const char* userRole = nullptr);

private:
  RelayChannel _relays[MAX_RELAYS];
  int _relayCount;

  // Touch debouncing
  bool _lastTouchReading;
  unsigned long _lastDebounceTime;
  bool _touchTriggered;

  // Persistence timing
  unsigned long _lastPersistMillis;
  uint32_t _uptimeSeconds;

  void applyRelayHardware(int index, bool active);
  void transitionRelayTo(int index, SystemMode newMode);
  void persistAllRelays();
  bool loadAllRelays();
};

extern StateManager stateManager;
