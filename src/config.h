#pragma once

#include <Arduino.h>

namespace config {
// WiFi credentials (set via build flags or edit here)
static constexpr const char *WIFI_SSID = "YOUR_WIFI_SSID";
static constexpr const char *WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// Device identity
static constexpr const char *DEVICE_NAME = "Stern";
static constexpr const char *OTA_PASSWORD = "stern-ota";

// LED strip wiring
static constexpr uint8_t LED_PIN = 6;              // D6 on UNO R4 WiFi
static constexpr uint16_t LED_COUNT = 13;          // 12 spokes + 1 center
static constexpr uint8_t LED_BRIGHTNESS = 180;     // Default brightness (0-255)

// Timing
static constexpr uint16_t WEB_SERVER_PORT = 80;
static constexpr uint16_t OTA_PORT = 3232;

// Calibration per color channel (0.0f - 1.0f)
static constexpr float CALIBRATION_R = 1.00f;
static constexpr float CALIBRATION_G = 0.92f;
static constexpr float CALIBRATION_B = 0.98f;
static constexpr float CALIBRATION_W = 1.00f;
}  // namespace config
