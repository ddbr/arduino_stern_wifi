#include <Arduino.h>
#include <memory>

#include "config.h"
#include "effects/EffectFactory.h"
#include "effects/EffectManager.h"
#include "led/StripController.h"
//#include "network/OtaHandler.h"
#include "network/WiFiManager.h"
#include "web/WebServer.h"

WiFiManager wifiManager;
OtaHandler otaHandler;
StripController stripController;
EffectManager effectManager;
EffectState effectState;
std::unique_ptr<SternWebServer> webServer;

unsigned long lastFrame = 0;

void setup() {
  Serial.begin(115200);
  stripController.begin();

  effectManager.registerEffect(createSolidEffect());
  effectManager.registerEffect(createPulseEffect());
  effectManager.registerEffect(createTwinkleEffect());
  effectManager.setActiveEffect(effectState.name);

  wifiManager.begin();
  otaHandler.begin();

  webServer = std::make_unique<SternWebServer>(effectManager, effectState);
  webServer->begin();

  lastFrame = millis();
}

void loop() {
  wifiManager.loop();
  otaHandler.loop();
  webServer->loop();

  unsigned long now = millis();
  uint32_t delta = now - lastFrame;
  lastFrame = now;

  effectManager.update(stripController, delta, effectState);
}

