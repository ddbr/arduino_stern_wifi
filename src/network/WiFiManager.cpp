#include "WiFiManager.h"

#include <Arduino.h>
#include <MDNS_Generic.h>

void WiFiManager::begin() { connect(); }

void WiFiManager::connect() {
  if (WiFi.status() == WL_CONNECTED) return;

  Serial.print("Verbinde mit ");
  Serial.println(config::WIFI_SSID);

  WiFi.begin(config::WIFI_SSID, config::WIFI_PASSWORD);
}

void WiFiManager::loop() {
  if (WiFi.status() != WL_CONNECTED && millis() - lastAttempt > 5000) {
    lastAttempt = millis();
    connect();
  }

  if (WiFi.status() == WL_CONNECTED && !MDNS.isRunning()) {
    if (MDNS.begin(config::DEVICE_NAME)) {
      MDNS.setInstanceName("Arduino Stern");
      Serial.println("mDNS gestartet");
    } else {
      Serial.println("mDNS konnte nicht gestartet werden");
    }
  }
}

bool WiFiManager::isConnected() const { return WiFi.status() == WL_CONNECTED; }

