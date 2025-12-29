#include "OtaHandler.h"

#include <Arduino.h>

void OtaHandler::begin() {
  ArduinoOTA.setPort(config::OTA_PORT);
  ArduinoOTA.setHostname(config::DEVICE_NAME);
  ArduinoOTA.setPassword(config::OTA_PASSWORD);

  ArduinoOTA.onStart([]() { Serial.println("Starte OTA Update"); });
  ArduinoOTA.onEnd([]() { Serial.println("OTA Update beendet"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("OTA Fortschritt: %u%%\n", (progress * 100) / total);
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("OTA Fehler[%u]\n", error);
  });

  ArduinoOTA.begin();
  Serial.println("ArduinoOTA bereit");
}

void OtaHandler::loop() { ArduinoOTA.handle(); }

