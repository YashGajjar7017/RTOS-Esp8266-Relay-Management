#include "storage_db.h"
#include <LittleFS.h>
#include <ArduinoJson.h>

StorageDB storageDB;

StorageDB::StorageDB() : _userCount(0), _fsMounted(false) {
  memset(_users, 0, sizeof(_users));
  memset(&_analytics, 0, sizeof(_analytics));
  memset(&_oneTimeReq, 0, sizeof(_oneTimeReq));
  strncpy(_analytics.currentMonthYear, "November 2026", sizeof(_analytics.currentMonthYear) - 1);
  _analytics.totalOnSeconds = 48 * 3600;  // 48 hours baseline demo
  _analytics.totalOffSeconds = 240 * 3600; // 240 hours baseline demo
  _analytics.totalCyclesCount = 6;
  _oneTimeReq.active = false;
}

bool StorageDB::begin() {
  if (LittleFS.begin()) {
    _fsMounted = true;
    Serial.println(F("[DB] LittleFS mounted successfully."));
  } else {
    Serial.println(F("[DB] LittleFS mount failed, formatting..."));
    if (LittleFS.format() && LittleFS.begin()) {
      _fsMounted = true;
      Serial.println(F("[DB] LittleFS formatted and mounted."));
    } else {
      Serial.println(F("[DB] CRITICAL: LittleFS failed to initialize!"));
      _fsMounted = false;
    }
  }

  // Load or seed users
  if (!loadUsersFromFile() || _userCount < 3) {
    seedPredefinedUsers();
    saveUsersToFile();
  }

  // Load monthly analytics
  loadAnalytics(_analytics);

  return true;
}

void StorageDB::seedPredefinedUsers() {
  _userCount = 3;

  // 1. Yash_Gajjar: Super Admin (System writes, timers, lockout bypass, permission grants)
  _users[0].id = 1;
  strncpy(_users[0].username, USER1_NAME, sizeof(_users[0].username) - 1);
  strncpy(_users[0].password, USER1_PASS, sizeof(_users[0].password) - 1);
  strncpy(_users[0].role, USER1_ROLE, sizeof(_users[0].role) - 1);
  strncpy(_users[0].status, "active", sizeof(_users[0].status) - 1);
  strncpy(_users[0].created, "2026-11-11", sizeof(_users[0].created) - 1);

  // 2. Mahi_Gajjar: Guarded Operator (Requires Admin confirmation popup for 1-time grant, session then destroyed)
  _users[1].id = 2;
  strncpy(_users[1].username, USER2_NAME, sizeof(_users[1].username) - 1);
  strncpy(_users[1].password, USER2_PASS, sizeof(_users[1].password) - 1);
  strncpy(_users[1].role, USER2_ROLE, sizeof(_users[1].role) - 1);
  strncpy(_users[1].status, "active", sizeof(_users[1].status) - 1);
  strncpy(_users[1].created, "2026-11-11", sizeof(_users[1].created) - 1);

  // 3. Guest: Read-Only Analytics Viewer (Can only view Monthly On/Off Switch Analytics)
  _users[2].id = 3;
  strncpy(_users[2].username, USER3_NAME, sizeof(_users[2].username) - 1);
  strncpy(_users[2].password, USER3_PASS, sizeof(_users[2].password) - 1);
  strncpy(_users[2].role, USER3_ROLE, sizeof(_users[2].role) - 1);
  strncpy(_users[2].status, "active", sizeof(_users[2].status) - 1);
  strncpy(_users[2].created, "2026-11-11", sizeof(_users[2].created) - 1);

  Serial.println(F("[DB] Initialized 3-tier user accounts in Flash:"));
  Serial.printf("[DB] 1. %s (%s) - Master Admin\n", USER1_NAME, USER1_ROLE);
  Serial.printf("[DB] 2. %s (%s) - Guarded Operator (1-time grant)\n", USER2_NAME, USER2_ROLE);
  Serial.printf("[DB] 3. %s (%s) - Guest Analytics\n", USER3_NAME, USER3_ROLE);
}

bool StorageDB::loadUsersFromFile() {
  if (!_fsMounted || !LittleFS.exists("/users.json")) {
    return false;
  }

  File f = LittleFS.open("/users.json", "r");
  if (!f) return false;

  StaticJsonDocument<1536> doc;
  DeserializationError error = deserializeJson(doc, f);
  f.close();

  if (error) {
    Serial.println(F("[DB] Error parsing users.json"));
    return false;
  }

  JsonArray arr = doc["users"].as<JsonArray>();
  _userCount = 0;
  for (JsonObject u : arr) {
    if (_userCount >= MAX_USERS) break;
    _users[_userCount].id = u["id"] | (_userCount + 1);
    strncpy(_users[_userCount].username, u["username"] | "", sizeof(_users[_userCount].username) - 1);
    strncpy(_users[_userCount].password, u["password"] | "", sizeof(_users[_userCount].password) - 1);
    strncpy(_users[_userCount].role, u["role"] | "operator", sizeof(_users[_userCount].role) - 1);
    strncpy(_users[_userCount].status, u["status"] | "active", sizeof(_users[_userCount].status) - 1);
    strncpy(_users[_userCount].created, u["created"] | "2026-11-11", sizeof(_users[_userCount].created) - 1);
    _userCount++;
  }

  // Ensure Yash_Gajjar is always present
  UserRecord adminCheck;
  if (!findUser(USER1_NAME, adminCheck)) {
    return false; // Force re-seed
  }

  Serial.printf("[DB] Loaded %d user(s) from Flash DB.\n", _userCount);
  return true;
}

bool StorageDB::saveUsersToFile() {
  if (!_fsMounted) return false;

  StaticJsonDocument<1536> doc;
  JsonArray arr = doc.createNestedArray("users");

  for (int i = 0; i < _userCount; i++) {
    JsonObject u = arr.createNestedObject();
    u["id"] = _users[i].id;
    u["username"] = _users[i].username;
    u["password"] = _users[i].password;
    u["role"] = _users[i].role;
    u["status"] = _users[i].status;
    u["created"] = _users[i].created;
  }

  File f = LittleFS.open("/users.json", "w");
  if (!f) return false;
  serializeJson(doc, f);
  f.close();
  return true;
}

int StorageDB::getUserCount() {
  return _userCount;
}

bool StorageDB::getUser(int index, UserRecord& user) {
  if (index < 0 || index >= _userCount) return false;
  user = _users[index];
  return true;
}

bool StorageDB::findUser(const char* username, UserRecord& user) {
  for (int i = 0; i < _userCount; i++) {
    if (strcasecmp(_users[i].username, username) == 0) {
      user = _users[i];
      return true;
    }
  }
  return false;
}

bool StorageDB::verifyCredentials(const char* username, const char* password, UserRecord& matchedUser) {
  for (int i = 0; i < _userCount; i++) {
    if (strcasecmp(_users[i].username, username) == 0 && strcmp(_users[i].password, password) == 0) {
      matchedUser = _users[i];
      return true;
    }
  }
  return false;
}

bool StorageDB::verifyAdminPassword(const char* adminPassword) {
  for (int i = 0; i < _userCount; i++) {
    if (strcmp(_users[i].role, "admin") == 0 && strcmp(_users[i].password, adminPassword) == 0) {
      return true;
    }
  }
  return false;
}

// Level 2: One-Time Session Access Flow for Mahi_Gajjar
bool StorageDB::createOneTimeRequest(const char* username, const char* action, String& requestId, String& errorMsg) {
  UserRecord u;
  if (!findUser(username, u)) {
    errorMsg = "User not found";
    return false;
  }

  if (strcmp(u.role, "guarded_operator") != 0 && strcmp(u.role, "admin") != 0) {
    errorMsg = "User does not have system operation privileges";
    return false;
  }

  // Create pending one-time action request
  snprintf(_oneTimeReq.requestId, sizeof(_oneTimeReq.requestId), "req-%lu", millis());
  strncpy(_oneTimeReq.username, username, sizeof(_oneTimeReq.username) - 1);
  strncpy(_oneTimeReq.requestedAction, action, sizeof(_oneTimeReq.requestedAction) - 1);
  _oneTimeReq.requestTime = millis();
  _oneTimeReq.active = true;
  _oneTimeReq.approved = false;
  _oneTimeReq.oneTimeToken[0] = '\0';

  requestId = String(_oneTimeReq.requestId);
  Serial.printf("[AUTH-L2] One-Time Access requested by %s for action '%s' (ReqID: %s)\n", username, action, _oneTimeReq.requestId);
  return true;
}

bool StorageDB::approveOneTimeRequest(const char* adminPassword, String& oneTimeToken, String& errorMsg) {
  if (!verifyAdminPassword(adminPassword)) {
    errorMsg = "Invalid Admin Master Password. Access grant denied.";
    return false;
  }

  if (!_oneTimeReq.active) {
    errorMsg = "No active request waiting for approval.";
    return false;
  }

  _oneTimeReq.approved = true;
  snprintf(_oneTimeReq.oneTimeToken, sizeof(_oneTimeReq.oneTimeToken), "1time-%lu-%04x", millis(), (uint16_t)random(0xFFFF));
  oneTimeToken = String(_oneTimeReq.oneTimeToken);

  Serial.printf("[AUTH-L2] Admin approved 1-time grant for %s. Issued Token: %s\n", _oneTimeReq.username, _oneTimeReq.oneTimeToken);
  return true;
}

bool StorageDB::consumeOneTimeToken(const char* token) {
  if (_oneTimeReq.active && _oneTimeReq.approved) {
    if (strcmp(_oneTimeReq.oneTimeToken, token) == 0) {
      // Token is valid! Destroy session immediately
      Serial.printf("[AUTH-L2] Consumed 1-time token '%s'. Session destroyed immediately.\n", token);
      memset(&_oneTimeReq, 0, sizeof(_oneTimeReq));
      _oneTimeReq.active = false;
      return true;
    }
  }
  return false;
}

void StorageDB::rejectOneTimeRequest() {
  Serial.printf("[AUTH-L2] One-time request %s was rejected/dismissed by Admin.\n", _oneTimeReq.requestId);
  memset(&_oneTimeReq, 0, sizeof(_oneTimeReq));
  _oneTimeReq.active = false;
}

// Monthly Analytics Tracking
void StorageDB::recordStateSecond(bool relayOn) {
  if (relayOn) {
    _analytics.totalOnSeconds++;
  } else {
    _analytics.totalOffSeconds++;
  }
}

void StorageDB::recordCycleTriggered() {
  _analytics.totalCyclesCount++;
  saveAnalytics(_analytics);
}

bool StorageDB::loadAnalytics(MonthlyAnalytics& analytics) {
  if (!_fsMounted || !LittleFS.exists("/analytics.json")) {
    return false;
  }

  File f = LittleFS.open("/analytics.json", "r");
  if (!f) return false;

  StaticJsonDocument<384> doc;
  DeserializationError err = deserializeJson(doc, f);
  f.close();
  if (err) return false;

  strncpy(analytics.currentMonthYear, doc["month"] | "November 2026", sizeof(analytics.currentMonthYear) - 1);
  analytics.totalOnSeconds = doc["onSeconds"] | 0;
  analytics.totalOffSeconds = doc["offSeconds"] | 0;
  analytics.totalCyclesCount = doc["cycles"] | 0;
  analytics.lastMonthOnSeconds = doc["lastMonthOn"] | 0;
  analytics.lastMonthOffSeconds = doc["lastMonthOff"] | 0;
  return true;
}

bool StorageDB::saveAnalytics(const MonthlyAnalytics& analytics) {
  if (!_fsMounted) return false;

  StaticJsonDocument<384> doc;
  doc["month"] = analytics.currentMonthYear;
  doc["onSeconds"] = analytics.totalOnSeconds;
  doc["offSeconds"] = analytics.totalOffSeconds;
  doc["cycles"] = analytics.totalCyclesCount;
  doc["lastMonthOn"] = analytics.lastMonthOnSeconds;
  doc["lastMonthOff"] = analytics.lastMonthOffSeconds;

  File f = LittleFS.open("/analytics.json", "w");
  if (!f) return false;
  serializeJson(doc, f);
  f.close();
  return true;
}

String StorageDB::getAnalyticsJson() {
  StaticJsonDocument<512> doc;
  doc["currentMonth"] = _analytics.currentMonthYear;
  doc["totalOnSeconds"] = _analytics.totalOnSeconds;
  doc["totalOffSeconds"] = _analytics.totalOffSeconds;
  doc["totalCyclesCount"] = _analytics.totalCyclesCount;
  
  float onHours = (float)_analytics.totalOnSeconds / 3600.0f;
  float offHours = (float)_analytics.totalOffSeconds / 3600.0f;
  float totalHours = onHours + offHours;
  float dutyPercentage = (totalHours > 0) ? (onHours / totalHours * 100.0f) : 0.0f;

  doc["totalOnHours"] = serialized(String(onHours, 1));
  doc["totalOffHours"] = serialized(String(offHours, 1));
  doc["dutyCyclePercent"] = serialized(String(dutyPercentage, 1));

  String out;
  serializeJson(doc, out);
  return out;
}

String StorageDB::getPendingRequestJson() {
  StaticJsonDocument<384> doc;
  doc["active"] = _oneTimeReq.active;
  doc["requestId"] = _oneTimeReq.requestId;
  doc["username"] = _oneTimeReq.username;
  doc["requestedAction"] = _oneTimeReq.requestedAction;
  doc["approved"] = _oneTimeReq.approved;
  doc["oneTimeToken"] = _oneTimeReq.approved ? _oneTimeReq.oneTimeToken : "";
  
  String out;
  serializeJson(doc, out);
  return out;
}

bool StorageDB::registerUserWithAdminApproval(const char* username, const char* password, const char* role, const char* adminPassword, String& errorMsg) {
  if (_userCount >= MAX_USERS) {
    errorMsg = "Database capacity full (Max 5 users)";
    return false;
  }

  if (!verifyAdminPassword(adminPassword)) {
    errorMsg = "Invalid Admin Master Password. Authorization rejected.";
    return false;
  }

  UserRecord dummy;
  if (findUser(username, dummy)) {
    errorMsg = "Username already exists";
    return false;
  }

  uint8_t nextId = 1;
  for (int i = 0; i < _userCount; i++) {
    if (_users[i].id >= nextId) nextId = _users[i].id + 1;
  }

  _users[_userCount].id = nextId;
  strncpy(_users[_userCount].username, username, sizeof(_users[_userCount].username) - 1);
  strncpy(_users[_userCount].password, password, sizeof(_users[_userCount].password) - 1);
  strncpy(_users[_userCount].role, role ? role : "guest_analytics", sizeof(_users[_userCount].role) - 1);
  strncpy(_users[_userCount].status, "active", sizeof(_users[_userCount].status) - 1);
  strncpy(_users[_userCount].created, "2026-11-11", sizeof(_users[_userCount].created) - 1);
  _userCount++;

  saveUsersToFile();
  Serial.printf("[DB] Authorized and enrolled user: %s (Role: %s)\n", username, role);
  return true;
}

bool StorageDB::deleteUser(uint8_t id, String& errorMsg) {
  int targetIndex = -1;
  for (int i = 0; i < _userCount; i++) {
    if (_users[i].id == id) {
      targetIndex = i;
      break;
    }
  }

  if (targetIndex == -1) {
    errorMsg = "User not found";
    return false;
  }

  if (strcmp(_users[targetIndex].username, USER1_NAME) == 0) {
    errorMsg = "Cannot delete Super Administrator (Yash_Gajjar)";
    return false;
  }

  for (int i = targetIndex; i < _userCount - 1; i++) {
    _users[i] = _users[i + 1];
  }
  _userCount--;

  saveUsersToFile();
  Serial.printf("[DB] Deleted user ID: %d\n", id);
  return true;
}

String StorageDB::getUsersJson() {
  StaticJsonDocument<1536> doc;
  JsonArray arr = doc.to<JsonArray>();

  for (int i = 0; i < _userCount; i++) {
    JsonObject u = arr.createNestedObject();
    u["id"] = _users[i].id;
    u["username"] = _users[i].username;
    u["role"] = _users[i].role;
    u["status"] = _users[i].status;
    u["created"] = _users[i].created;
  }

  String output;
  serializeJson(doc, output);
  return output;
}

bool StorageDB::loadSettings(SystemSettings& settings) {
  settings.onHours = DEFAULT_ON_HOURS;
  settings.lockoutDays = DEFAULT_LOCKOUT_DAYS;

  if (!_fsMounted || !LittleFS.exists("/settings.json")) {
    return false;
  }

  File f = LittleFS.open("/settings.json", "r");
  if (!f) return false;

  StaticJsonDocument<256> doc;
  DeserializationError err = deserializeJson(doc, f);
  f.close();
  if (err) return false;

  settings.onHours = doc["onHours"] | DEFAULT_ON_HOURS;
  settings.lockoutDays = doc["lockoutDays"] | DEFAULT_LOCKOUT_DAYS;

  if (settings.onHours < MIN_ON_HOURS) settings.onHours = MIN_ON_HOURS;
  if (settings.onHours > MAX_ON_HOURS) settings.onHours = MAX_ON_HOURS;
  if (settings.lockoutDays < MIN_LOCKOUT_DAYS) settings.lockoutDays = MIN_LOCKOUT_DAYS;
  if (settings.lockoutDays > MAX_LOCKOUT_DAYS) settings.lockoutDays = MAX_LOCKOUT_DAYS;

  return true;
}

bool StorageDB::saveSettings(const SystemSettings& settings) {
  if (!_fsMounted) return false;

  StaticJsonDocument<256> doc;
  doc["onHours"] = settings.onHours;
  doc["lockoutDays"] = settings.lockoutDays;

  File f = LittleFS.open("/settings.json", "w");
  if (!f) return false;
  serializeJson(doc, f);
  f.close();
  return true;
}

bool StorageDB::loadState(PersistentState& state) {
  state.mode = MODE_IDLE;
  state.onRemainingSec = 0;
  state.lockoutRemainingSec = 0;
  state.totalOnSec = 0;
  state.totalLockoutSec = 0;

  if (!_fsMounted || !LittleFS.exists("/state.json")) {
    return false;
  }

  File f = LittleFS.open("/state.json", "r");
  if (!f) return false;

  StaticJsonDocument<256> doc;
  DeserializationError err = deserializeJson(doc, f);
  f.close();
  if (err) return false;

  uint8_t m = doc["mode"] | 0;
  state.mode = (m <= 2) ? (SystemMode)m : MODE_IDLE;
  state.onRemainingSec = doc["onRemainingSec"] | 0;
  state.lockoutRemainingSec = doc["lockoutRemainingSec"] | 0;
  state.totalOnSec = doc["totalOnSec"] | 0;
  state.totalLockoutSec = doc["totalLockoutSec"] | 0;

  return true;
}

bool StorageDB::saveState(const PersistentState& state) {
  if (!_fsMounted) return false;

  StaticJsonDocument<256> doc;
  doc["mode"] = (uint8_t)state.mode;
  doc["onRemainingSec"] = state.onRemainingSec;
  doc["lockoutRemainingSec"] = state.lockoutRemainingSec;
  doc["totalOnSec"] = state.totalOnSec;
  doc["totalLockoutSec"] = state.totalLockoutSec;

  File f = LittleFS.open("/state.json", "w");
  if (!f) return false;
  serializeJson(doc, f);
  f.close();
  return true;
}
