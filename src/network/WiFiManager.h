#pragma once

#include <WiFiS3.h>

#include "config.h"

class WiFiManager {
 public:
  void begin();
  void loop();
  bool isConnected() const;

 private:
  void connect();
  unsigned long lastAttempt{0};
};

