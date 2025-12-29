#pragma once

#include <Arduino.h>
#include <map>
#include <memory>

#include "Effect.h"

class EffectManager {
 public:
  void registerEffect(std::unique_ptr<Effect> effect);
  void setActiveEffect(const String &name);
  String activeEffectName() const;
  void update(StripController &strip, uint32_t deltaMs, EffectState &state);
  std::vector<String> availableEffects() const;

 private:
  std::map<String, std::unique_ptr<Effect>> effects;
  Effect *active{nullptr};
};

