#pragma once

#include <ArduinoOTA.h>

#include "config.h"

class OtaHandler {
 public:
  void begin();
  void loop();
};

