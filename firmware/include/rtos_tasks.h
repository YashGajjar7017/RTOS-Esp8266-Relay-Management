#pragma once
#include <Arduino.h>

/**
 * =========================================================================
 *              RTOS TASK CONTROLLER & SCHEDULER DECLARATIONS
 * =========================================================================
 * Defines prioritized RTOS tasks for real-time responsiveness:
 * 1. TaskTouchInput: High-frequency polling and debouncing of TTP223 capacitive touch sensor
 * 2. TaskCountdownManager: 1 Hz clock tick, duty cycle expiration & lockout decrements
 * 3. TaskNetworkServer: Serves REST API requests & Captive Portal DNS queries
 * 4. TaskWatchdogKeepalive: Hardware watchdog pet and diagnostic telemetries
 */

struct RTOSTask {
  const char* name;
  unsigned long intervalMs;
  unsigned long lastRunMillis;
  void (*taskFunction)();
  bool enabled;
};

class RTOSTaskScheduler {
public:
  RTOSTaskScheduler();
  void begin();
  void run();

  // Task registrations
  void registerTask(const char* name, unsigned long intervalMs, void (*func)());

private:
  static const int MAX_TASKS = 6;
  RTOSTask _tasks[MAX_TASKS];
  int _taskCount;
};

extern RTOSTaskScheduler rtosScheduler;

