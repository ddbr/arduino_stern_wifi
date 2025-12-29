#include "StripController.h"

StripController::StripController()
    : strip(ledCount, config::LED_PIN, NEO_GRBW + NEO_KHZ800),
      ledMap{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12} {}

void StripController::begin() {
  strip.begin();
  strip.setBrightness(config::LED_BRIGHTNESS);
  strip.show();
}

uint32_t StripController::applyCalibration(const ColorState &color) const {
  auto clamp = [](float value) { return static_cast<uint8_t>(std::min(255.0f, std::max(0.0f, value))); };
  uint8_t calibratedR = clamp(color.r * calibration[0]);
  uint8_t calibratedG = clamp(color.g * calibration[1]);
  uint8_t calibratedB = clamp(color.b * calibration[2]);
  uint8_t calibratedW = clamp(color.w * calibration[3]);
  return strip.Color(calibratedR, calibratedG, calibratedB, calibratedW);
}

void StripController::setColor(uint16_t index, const ColorState &color) {
  if (index >= ledMap.size()) return;
  strip.setBrightness(color.brightness);
  strip.setPixelColor(ledMap[index], applyCalibration(color));
}

void StripController::setAll(const ColorState &color) {
  strip.setBrightness(color.brightness);
  for (uint16_t i = 0; i < ledCount; ++i) {
    setColor(i, color);
  }
}

void StripController::show() { strip.show(); }

