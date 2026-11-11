#include "rtos_tasks.h"

RTOSTaskScheduler rtosScheduler;

RTOSTaskScheduler::RTOSTaskScheduler() : _taskCount(0) {
  memset(_tasks, 0, sizeof(_tasks));
}

void RTOSTaskScheduler::begin() {
  Serial.printf("[RTOS] Scheduler started with %d registered tasks.\n", _taskCount);
}

void RTOSTaskScheduler::registerTask(const char* name, unsigned long intervalMs, void (*func)()) {
  if (_taskCount >= MAX_TASKS) return;
  _tasks[_taskCount].name = name;
  _tasks[_taskCount].intervalMs = intervalMs;
  _tasks[_taskCount].lastRunMillis = 0;
  _tasks[_taskCount].taskFunction = func;
  _tasks[_taskCount].enabled = true;
  _taskCount++;
}

void RTOSTaskScheduler::run() {
  unsigned long currentMillis = millis();

  for (int i = 0; i < _taskCount; i++) {
    if (_tasks[i].enabled && _tasks[i].taskFunction) {
      if (currentMillis - _tasks[i].lastRunMillis >= _tasks[i].intervalMs) {
        _tasks[i].lastRunMillis = currentMillis;
        _tasks[i].taskFunction();
      }
    }
  }

  // Yield to allow ESP8266 background WiFi stack to execute without WDT triggers
  yield();
}

