#pragma once

#include <WiFiS3.h>

#include "../effects/EffectManager.h"
#include "../network/WiFiManager.h"

class SternWebServer {
 public:
  SternWebServer(EffectManager &effects, EffectState &state);
  void begin();
  void loop();

 private:
  void handleClient(WiFiClient &client);
  void sendResponse(WiFiClient &client, int code, const char *contentType, const char *body);
  void sendResponseProgmem(WiFiClient &client, int code, const char *contentType, const char *bodyProgmem);

  void handleStateGet(WiFiClient &client);
  void handleStatePost(WiFiClient &client, const String &body);

  WiFiServer server;
  EffectManager &effectManager;
  EffectState &effectState;
};
