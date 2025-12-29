#include "WebServer.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFiS3.h>

#include "../config.h"

SternWebServer::SternWebServer(EffectManager &effects, EffectState &state)
    : server(config::WEB_SERVER_PORT), effectManager(effects), effectState(state) {}

void SternWebServer::begin() {
  registerRoutes();
  server.begin();
}

void SternWebServer::loop() { server.handleClient(); }

void SternWebServer::registerRoutes() {
  server.on("/", HTTP_GET, [this]() { server.send_P(200, "text/html", INDEX_HTML); });
  server.on("/style.css", HTTP_GET, [this]() { server.send_P(200, "text/css", STYLE_CSS); });
  server.on("/app.js", HTTP_GET, [this]() { server.send_P(200, "application/javascript", APP_JS); });

  server.on("/api/state", HTTP_GET, [this]() { handleStateGet(); });
  server.on("/api/state", HTTP_POST, [this]() { handleStatePost(); });

  server.onNotFound([this]() { server.send(404, "text/plain", "Not found"); });
}

void SternWebServer::handleStateGet() {
  DynamicJsonDocument doc(512);
  doc["isOn"] = effectState.isOn;
  doc["brightness"] = effectState.brightness;
  doc["speed"] = effectState.speed;
  doc["effect"] = effectState.name;

  JsonObject color = doc.createNestedObject("color");
  color["r"] = effectState.color.r;
  color["g"] = effectState.color.g;
  color["b"] = effectState.color.b;
  color["w"] = effectState.color.w;

  JsonArray effects = doc.createNestedArray("effects");
  for (const auto &name : effectManager.availableEffects()) {
    effects.add(name);
  }

  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void SternWebServer::handleStatePost() {
  DynamicJsonDocument doc(512);
  DeserializationError err = deserializeJson(doc, server.arg("plain"));
  if (err) {
    server.send(400, "application/json", "{\"error\":\"invalid json\"}");
    return;
  }

  effectState.isOn = doc["isOn"].as<bool>();
  effectState.brightness = constrain(doc["brightness"].as<int>(), 0, 255);
  effectState.speed = constrain(doc["speed"].as<int>(), 0, 255);
  String requestedEffect = doc["effect"].as<String>();

  JsonObject color = doc["color"].as<JsonObject>();
  effectState.color.r = constrain(color["r"].as<int>(), 0, 255);
  effectState.color.g = constrain(color["g"].as<int>(), 0, 255);
  effectState.color.b = constrain(color["b"].as<int>(), 0, 255);
  effectState.color.w = constrain(color["w"].as<int>(), 0, 255);

  if (!effectManager.setActiveEffect(requestedEffect)) {
    server.send(400, "application/json", "{\"error\":\"unknown effect\"}");
    return;
  }

  effectState.name = effectManager.activeEffectName();

  server.send(200, "application/json", "{\"status\":\"ok\"}");
}

