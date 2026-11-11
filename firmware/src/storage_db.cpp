#include "storage_db.h"
#include <LittleFS.h>
#include <ArduinoJson.h>

StorageDB storageDB;

StorageDB::StorageDB() : _userCount(0), _fsMounted(false) {
  memset(_users, 0, sizeof(_users));
  memset(&_pendingRequest, 0, sizeof(_pendingRequest));
  _pendingRequest.active = false;
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
  if (!loadUsersFromFile() || _userCount == 0) {
    seedDefaultAdmin();
    saveUsersToFile();
  }

  return true;
}

void StorageDB::seedDefaultAdmin() {
  _userCount = 1;
  _users[0].id = 1;
  strncpy(_users[0].username, DEFAULT_ADMIN_USERNAME, sizeof(_users[0].username) - 1);
  strncpy(_users[0].password, DEFAULT_ADMIN_PASSWORD, sizeof(_users[0].password) - 1);
  strncpy(_users[0].role, "admin", sizeof(_users[0].role) - 1);
  strncpy(_users[0].status, "active", sizeof(_users[0].status) - 1);
  strncpy(_users[0].created, "2026-11-10", sizeof(_users[0].created) - 1);
  Serial.println(F("[DB] Initialized Master Admin account in DB."));
}

bool StorageDB::loadUsersFromFile() {
  if (!_fsMounted || !LittleFS.exists("/users.json")) {
    return false;
  }

  File f = LittleFS.open("/users.json", "r");
  if (!f) return false;

  StaticJsonDocument<1024> doc;
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
    strncpy(_users[_userCount].created, u["created"] | "2026-11-10", sizeof(_users[_userCount].created) - 1);
    _userCount++;
  }

  Serial.printf("[DB] Loaded %d user(s) from Flash DB.\n", _userCount);
  return true;
}

bool StorageDB::saveUsersToFile() {
  if (!_fsMounted) return false;

  StaticJsonDocument<1024> doc;
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

bool StorageDB::registerUserWithAdminApproval(const char* username, const char* password, const char* role, const char* adminPassword, String& errorMsg) {
  // 1. Verify capacity
  if (_userCount >= MAX_USERS) {
    errorMsg = "Database capacity full (Max 5 users)";
    return false;
  }

  // 2. Verify Master Admin Password
  if (!verifyAdminPassword(adminPassword)) {
    errorMsg = "Invalid Admin Master Password. Authorization rejected.";
    return false;
  }

  // 3. Verify uniqueness
  UserRecord dummy;
  if (findUser(username, dummy)) {
    errorMsg = "Username already exists";
    return false;
  }

  // 4. Enroll new user
  uint8_t nextId = 1;
  for (int i = 0; i < _userCount; i++) {
    if (_users[i].id >= nextId) nextId = _users[i].id + 1;
  }

  _users[_userCount].id = nextId;
  strncpy(_users[_userCount].username, username, sizeof(_users[_userCount].username) - 1);
  strncpy(_users[_userCount].password, password, sizeof(_users[_userCount].password) - 1);
  strncpy(_users[_userCount].role, (role && strcmp(role, "admin") == 0) ? "admin" : "operator", sizeof(_users[_userCount].role) - 1);
  strncpy(_users[_userCount].status, "active", sizeof(_users[_userCount].status) - 1);
  strncpy(_users[_userCount].created, "2026-11-11", sizeof(_users[_userCount].created) - 1);
  _userCount++;

  saveUsersToFile();
  Serial.printf("[DB] Authorized and enrolled user: %s (Role: %s)\n", username, role);
  return true;
}

bool StorageDB::submitLoginRequest(const char* username, const char* password, const char* role, String& errorMsg) {
  if (_userCount >= MAX_USERS) {
    errorMsg = "Database capacity full (Max 5 users)";
    return false;
  }

  UserRecord dummy;
  if (findUser(username, dummy)) {
    errorMsg = "Username already exists in database. Try standard login.";
    return false;
  }

  strncpy(_pendingRequest.username, username, sizeof(_pendingRequest.username) - 1);
  strncpy(_pendingRequest.password, password, sizeof(_pendingRequest.password) - 1);
  strncpy(_pendingRequest.role, (role && strcmp(role, "admin") == 0) ? "admin" : "operator", sizeof(_pendingRequest.role) - 1);
  _pendingRequest.timestamp = millis();
  _pendingRequest.active = true;

  Serial.printf("[DB] New user login request queued for: %s (Awaiting Admin Password Approval)\n", username);
  return true;
}

bool StorageDB::approvePendingRequest(const char* adminPassword, String& errorMsg) {
  if (!_pendingRequest.active) {
    errorMsg = "No pending login request to approve";
    return false;
  }

  bool success = registerUserWithAdminApproval(
    _pendingRequest.username,
    _pendingRequest.password,
    _pendingRequest.role,
    adminPassword,
    errorMsg
  );

  if (success) {
    _pendingRequest.active = false;
  }
  return success;
}

void StorageDB::rejectPendingRequest() {
  _pendingRequest.active = false;
  Serial.println(F("[DB] Pending login request was dismissed/rejected by Admin."));
}

bool StorageDB::isUserActive(const char* username) {
  UserRecord u;
  if (findUser(username, u)) {
    return strcmp(u.status, "active") == 0;
  }
  return false;
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

  if (strcmp(_users[targetIndex].role, "admin") == 0) {
    errorMsg = "Cannot delete Master Administrator";
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
  StaticJsonDocument<1024> doc;
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
