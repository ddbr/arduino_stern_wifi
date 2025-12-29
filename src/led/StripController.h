#pragma once

#include <algorithm>
#include <Adafruit_NeoPixel.h>
#include <array>
#include <vector>

#include "config.h"

struct ColorState {
  uint8_t r{255};
  uint8_t g{200};
  uint8_t b{180};
  uint8_t w{0};
  uint8_t brightness{config::LED_BRIGHTNESS};
};

class StripController {
 public:
  StripController();
  void begin();
  void setColor(uint16_t index, const ColorState &color);
  void setAll(const ColorState &color);
  void show();
  uint16_t size() const { return ledCount; }

 private:
  uint32_t applyCalibration(const ColorState &color) const;
  Adafruit_NeoPixel strip;
  const uint16_t ledCount{config::LED_COUNT};
  const std::array<float, 4> calibration{config::CALIBRATION_R, config::CALIBRATION_G,
                                         config::CALIBRATION_B, config::CALIBRATION_W};
  const std::vector<uint16_t> ledMap;
};

