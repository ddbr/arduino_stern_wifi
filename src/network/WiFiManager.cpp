#include "WiFiManager.h"
#include <Arduino.h>
#include <MDNS_Generic.h>
#include "../config.h" 

void WiFiManager::begin() { connect(); }

void WiFiManager::connect() {
  if (WiFi.status() == WL_CONNECTED) return;

  Serial.print("Verbinde mit ");
  Serial.println(config::WIFI_SSID);

  WiFi.begin(config::WIFI_SSID, config::WIFI_PASSWORD);

  // kurze Blockade bis zum ersten Verbindungsversuch abgeschlossen ist,
  // damit OTA/Webserver konsistent starten
  const unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 8000) {
    delay(250);
  }
}

void WiFiManager::loop() {
  if (WiFi.status() != WL_CONNECTED && millis() - lastAttempt > 5000) {
    lastAttempt = millis();
    connect();
  }

  if (WiFi.status() == WL_CONNECTED && !MDNS.isRunning()) {
    if (MDNS.begin(config::DEVICE_NAME)) {
      MDNS.setInstanceName("Arduino Stern");
      MDNS.addService("http", "tcp", config::WEB_SERVER_PORT);
      Serial.println("mDNS gestartet");
    } else {
      Serial.println("mDNS konnte nicht gestartet werden");
    }
  }

  if (MDNS.isRunning()) {
    MDNS.update();
  }
}

bool WiFiManager::isConnected() const { return WiFi.status() == WL_CONNECTED; }

