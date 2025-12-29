#pragma once

#include <WebServer.h>

#include "../effects/EffectManager.h"
#include "../network/WiFiManager.h"
#include "index_html.h"
#include "style_css.h"
#include "app_js.h"

class SternWebServer {
 public:
  SternWebServer(EffectManager &effects, EffectState &state);
  void begin();
  void loop();

 private:
  void registerRoutes();
  void handleStateGet();
  void handleStatePost();

  WebServer server;
  EffectManager &effectManager;
  EffectState &effectState;
};

