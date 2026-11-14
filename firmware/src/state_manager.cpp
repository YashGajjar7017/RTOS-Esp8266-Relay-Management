#include "state_manager.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

StateManager stateManager;

StateManager::StateManager() : 
  _relayActive(false),
  _lastTouchReading(false),
  _lastDebounceTime(0),
  _touchTriggered(false),
  _lastPersistMillis(0),
  _uptimeSeconds(0)
{
  _settings.onHours = DEFAULT_ON_HOURS;
  _settings.lockoutDays = DEFAULT_LOCKOUT_DAYS;
  _state.mode = MODE_IDLE;
  _state.onRemainingSec = 0;
  _state.lockoutRemainingSec = 0;
  _state.totalOnSec = (uint32_t)(DEFAULT_ON_HOURS * 3600.0f);
  _state.totalLockoutSec = (uint32_t)(DEFAULT_LOCKOUT_DAYS * 86400.0f);
}

void StateManager::begin() {
  pinMode(PIN_RELAY, OUTPUT);
  applyRelayHardware(false);

  pinMode(PIN_TOUCH, INPUT);

  storageDB.loadSettings(_settings);
  
  PersistentState restoredState;
  if (storageDB.loadState(restoredState)) {
    _state = restoredState;
    Serial.printf("[STATE] Restored state from Flash: Mode=%s, OnRem=%u s, LockRem=%u s\n", 
      getModeString(), _state.onRemainingSec, _state.lockoutRemainingSec);

    if (_state.mode == MODE_RELAY_ON) {
      if (_state.onRemainingSec > 0) {
        applyRelayHardware(true);
      } else {
        transitionTo(MODE_LOCKOUT);
      }
    } else if (_state.mode == MODE_LOCKOUT) {
      applyRelayHardware(false);
      if (_state.lockoutRemainingSec == 0) {
        transitionTo(MODE_IDLE);
      }
    } else {
      applyRelayHardware(false);
    }
  } else {
    transitionTo(MODE_IDLE);
  }

  Serial.println(F("[STATE] State Manager initialized and running."));
}

void StateManager::applyRelayHardware(bool active) {
  _relayActive = active;
  digitalWrite(PIN_RELAY, active ? RELAY_ACTIVE_LEVEL : RELAY_INACTIVE_LEVEL);
  Serial.printf("[HW] Relay Pin %d set to %s\n", PIN_RELAY, active ? "ENERGIZED (ON)" : "OFF");
}

const char* StateManager::getModeString() const {
  switch (_state.mode) {
    case MODE_IDLE:     return "IDLE";
    case MODE_RELAY_ON: return "RELAY_ON";
    case MODE_LOCKOUT:  return "LOCKOUT";
    default:            return "UNKNOWN";
  }
}

void StateManager::transitionTo(SystemMode newMode) {
  _state.mode = newMode;

  switch (newMode) {
    case MODE_IDLE:
      applyRelayHardware(false);
      _state.onRemainingSec = 0;
      _state.lockoutRemainingSec = 0;
      Serial.println(F("[STATE] ---> Transitioned to IDLE (Touch module ready)."));
      break;

    case MODE_RELAY_ON:
      _state.totalOnSec = (uint32_t)(_settings.onHours * 3600.0f);
      _state.onRemainingSec = _state.totalOnSec;
      applyRelayHardware(true);
      storageDB.recordCycleTriggered();
      Serial.printf("[STATE] ---> Transitioned to RELAY_ON for %u seconds (%.1f h).\n", _state.totalOnSec, _settings.onHours);
      break;

    case MODE_LOCKOUT:
      applyRelayHardware(false);
      _state.totalLockoutSec = (uint32_t)(_settings.lockoutDays * 86400.0f);
      _state.lockoutRemainingSec = _state.totalLockoutSec;
      Serial.printf("[STATE] ---> Transitioned to LOCKOUT for %u seconds (%.1f days). Touch BLOCKED.\n", _state.totalLockoutSec, _settings.lockoutDays);
      break;
  }

  persistState();
}

void StateManager::persistState() {
  storageDB.saveState(_state);
}

void StateManager::handleTouchSensor() {
  int rawReading = digitalRead(PIN_TOUCH);
  bool isTouched = (rawReading == TOUCH_ACTIVE_LEVEL);

  if (isTouched != _lastTouchReading) {
    _lastDebounceTime = millis();
  }

  if ((millis() - _lastDebounceTime) > TOUCH_DEBOUNCE_MS) {
    if (isTouched && !_touchTriggered) {
      _touchTriggered = true;
      Serial.println(F("[TOUCH] Capacitive touch event detected!"));

      if (_state.mode == MODE_IDLE) {
        Serial.println(F("[TOUCH] System is IDLE. Triggering Relay Active Phase."));
        transitionTo(MODE_RELAY_ON);
      } else if (_state.mode == MODE_LOCKOUT) {
        Serial.printf("[TOUCH] BLOCKED! Lockout cooldown in progress (%u s remaining). Touch ignored.\n", _state.lockoutRemainingSec);
      } else if (_state.mode == MODE_RELAY_ON) {
        Serial.println(F("[TOUCH] Relay is already active in cycle. Touch ignored."));
      }
    } else if (!isTouched) {
      _touchTriggered = false;
    }
  }

  _lastTouchReading = isTouched;
}

void StateManager::tickOneSecond() {
  _uptimeSeconds++;

  // Feed monthly analytics runtime accumulator
  storageDB.recordStateSecond(_relayActive);

  if (_state.mode == MODE_RELAY_ON) {
    if (_state.onRemainingSec > 0) {
      _state.onRemainingSec--;
    }
    if (_state.onRemainingSec == 0) {
      Serial.println(F("[TIMER] Relay active time expired! Transitioning to Lockout Cooldown."));
      transitionTo(MODE_LOCKOUT);
    }
  } 
  else if (_state.mode == MODE_LOCKOUT) {
    if (_state.lockoutRemainingSec > 0) {
      _state.lockoutRemainingSec--;
    }
    if (_state.lockoutRemainingSec == 0) {
      Serial.println(F("[TIMER] Lockout period complete! Re-enabling Touch Sensor."));
      transitionTo(MODE_IDLE);
    }
  }

  if (millis() - _lastPersistMillis >= PERSIST_INTERVAL_MS) {
    _lastPersistMillis = millis();
    persistState();
  }
}

bool StateManager::triggerCycle(const char* source, const char* oneTimeToken) {
  if (_state.mode != MODE_IDLE) {
    Serial.printf("[CMD] Cannot trigger cycle from %s: system is %s\n", source, getModeString());
    return false;
  }

  // If one-time token was provided (from Mahi_Gajjar Level 2), consume and destroy it
  if (oneTimeToken && strlen(oneTimeToken) > 0) {
    if (!storageDB.consumeOneTimeToken(oneTimeToken)) {
      Serial.println(F("[AUTH-L2] REJECTED: Invalid or expired 1-time token."));
      return false;
    }
    Serial.println(F("[AUTH-L2] Valid 1-time token consumed. Session destroyed!"));
  }

  Serial.printf("[CMD] Cycle triggered by %s\n", source);
  transitionTo(MODE_RELAY_ON);
  return true;
}

bool StateManager::cancelCycle(const char* source) {
  Serial.printf("[CMD] Emergency cutoff initiated by %s\n", source);
  transitionTo(MODE_IDLE);
  return true;
}

bool StateManager::resetLockout(const char* source) {
  if (_state.mode != MODE_LOCKOUT) {
    return false;
  }
  Serial.printf("[CMD] Lockout bypassed by %s\n", source);
  transitionTo(MODE_IDLE);
  return true;
}

bool StateManager::extendActiveTimer(uint32_t additionalSeconds) {
  if (_state.mode != MODE_RELAY_ON) {
    Serial.println(F("[TIMER] Cannot extend: relay is not in RELAY_ON state."));
    return false;
  }
  _state.onRemainingSec += additionalSeconds;
  _state.totalOnSec += additionalSeconds;
  persistState();
  Serial.printf("[TIMER] Extended active relay by %u seconds. New remaining: %u s\n", additionalSeconds, _state.onRemainingSec);
  return true;
}

bool StateManager::updateTimers(float onHours, float lockoutDays) {
  if (onHours < MIN_ON_HOURS || onHours > MAX_ON_HOURS ||
      lockoutDays < MIN_LOCKOUT_DAYS || lockoutDays > MAX_LOCKOUT_DAYS) {
    return false;
  }

  _settings.onHours = onHours;
  _settings.lockoutDays = lockoutDays;
  storageDB.saveSettings(_settings);

  Serial.printf("[CONFIG] Updated timers: OnHours=%.1f h, LockoutDays=%.1f d\n", onHours, lockoutDays);
  return true;
}

String StateManager::getStatusJson(const char* currentUsername, const char* userRole) {
  StaticJsonDocument<2048> doc;

  doc["mode"] = getModeString();
  doc["relayActive"] = _relayActive;
  doc["touchActive"] = isTouchAllowed();
  doc["lockoutActive"] = isLockoutActive();
  doc["onRemainingSec"] = _state.onRemainingSec;
  doc["totalOnSec"] = _state.totalOnSec;
  doc["lockoutRemainingSec"] = _state.lockoutRemainingSec;
  doc["totalLockoutSec"] = _state.totalLockoutSec;
  doc["uptimeSeconds"] = _uptimeSeconds;
  doc["version"] = FIRMWARE_VERSION;

  // Real RF & Hardware diagnostics from ESP8266
  doc["wifiRssi"] = WiFi.RSSI();
  doc["wifiChannel"] = WiFi.channel();
  doc["softApStations"] = WiFi.softAPgetStationNum();
  doc["freeHeap"] = ESP.getFreeHeap();

  JsonObject cfg = doc.createNestedObject("config");
  cfg["onHours"] = _settings.onHours;
  cfg["lockoutDays"] = _settings.lockoutDays;

  if (currentUsername) {
    JsonObject cur = doc.createNestedObject("currentUser");
    cur["username"] = currentUsername;
    cur["role"] = userRole ? userRole : "guest_analytics";
  }

  // Monthly Analytics for Guest and all users
  const MonthlyAnalytics& an = storageDB.getAnalytics();
  JsonObject analyticsObj = doc.createNestedObject("analytics");
  analyticsObj["currentMonth"] = an.currentMonthYear;
  analyticsObj["totalOnSeconds"] = an.totalOnSeconds;
  analyticsObj["totalOffSeconds"] = an.totalOffSeconds;
  analyticsObj["totalCyclesCount"] = an.totalCyclesCount;
  float onHours = (float)an.totalOnSeconds / 3600.0f;
  float offHours = (float)an.totalOffSeconds / 3600.0f;
  analyticsObj["totalOnHours"] = serialized(String(onHours, 1));
  analyticsObj["totalOffHours"] = serialized(String(offHours, 1));

  // Level 2 Pending One-Time Request for Admin Popup
  if (storageDB.hasActiveOneTimeRequest()) {
    const OneTimeAccessRequest& req = storageDB.getOneTimeRequest();
    JsonObject pendingObj = doc.createNestedObject("pendingRequest");
    pendingObj["active"] = true;
    pendingObj["requestId"] = req.requestId;
    pendingObj["username"] = req.username;
    pendingObj["action"] = req.requestedAction;
  }

  // Users list
  JsonArray usersArr = doc.createNestedArray("users");
  int count = storageDB.getUserCount();
  for (int i = 0; i < count; i++) {
    UserRecord u;
    if (storageDB.getUser(i, u)) {
      JsonObject uObj = usersArr.createNestedObject();
      uObj["id"] = u.id;
      uObj["username"] = u.username;
      uObj["role"] = u.role;
      uObj["status"] = u.status;
      uObj["created"] = u.created;
    }
  }

  String output;
  serializeJson(doc, output);
  return output;
}
