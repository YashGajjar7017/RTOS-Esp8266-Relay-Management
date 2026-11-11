#pragma once
#include <Arduino.h>
#include "config.h"

enum SystemMode : uint8_t {
  MODE_IDLE = 0,
  MODE_RELAY_ON = 1,
  MODE_LOCKOUT = 2
};

struct UserRecord {
  uint8_t id;
  char username[20];
  char password[34];
  char role[12];     // "admin" or "operator"
  char status[12];   // "active" or "pending"
  char created[16];
};

struct PendingLoginRequest {
  char username[20];
  char password[34];
  char role[12];
  unsigned long timestamp;
  bool active;
};

struct SystemSettings {
  float onHours;
  float lockoutDays;
};

struct PersistentState {
  SystemMode mode;
  uint32_t onRemainingSec;
  uint32_t lockoutRemainingSec;
  uint32_t totalOnSec;
  uint32_t totalLockoutSec;
};

class StorageDB {
public:
  StorageDB();
  bool begin();

  // Settings Management
  bool loadSettings(SystemSettings& settings);
  bool saveSettings(const SystemSettings& settings);

  // Operational State Persistence (Crash & Power-Cut Protection)
  bool loadState(PersistentState& state);
  bool saveState(const PersistentState& state);

  // User Database Operations (Max 5 users)
  int getUserCount();
  bool getUser(int index, UserRecord& user);
  bool findUser(const char* username, UserRecord& user);
  bool verifyCredentials(const char* username, const char* password, UserRecord& matchedUser);
  bool verifyAdminPassword(const char* adminPassword);
  
  // Registration with Admin Approval
  bool registerUserWithAdminApproval(const char* username, const char* password, const char* role, const char* adminPassword, String& errorMsg);
  bool deleteUser(uint8_t id, String& errorMsg);

  // Pending User Login Request Queue (Real-Time Admin Approval Flow)
  bool submitLoginRequest(const char* username, const char* password, const char* role, String& errorMsg);
  bool hasPendingRequest() const { return _pendingRequest.active; }
  const PendingLoginRequest& getPendingRequest() const { return _pendingRequest; }
  bool approvePendingRequest(const char* adminPassword, String& errorMsg);
  void rejectPendingRequest();
  bool isUserActive(const char* username);

  // JSON Export helper
  String getUsersJson();
  String getSettingsJson(const SystemSettings& settings);

private:
  UserRecord _users[MAX_USERS];
  int _userCount;
  bool _fsMounted;
  PendingLoginRequest _pendingRequest;

  void seedDefaultAdmin();
  bool loadUsersFromFile();
  bool saveUsersToFile();
};

extern StorageDB storageDB;
