#pragma once

#include <Arduino.h>

#include "../led/StripController.h"

struct EffectState {
  bool isOn{true};
  ColorState color{};
  uint8_t brightness{config::LED_BRIGHTNESS};
  uint8_t speed{128};
  String name{"solid"};
};

class Effect {
 public:
  explicit Effect(String effectName) : name(std::move(effectName)) {}
  virtual ~Effect() = default;

  virtual void update(StripController &strip, uint32_t deltaMs, const EffectState &state) = 0;
  const String &getName() const { return name; }

 protected:
  String name;
};

