#include "WiFiManager.h"

#include <Arduino.h>

void WiFiManager::begin() { connect(); }

void WiFiManager::connect() {
  if (WiFi.status() == WL_CONNECTED) return;

  Serial.print("Verbinde mit ");
  Serial.println(config::WIFI_SSID);

  WiFi.begin(config::WIFI_SSID, config::WIFI_PASSWORD);

  // kurzer Block bis der erste Verbindungsversuch abgeschlossen ist
  const unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 8000) {
    delay(250);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.print("Verbunden, IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println();
    Serial.println("Verbindung fehlgeschlagen");
  }
}

void WiFiManager::loop() {
  if (WiFi.status() != WL_CONNECTED && millis() - lastAttempt > 5000) {
    lastAttempt = millis();
    connect();
  }
}

bool WiFiManager::isConnected() const { return WiFi.status() == WL_CONNECTED; }

