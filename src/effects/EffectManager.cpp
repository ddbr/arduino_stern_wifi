#include "EffectManager.h"

void EffectManager::registerEffect(std::unique_ptr<Effect> effect) {
  String key = effect->getName();
  if (!effects.count(key)) {
    effects[key] = std::move(effect);
    if (!active) {
      active = effects[key].get();
    }
  }
}

bool EffectManager::setActiveEffect(const String &name) {
  auto it = effects.find(name);
  if (it != effects.end()) {
    active = it->second.get();
    return true;
  }
  return false;
}

String EffectManager::activeEffectName() const { return active ? active->getName() : ""; }

void EffectManager::update(StripController &strip, uint32_t deltaMs, EffectState &state) {
  if (!active) return;
  active->update(strip, deltaMs, state);
}

std::vector<String> EffectManager::availableEffects() const {
  std::vector<String> names;
  names.reserve(effects.size());
  for (const auto &pair : effects) {
    names.push_back(pair.first);
  }
  return names;
}

