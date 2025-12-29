#include "WebServer.h"

#include <Arduino.h>
#include <ArduinoJson.h>

#include "../config.h"
#include "index_html.h"
#include "style_css.h"
#include "app_js.h"

static String readLine(WiFiClient &c) {
  String line;
  while (c.connected()) {
    if (!c.available()) { delay(1); continue; }
    char ch = (char)c.read();
    if (ch == '\r') continue;
    if (ch == '\n') break;
    line += ch;
  }
  return line;
}

static String readN(WiFiClient &c, int n) {
  String s;
  s.reserve(n);
  while (c.connected() && (int)s.length() < n) {
    if (!c.available()) { delay(1); continue; }
    s += (char)c.read();
  }
  return s;
}

SternWebServer::SternWebServer(EffectManager &effects, EffectState &state)
    : server(config::WEB_SERVER_PORT), effectManager(effects), effectState(state) {}

void SternWebServer::begin() {
  server.begin();
}

void SternWebServer::loop() {
  WiFiClient client = server.available();
  if (!client) return;
  handleClient(client);
  client.stop();
}

void SternWebServer::sendResponse(
    WiFiClient &client,
    int code,
    const char *contentType,
    const char *body
) {
  // Map HTTP status code to reason phrase
  const char *statusText =
      (code == 200) ? "OK" :
      (code == 400) ? "Bad Request" :
      (code == 404) ? "Not Found" :
      (code == 405) ? "Method Not Allowed" :
      "OK";

  // Status line
  client.print("HTTP/1.1 ");
  client.print(code);
  client.print(" ");
  client.print(statusText);
  client.print("\r\n");

  // Headers
  client.print("Content-Type: ");
  client.print(contentType);
  client.print("\r\n");

  client.print("Connection: close\r\n");

  client.print("Content-Length: ");
  client.print(strlen(body));
  client.print("\r\n");

  // End of headers
  client.print("\r\n");

  // Body
  client.print(body);
}


void SternWebServer::sendResponseProgmem(WiFiClient &client, int code, const char *contentType, const char *bodyProgmem) {
  // On non-AVR (like Uno R4), PROGMEM is basically normal flash; strlen_P may not exist.
  // If your index_html.h defines plain C strings, strlen() works.
  sendResponse(client, code, contentType, bodyProgmem);
}

void SternWebServer::handleClient(WiFiClient &client) {
  // Request line: "GET /path HTTP/1.1"
  String reqLine = readLine(client);
  if (reqLine.length() == 0) return;

  // Headers
  int contentLength = 0;
  String line;
  while (true) {
    line = readLine(client);
    if (line.length() == 0) break; // blank line ends headers
    if (line.startsWith("Content-Length:")) {
      contentLength = line.substring(strlen("Content-Length:")).toInt();
    }
  }

  // Parse method + path
  int sp1 = reqLine.indexOf(' ');
  int sp2 = reqLine.indexOf(' ', sp1 + 1);
  if (sp1 < 0 || sp2 < 0) {
    sendResponse(client, 400, "text/plain", "Bad request");
    return;
  }
  String method = reqLine.substring(0, sp1);
  String path = reqLine.substring(sp1 + 1, sp2);

  String body;
  if (method == "POST" && contentLength > 0) {
    body = readN(client, contentLength);
  }

  // Routing
  if (method == "GET" && path == "/") {
    sendResponseProgmem(client, 200, "text/html", INDEX_HTML);
    return;
  }
  if (method == "GET" && path == "/style.css") {
    sendResponseProgmem(client, 200, "text/css", STYLE_CSS);
    return;
  }
  if (method == "GET" && path == "/app.js") {
    sendResponseProgmem(client, 200, "application/javascript", APP_JS);
    return;
  }

  if (path == "/api/state") {
    if (method == "GET") {
      handleStateGet(client);
      return;
    }
    if (method == "POST") {
      handleStatePost(client, body);
      return;
    }
    sendResponse(client, 405, "text/plain", "Method Not Allowed");
    return;
  }

  sendResponse(client, 404, "text/plain", "Not found");
}

void SternWebServer::handleStateGet(WiFiClient &client) {
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

  // Proper JSON response
  client.print("HTTP/1.1 200 OK\r\n");
  client.print("Content-Type: application/json\r\n");
  client.print("Connection: close\r\n");
  client.print("Content-Length: ");
  client.print(strlen(response.c_str()));
  client.print("\r\n");
  client.print("\r\n");
  client.print(response);
}

void SternWebServer::handleStatePost(WiFiClient &client, const String &body) {
  DynamicJsonDocument doc(512);
  DeserializationError err = deserializeJson(doc, body);
  if (err) {
    sendResponse(client, 400, "application/json", "{\"error\":\"invalid json\"}");
    return;
  }

  effectState.isOn = doc["isOn"] | effectState.isOn;
  effectState.brightness = constrain((int)(doc["brightness"] | effectState.brightness), 0, 255);
  effectState.speed = constrain((int)(doc["speed"] | effectState.speed), 0, 255);

  String requestedEffect = (const char*)(doc["effect"] | effectState.name.c_str());

  JsonObject color = doc["color"].as<JsonObject>();
  if (!color.isNull()) {
    effectState.color.r = constrain((int)(color["r"] | effectState.color.r), 0, 255);
    effectState.color.g = constrain((int)(color["g"] | effectState.color.g), 0, 255);
    effectState.color.b = constrain((int)(color["b"] | effectState.color.b), 0, 255);
    effectState.color.w = constrain((int)(color["w"] | effectState.color.w), 0, 255);
  }

  if (!effectManager.setActiveEffect(requestedEffect)) {
    sendResponse(client, 400, "application/json", "{\"error\":\"unknown effect\"}");
    return;
  }
  effectState.name = effectManager.activeEffectName();

  sendResponse(client, 200, "application/json", "{\"status\":\"ok\"}");
}
