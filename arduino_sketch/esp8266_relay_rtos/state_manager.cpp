#include "state_manager.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <LittleFS.h>

StateManager stateManager;

StateManager::StateManager() : 
  _relayCount(1),
  _lastTouchReading(false),
  _lastDebounceTime(0),
  _touchTriggered(false),
  _lastPersistMillis(0),
  _uptimeSeconds(0),
  _logCount(0),
  _ackVerified(false)
{
  memset(_relays, 0, sizeof(_relays));

  // Initialize Primary Relay 1
  _relays[0].id = 1;
  strncpy(_relays[0].name, "Relay 1 (Main)", sizeof(_relays[0].name) - 1);
  _relays[0].pin = PIN_RELAY;
  _relays[0].activeLevel = RELAY_ACTIVE_LEVEL;
  _relays[0].onHours = DEFAULT_ON_HOURS;
  _relays[0].lockoutDays = DEFAULT_LOCKOUT_DAYS;
  _relays[0].enabled = true;
  _relays[0].mode = MODE_IDLE;
  _relays[0].onRemainingSec = 0;
  _relays[0].lockoutRemainingSec = 0;
  _relays[0].totalOnSec = (uint32_t)(DEFAULT_ON_HOURS * 3600.0f);
  _relays[0].totalLockoutSec = (uint32_t)(DEFAULT_LOCKOUT_DAYS * 86400.0f);
  _relays[0].relayActive = false;

  for (int i = 1; i < MAX_RELAYS; i++) {
    _relays[i].id = (uint8_t)(i + 1);
    _relays[i].pin = -1;
    _relays[i].enabled = false;
  }
}

void StateManager::soundBuzzer(bool turningOn) {
  pinMode(PIN_BUZZER, OUTPUT);
  if (turningOn) {
    // Two short energetic beeps when relay is activated ON
    digitalWrite(PIN_BUZZER, HIGH);
    delay(75);
    digitalWrite(PIN_BUZZER, LOW);
    delay(50);
    digitalWrite(PIN_BUZZER, HIGH);
    delay(75);
    digitalWrite(PIN_BUZZER, LOW);
  } else {
    // Single longer lower-tone beep when relay is turned OFF
    digitalWrite(PIN_BUZZER, HIGH);
    delay(200);
    digitalWrite(PIN_BUZZER, LOW);
  }
}

void StateManager::soundAckBuzzer() {
  pinMode(PIN_BUZZER, OUTPUT);
  // 3 distinct short beeps to acknowledge WiFi + Firebase connection
  for (int i = 0; i < 3; i++) {
    digitalWrite(PIN_BUZZER, HIGH);
    delay(90);
    digitalWrite(PIN_BUZZER, LOW);
    if (i < 2) delay(110);
  }
}

void StateManager::addSystemLog(const char* tag, const char* msg) {
  if (!tag || !msg) return;
  if (_logCount < MAX_SYSTEM_LOGS) {
    _systemLogs[_logCount].uptime = _uptimeSeconds;
    strncpy(_systemLogs[_logCount].tag, tag, sizeof(_systemLogs[_logCount].tag) - 1);
    _systemLogs[_logCount].tag[sizeof(_systemLogs[_logCount].tag) - 1] = '\0';
    strncpy(_systemLogs[_logCount].msg, msg, sizeof(_systemLogs[_logCount].msg) - 1);
    _systemLogs[_logCount].msg[sizeof(_systemLogs[_logCount].msg) - 1] = '\0';
    _logCount++;
  } else {
    for (int i = 0; i < MAX_SYSTEM_LOGS - 1; i++) {
      _systemLogs[i] = _systemLogs[i + 1];
    }
    _systemLogs[MAX_SYSTEM_LOGS - 1].uptime = _uptimeSeconds;
    strncpy(_systemLogs[MAX_SYSTEM_LOGS - 1].tag, tag, sizeof(_systemLogs[MAX_SYSTEM_LOGS - 1].tag) - 1);
    _systemLogs[MAX_SYSTEM_LOGS - 1].tag[sizeof(_systemLogs[MAX_SYSTEM_LOGS - 1].tag) - 1] = '\0';
    strncpy(_systemLogs[MAX_SYSTEM_LOGS - 1].msg, msg, sizeof(_systemLogs[MAX_SYSTEM_LOGS - 1].msg) - 1);
    _systemLogs[MAX_SYSTEM_LOGS - 1].msg[sizeof(_systemLogs[MAX_SYSTEM_LOGS - 1].msg) - 1] = '\0';
  }
  Serial.printf("[SYSLOG][%s] %s\n", tag, msg);
}

void StateManager::begin() {
  // Record device boot in wireless/system event log
  addSystemLog("RESTART", (String("Boot reason: ") + ESP.getResetReason()).c_str());

  // Initialize Buzzer on D8 (GPIO15)
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);

  // Initialize Touch Sensor
  pinMode(PIN_TOUCH, INPUT);

  // Load relays from LittleFS or restore legacy settings
  if (!loadAllRelays()) {
    SystemSettings legacySettings;
    storageDB.loadSettings(legacySettings);
    _relays[0].onHours = legacySettings.onHours;
    _relays[0].lockoutDays = legacySettings.lockoutDays;

    PersistentState restoredState;
    if (storageDB.loadState(restoredState)) {
      _relays[0].mode = restoredState.mode;
      _relays[0].onRemainingSec = restoredState.onRemainingSec;
      _relays[0].lockoutRemainingSec = restoredState.lockoutRemainingSec;
      _relays[0].totalOnSec = restoredState.totalOnSec;
      _relays[0].totalLockoutSec = restoredState.totalLockoutSec;

      if (_relays[0].mode == MODE_RELAY_ON) {
        if (_relays[0].onRemainingSec > 0) {
          applyRelayHardware(0, true);
        } else {
          transitionRelayTo(0, MODE_LOCKOUT);
        }
      } else if (_relays[0].mode == MODE_LOCKOUT) {
        applyRelayHardware(0, false);
        if (_relays[0].lockoutRemainingSec == 0) {
          transitionRelayTo(0, MODE_IDLE);
        }
      } else {
        applyRelayHardware(0, false);
      }
    } else {
      transitionRelayTo(0, MODE_IDLE);
    }
  } else {
    // Configure hardware pins for all restored channels
    for (int i = 0; i < _relayCount; i++) {
      if (_relays[i].enabled && _relays[i].pin >= 0) {
        pinMode(_relays[i].pin, OUTPUT);
        bool shouldBeActive = (_relays[i].mode == MODE_RELAY_ON && _relays[i].onRemainingSec > 0);
        _relays[i].relayActive = shouldBeActive;
        digitalWrite(_relays[i].pin, shouldBeActive ? _relays[i].activeLevel : (!_relays[i].activeLevel));
      }
    }
  }

  Serial.printf("[STATE] Multi-Relay Manager online (%d configured relays). Buzzer initialized on D8.\n", _relayCount);
}

void StateManager::applyRelayHardware(int index, bool active) {
  if (index < 0 || index >= _relayCount) return;
  bool prevState = _relays[index].relayActive;
  _relays[index].relayActive = active;

  if (_relays[index].pin >= 0) {
    pinMode(_relays[index].pin, OUTPUT);
    digitalWrite(_relays[index].pin, active ? _relays[index].activeLevel : (!_relays[index].activeLevel));
  }

  Serial.printf("[HW] Relay %d (%s) Pin %d set to %s\n", 
    _relays[index].id, _relays[index].name, _relays[index].pin, active ? "ENERGIZED (ON)" : "OFF");

  if (prevState != active) {
    soundBuzzer(active);
  }
}

const char* StateManager::getModeString() const {
  return getRelayModeString(1);
}

const char* StateManager::getRelayModeString(uint8_t relayId) const {
  for (int i = 0; i < _relayCount; i++) {
    if (_relays[i].id == relayId) {
      switch (_relays[i].mode) {
        case MODE_IDLE:     return "IDLE";
        case MODE_RELAY_ON: return "RELAY_ON";
        case MODE_LOCKOUT:  return "LOCKOUT";
        default:            return "UNKNOWN";
      }
    }
  }
  return "UNKNOWN";
}

void StateManager::transitionRelayTo(int index, SystemMode newMode) {
  if (index < 0 || index >= _relayCount) return;

  _relays[index].mode = newMode;

  switch (newMode) {
    case MODE_IDLE:
      applyRelayHardware(index, false);
      _relays[index].onRemainingSec = 0;
      _relays[index].lockoutRemainingSec = 0;
      Serial.printf("[STATE] Relay %d (%s) ---> Transitioned to IDLE (Ready for switch).\n", 
        _relays[index].id, _relays[index].name);
      break;

    case MODE_RELAY_ON:
      _relays[index].totalOnSec = (uint32_t)(_relays[index].onHours * 3600.0f);
      _relays[index].onRemainingSec = _relays[index].totalOnSec;
      applyRelayHardware(index, true);
      if (index == 0) {
        storageDB.recordCycleTriggered();
      }
      Serial.printf("[STATE] Relay %d (%s) ---> Transitioned to RELAY_ON for %u seconds (%.1f h).\n", 
        _relays[index].id, _relays[index].name, _relays[index].totalOnSec, _relays[index].onHours);
      break;

    case MODE_LOCKOUT:
      applyRelayHardware(index, false);
      _relays[index].totalLockoutSec = (uint32_t)(_relays[index].lockoutDays * 86400.0f);
      _relays[index].lockoutRemainingSec = _relays[index].totalLockoutSec;
      Serial.printf("[STATE] Relay %d (%s) ---> Transitioned to LOCKOUT/OFF for %u seconds (%.1f days).\n", 
        _relays[index].id, _relays[index].name, _relays[index].totalLockoutSec, _relays[index].lockoutDays);
      break;
  }

  persistAllRelays();
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

      if (_relays[0].mode == MODE_IDLE) {
        Serial.println(F("[TOUCH] Primary Relay 1 is IDLE. Triggering Active Phase."));
        transitionRelayTo(0, MODE_RELAY_ON);
      } else if (_relays[0].mode == MODE_LOCKOUT) {
        Serial.printf("[TOUCH] BLOCKED! Lockout cooldown in progress (%u s remaining). Touch ignored.\n", 
          _relays[0].lockoutRemainingSec);
      } else if (_relays[0].mode == MODE_RELAY_ON) {
        Serial.println(F("[TOUCH] Relay 1 is already active in cycle. Touch ignored."));
      }
    } else if (!isTouched) {
      _touchTriggered = false;
    }
  }

  _lastTouchReading = isTouched;
}

void StateManager::tickOneSecond() {
  _uptimeSeconds++;

  // Record runtime analytics for primary channel
  if (_relayCount > 0) {
    storageDB.recordStateSecond(_relays[0].relayActive);
  }

  for (int i = 0; i < _relayCount; i++) {
    if (!_relays[i].enabled) continue;

    if (_relays[i].mode == MODE_RELAY_ON) {
      if (_relays[i].onRemainingSec > 0) {
        _relays[i].onRemainingSec--;
      }
      if (_relays[i].onRemainingSec == 0) {
        Serial.printf("[TIMER] Relay %d (%s) active duty expired! Transitioning to Lockout/Off.\n", 
          _relays[i].id, _relays[i].name);
        transitionRelayTo(i, MODE_LOCKOUT);
      }
    } 
    else if (_relays[i].mode == MODE_LOCKOUT) {
      if (_relays[i].lockoutRemainingSec > 0) {
        _relays[i].lockoutRemainingSec--;
      }
      if (_relays[i].lockoutRemainingSec == 0) {
        Serial.printf("[TIMER] Relay %d (%s) lockout/off period complete! Transitioning to IDLE.\n", 
          _relays[i].id, _relays[i].name);
        transitionRelayTo(i, MODE_IDLE);
      }
    }
  }

  if (millis() - _lastPersistMillis >= PERSIST_INTERVAL_MS) {
    _lastPersistMillis = millis();
    persistAllRelays();
  }
}

// Backward compatible primary methods
bool StateManager::triggerCycle(const char* source, const char* oneTimeToken) {
  return triggerRelay(1, source, oneTimeToken);
}

bool StateManager::cancelCycle(const char* source) {
  return cancelRelay(1, source);
}

bool StateManager::resetLockout(const char* source) {
  return resetRelayLockout(1, source);
}

bool StateManager::extendActiveTimer(uint32_t additionalSeconds) {
  return extendRelayTimer(1, additionalSeconds);
}

bool StateManager::updateTimers(float onHours, float lockoutDays) {
  if (onHours < MIN_ON_HOURS || onHours > MAX_ON_HOURS ||
      lockoutDays < MIN_LOCKOUT_DAYS || lockoutDays > MAX_LOCKOUT_DAYS) {
    return false;
  }
  _relays[0].onHours = onHours;
  _relays[0].lockoutDays = lockoutDays;
  persistAllRelays();
  return true;
}

// Multi-Relay Operations
const RelayChannel* StateManager::getRelay(int index) const {
  if (index >= 0 && index < _relayCount) return &_relays[index];
  return nullptr;
}

RelayChannel* StateManager::getRelayById(uint8_t id) {
  for (int i = 0; i < _relayCount; i++) {
    if (_relays[i].id == id) return &_relays[i];
  }
  return nullptr;
}

bool StateManager::triggerRelay(uint8_t relayId, const char* source, const char* oneTimeToken) {
  for (int i = 0; i < _relayCount; i++) {
    if (_relays[i].id == relayId && _relays[i].enabled) {
      if (_relays[i].mode != MODE_IDLE) {
        Serial.printf("[CMD] Cannot trigger Relay %d from %s: mode is %s\n", 
          relayId, source, getRelayModeString(relayId));
        return false;
      }
      if (oneTimeToken && strlen(oneTimeToken) > 0) {
        if (!storageDB.consumeOneTimeToken(oneTimeToken)) {
          Serial.println(F("[AUTH-L2] REJECTED: Invalid or expired 1-time token."));
          return false;
        }
      }
      Serial.printf("[CMD] Relay %d triggered by %s\n", relayId, source);
      transitionRelayTo(i, MODE_RELAY_ON);
      return true;
    }
  }
  return false;
}

bool StateManager::cancelRelay(uint8_t relayId, const char* source) {
  for (int i = 0; i < _relayCount; i++) {
    if (_relays[i].id == relayId && _relays[i].enabled) {
      Serial.printf("[CMD] Relay %d cutoff initiated by %s\n", relayId, source);
      transitionRelayTo(i, MODE_IDLE);
      return true;
    }
  }
  return false;
}

bool StateManager::toggleRelay(uint8_t relayId, const char* source) {
  for (int i = 0; i < _relayCount; i++) {
    if (_relays[i].id == relayId && _relays[i].enabled) {
      if (_relays[i].relayActive || _relays[i].mode == MODE_RELAY_ON) {
        Serial.printf("[CMD] Relay %d toggled OFF by %s\n", relayId, source);
        transitionRelayTo(i, MODE_IDLE);
      } else {
        Serial.printf("[CMD] Relay %d toggled ON by %s\n", relayId, source);
        transitionRelayTo(i, MODE_RELAY_ON);
      }
      return true;
    }
  }
  return false;
}

bool StateManager::resetRelayLockout(uint8_t relayId, const char* source) {
  for (int i = 0; i < _relayCount; i++) {
    if (_relays[i].id == relayId && _relays[i].enabled) {
      if (_relays[i].mode != MODE_LOCKOUT) return false;
      Serial.printf("[CMD] Relay %d Lockout bypassed by %s\n", relayId, source);
      transitionRelayTo(i, MODE_IDLE);
      return true;
    }
  }
  return false;
}

bool StateManager::extendRelayTimer(uint8_t relayId, uint32_t additionalSeconds) {
  for (int i = 0; i < _relayCount; i++) {
    if (_relays[i].id == relayId && _relays[i].enabled) {
      if (_relays[i].mode != MODE_RELAY_ON) return false;
      _relays[i].onRemainingSec += additionalSeconds;
      _relays[i].totalOnSec += additionalSeconds;
      persistAllRelays();
      Serial.printf("[TIMER] Extended Relay %d by %u s. New remaining: %u s\n", 
        relayId, additionalSeconds, _relays[i].onRemainingSec);
      return true;
    }
  }
  return false;
}

bool StateManager::addRelay(const char* name, int8_t pin, uint8_t activeLevel, float onHours, float lockoutDays, String& errorMsg) {
  if (_relayCount >= MAX_RELAYS) {
    errorMsg = "Maximum relay capacity reached (limit 4).";
    return false;
  }
  if (!name || strlen(name) == 0) {
    errorMsg = "Relay name is required.";
    return false;
  }
  if (pin == PIN_BUZZER) {
    errorMsg = "GPIO15 (D8) is reserved for the Alarm Buzzer and cannot be used for relays.";
    return false;
  }
  if (pin < 0 || pin > 16) {
    errorMsg = "Invalid ESP8266 GPIO pin.";
    return false;
  }
  for (int i = 0; i < _relayCount; i++) {
    if (_relays[i].pin == pin && _relays[i].enabled) {
      errorMsg = String("GPIO") + pin + " is already bound to Relay " + _relays[i].id;
      return false;
    }
  }

  uint8_t nextId = 1;
  for (int i = 0; i < _relayCount; i++) {
    if (_relays[i].id >= nextId) nextId = _relays[i].id + 1;
  }

  int slot = _relayCount;
  _relays[slot].id = nextId;
  strncpy(_relays[slot].name, name, sizeof(_relays[slot].name) - 1);
  _relays[slot].pin = pin;
  _relays[slot].activeLevel = activeLevel ? 1 : 0;
  _relays[slot].onHours = (onHours >= 0.1f) ? onHours : DEFAULT_ON_HOURS;
  _relays[slot].lockoutDays = (lockoutDays >= 0.0f) ? lockoutDays : DEFAULT_LOCKOUT_DAYS;
  _relays[slot].enabled = true;
  _relays[slot].mode = MODE_IDLE;
  _relays[slot].onRemainingSec = 0;
  _relays[slot].lockoutRemainingSec = 0;
  _relays[slot].totalOnSec = (uint32_t)(_relays[slot].onHours * 3600.0f);
  _relays[slot].totalLockoutSec = (uint32_t)(_relays[slot].lockoutDays * 86400.0f);
  _relays[slot].relayActive = false;

  pinMode(pin, OUTPUT);
  digitalWrite(pin, !_relays[slot].activeLevel);

  _relayCount++;
  persistAllRelays();
  soundBuzzer(true); // Short chime for added hardware

  Serial.printf("[CONFIG] Added new Relay %d (%s) bound to GPIO%d\n", nextId, name, pin);
  return true;
}

bool StateManager::updateRelay(uint8_t relayId, const char* name, int8_t pin, uint8_t activeLevel, float onHours, float lockoutDays, String& errorMsg) {
  for (int i = 0; i < _relayCount; i++) {
    if (_relays[i].id == relayId) {
      if (pin == PIN_BUZZER) {
        errorMsg = "GPIO15 (D8) is reserved for Buzzer.";
        return false;
      }
      for (int j = 0; j < _relayCount; j++) {
        if (j != i && _relays[j].pin == pin && _relays[j].enabled) {
          errorMsg = String("GPIO") + pin + " is already in use by another relay.";
          return false;
        }
      }

      if (name && strlen(name) > 0) {
        strncpy(_relays[i].name, name, sizeof(_relays[i].name) - 1);
      }
      if (pin >= 0) {
        _relays[i].pin = pin;
        pinMode(pin, OUTPUT);
      }
      _relays[i].activeLevel = activeLevel ? 1 : 0;
      if (onHours >= 0.1f) _relays[i].onHours = onHours;
      if (lockoutDays >= 0.0f) _relays[i].lockoutDays = lockoutDays;

      persistAllRelays();
      return true;
    }
  }
  errorMsg = "Relay not found.";
  return false;
}

bool StateManager::deleteRelay(uint8_t relayId, String& errorMsg) {
  if (relayId == 1) {
    errorMsg = "Primary Relay 1 cannot be deleted. You can edit its pin and timers in settings.";
    return false;
  }

  int targetIndex = -1;
  for (int i = 0; i < _relayCount; i++) {
    if (_relays[i].id == relayId) {
      targetIndex = i;
      break;
    }
  }

  if (targetIndex == -1) {
    errorMsg = "Relay channel not found.";
    return false;
  }

  // De-energize hardware pin before removing
  if (_relays[targetIndex].pin >= 0) {
    digitalWrite(_relays[targetIndex].pin, !_relays[targetIndex].activeLevel);
    pinMode(_relays[targetIndex].pin, INPUT);
  }

  for (int i = targetIndex; i < _relayCount - 1; i++) {
    _relays[i] = _relays[i + 1];
  }
  _relayCount--;
  memset(&_relays[_relayCount], 0, sizeof(RelayChannel));

  persistAllRelays();
  Serial.printf("[CONFIG] Removed Relay Channel %d\n", relayId);
  return true;
}

void StateManager::persistAllRelays() {
  StaticJsonDocument<2048> doc;
  JsonArray arr = doc.createNestedArray("relays");

  for (int i = 0; i < _relayCount; i++) {
    JsonObject r = arr.createNestedObject();
    r["id"] = _relays[i].id;
    r["name"] = _relays[i].name;
    r["pin"] = _relays[i].pin;
    r["activeLevel"] = _relays[i].activeLevel;
    r["onHours"] = _relays[i].onHours;
    r["lockoutDays"] = _relays[i].lockoutDays;
    r["enabled"] = _relays[i].enabled;
    r["mode"] = (uint8_t)_relays[i].mode;
    r["onRemainingSec"] = _relays[i].onRemainingSec;
    r["lockoutRemainingSec"] = _relays[i].lockoutRemainingSec;
    r["totalOnSec"] = _relays[i].totalOnSec;
    r["totalLockoutSec"] = _relays[i].totalLockoutSec;
    r["relayActive"] = _relays[i].relayActive;
  }

  File f = LittleFS.open("/relays.json", "w");
  if (f) {
    serializeJson(doc, f);
    f.close();
  }

  // Backward compatibility with legacy storage files
  SystemSettings s;
  s.onHours = _relays[0].onHours;
  s.lockoutDays = _relays[0].lockoutDays;
  storageDB.saveSettings(s);

  PersistentState st;
  st.mode = _relays[0].mode;
  st.onRemainingSec = _relays[0].onRemainingSec;
  st.lockoutRemainingSec = _relays[0].lockoutRemainingSec;
  st.totalOnSec = _relays[0].totalOnSec;
  st.totalLockoutSec = _relays[0].totalLockoutSec;
  storageDB.saveState(st);
}

bool StateManager::loadAllRelays() {
  if (!LittleFS.exists("/relays.json")) {
    return false;
  }

  File f = LittleFS.open("/relays.json", "r");
  if (!f) return false;

  StaticJsonDocument<2048> doc;
  DeserializationError err = deserializeJson(doc, f);
  f.close();
  if (err) return false;

  JsonArray arr = doc["relays"].as<JsonArray>();
  if (arr.isNull() || arr.size() == 0) return false;

  _relayCount = 0;
  for (JsonObject r : arr) {
    if (_relayCount >= MAX_RELAYS) break;
    int idx = _relayCount;
    _relays[idx].id = r["id"] | (idx + 1);
    const char* n = r["name"] | "Relay";
    strncpy(_relays[idx].name, n, sizeof(_relays[idx].name) - 1);
    _relays[idx].pin = r["pin"] | ((idx == 0) ? PIN_RELAY : -1);
    _relays[idx].activeLevel = r["activeLevel"] | RELAY_ACTIVE_LEVEL;
    _relays[idx].onHours = r["onHours"] | DEFAULT_ON_HOURS;
    _relays[idx].lockoutDays = r["lockoutDays"] | DEFAULT_LOCKOUT_DAYS;
    _relays[idx].enabled = r["enabled"] | true;
    uint8_t m = r["mode"] | 0;
    _relays[idx].mode = (m <= 2) ? (SystemMode)m : MODE_IDLE;
    _relays[idx].onRemainingSec = r["onRemainingSec"] | 0;
    _relays[idx].lockoutRemainingSec = r["lockoutRemainingSec"] | 0;
    _relays[idx].totalOnSec = r["totalOnSec"] | (uint32_t)(_relays[idx].onHours * 3600.0f);
    _relays[idx].totalLockoutSec = r["totalLockoutSec"] | (uint32_t)(_relays[idx].lockoutDays * 86400.0f);
    _relays[idx].relayActive = r["relayActive"] | false;
    _relayCount++;
  }

  return (_relayCount > 0);
}

String StateManager::getStatusJson(const char* currentUsername, const char* userRole) {
  StaticJsonDocument<3072> doc;

  // Primary Channel 1 telemetry for legacy UI & cloud compatibility
  doc["mode"] = getRelayModeString(1);
  doc["relayActive"] = _relays[0].relayActive;
  doc["touchActive"] = isTouchAllowed();
  doc["lockoutActive"] = isLockoutActive();
  doc["onRemainingSec"] = _relays[0].onRemainingSec;
  doc["totalOnSec"] = _relays[0].totalOnSec;
  doc["lockoutRemainingSec"] = _relays[0].lockoutRemainingSec;
  doc["totalLockoutSec"] = _relays[0].totalLockoutSec;
  doc["uptimeSeconds"] = _uptimeSeconds;
  doc["version"] = FIRMWARE_VERSION;
  doc["buzzerPin"] = PIN_BUZZER;

  // Real RF & Hardware diagnostics from ESP8266
  doc["wifiRssi"] = WiFi.RSSI();
  doc["wifiChannel"] = WiFi.channel();
  doc["softApStations"] = WiFi.softAPgetStationNum();
  doc["freeHeap"] = ESP.getFreeHeap();

  JsonObject cfg = doc.createNestedObject("config");
  cfg["onHours"] = _relays[0].onHours;
  cfg["lockoutDays"] = _relays[0].lockoutDays;

  // Multi-Relay array with dedicated pins and separate countdowns
  JsonArray relaysArr = doc.createNestedArray("relays");
  for (int i = 0; i < _relayCount; i++) {
    if (!_relays[i].enabled) continue;
    JsonObject rObj = relaysArr.createNestedObject();
    rObj["id"] = _relays[i].id;
    rObj["name"] = _relays[i].name;
    rObj["pin"] = _relays[i].pin;
    rObj["activeLevel"] = _relays[i].activeLevel;
    rObj["onHours"] = _relays[i].onHours;
    rObj["lockoutDays"] = _relays[i].lockoutDays;
    rObj["mode"] = getRelayModeString(_relays[i].id);
    rObj["relayActive"] = _relays[i].relayActive;
    rObj["onRemainingSec"] = _relays[i].onRemainingSec;
    rObj["totalOnSec"] = _relays[i].totalOnSec;
    rObj["lockoutRemainingSec"] = _relays[i].lockoutRemainingSec;
    rObj["totalLockoutSec"] = _relays[i].totalLockoutSec;
    rObj["enabled"] = _relays[i].enabled;
  }

  if (currentUsername) {
    JsonObject cur = doc.createNestedObject("currentUser");
    cur["username"] = currentUsername;
    cur["role"] = userRole ? userRole : "guest_analytics";
  }

  // Monthly Analytics
  const MonthlyAnalytics& an = storageDB.getAnalytics();
  JsonObject analyticsObj = doc.createNestedObject("analytics");
  analyticsObj["currentMonth"] = an.currentMonthYear;
  analyticsObj["totalOnSeconds"] = an.totalOnSeconds;
  analyticsObj["totalOffSeconds"] = an.totalOffSeconds;
  analyticsObj["totalCyclesCount"] = an.totalCyclesCount;
  float onH = (float)an.totalOnSeconds / 3600.0f;
  float offH = (float)an.totalOffSeconds / 3600.0f;
  analyticsObj["totalOnHours"] = serialized(String(onH, 1));
  analyticsObj["totalOffHours"] = serialized(String(offH, 1));

  // Level 2 Pending Request
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

  // Handshake ACK & Connection Pipeline Status
  JsonObject ackObj = doc.createNestedObject("ack");
  ackObj["verified"] = _ackVerified;
  ackObj["bootReason"] = ESP.getResetReason();
  ackObj["wifiConnected"] = (WiFi.status() == WL_CONNECTED);
  ackObj["softApSsid"] = WIFI_AP_SSID;
  ackObj["softApIp"] = WiFi.softAPIP().toString();
  ackObj["staIp"] = (WiFi.status() == WL_CONNECTED) ? WiFi.localIP().toString() : "0.0.0.0";
  ackObj["buzzerChimes"] = 3;

  // System & Wireless Event Logs
  JsonArray logsArr = doc.createNestedArray("systemLogs");
  for (int i = 0; i < _logCount; i++) {
    JsonObject lObj = logsArr.createNestedObject();
    lObj["uptime"] = _systemLogs[i].uptime;
    lObj["tag"] = _systemLogs[i].tag;
    lObj["msg"] = _systemLogs[i].msg;
  }

  String output;
  serializeJson(doc, output);
  return output;
}
