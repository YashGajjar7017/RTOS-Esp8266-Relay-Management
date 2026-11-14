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
  char username[24];
  char password[34];
  char role[20];     // "admin", "guarded_operator", "guest_analytics"
  char status[12];   // "active" or "pending"
  char created[16];
};

struct OneTimeAccessRequest {
  char requestId[20];
  char username[24];
  char requestedAction[32]; // e.g. "TRIGGER_RELAY", "SAVE_CONFIG"
  unsigned long requestTime;
  bool active;
  bool approved;
  char oneTimeToken[32];
};

struct MonthlyAnalytics {
  char currentMonthYear[16];  // e.g. "November 2026"
  uint32_t totalOnSeconds;    // Total seconds relay was active this month
  uint32_t totalOffSeconds;   // Total seconds relay was inactive/locked this month
  uint32_t totalCyclesCount;  // Total times relay was turned on this month
  uint32_t lastMonthOnSeconds;
  uint32_t lastMonthOffSeconds;
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

  // Operational State Persistence
  bool loadState(PersistentState& state);
  bool saveState(const PersistentState& state);

  // Monthly Analytics
  bool loadAnalytics(MonthlyAnalytics& analytics);
  bool saveAnalytics(const MonthlyAnalytics& analytics);
  void recordStateSecond(bool relayOn);
  void recordCycleTriggered();
  const MonthlyAnalytics& getAnalytics() const { return _analytics; }

  // User Database Operations (3 Predefined Users + Slots)
  int getUserCount();
  bool getUser(int index, UserRecord& user);
  bool findUser(const char* username, UserRecord& user);
  bool verifyCredentials(const char* username, const char* password, UserRecord& matchedUser);
  bool verifyAdminPassword(const char* adminPassword);
  
  // Registration & User Management
  bool registerUserWithAdminApproval(const char* username, const char* password, const char* role, const char* adminPassword, String& errorMsg);
  bool setUserStatus(const char* targetUsername, const char* newStatus, const char* adminPassword, String& errorMsg);
  bool resetUserPassword(const char* targetUsername, const char* recoveryKey, const char* newPassword, String& errorMsg);
  bool deleteUser(uint8_t id, String& errorMsg);

  // Level 2: One-Time Access Grant for Mahi_Gajjar
  bool createOneTimeRequest(const char* username, const char* action, String& requestId, String& errorMsg);
  bool hasActiveOneTimeRequest() const { return _oneTimeReq.active && !_oneTimeReq.approved; }
  const OneTimeAccessRequest& getOneTimeRequest() const { return _oneTimeReq; }
  bool approveOneTimeRequest(const char* adminPassword, String& oneTimeToken, String& errorMsg);
  bool consumeOneTimeToken(const char* token);
  void rejectOneTimeRequest();

  // JSON Export helpers
  String getUsersJson();
  String getAnalyticsJson();
  String getPendingRequestJson();

private:
  UserRecord _users[MAX_USERS];
  int _userCount;
  bool _fsMounted;
  MonthlyAnalytics _analytics;
  OneTimeAccessRequest _oneTimeReq;

  void seedPredefinedUsers();
  bool loadUsersFromFile();
  bool saveUsersToFile();
};

extern StorageDB storageDB;
