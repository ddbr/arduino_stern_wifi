#include "Effect.h"

class SolidEffect : public Effect {
 public:
  SolidEffect() : Effect("solid") {}
  void update(StripController &strip, uint32_t /*deltaMs*/, const EffectState &state) override {
    ColorState color = state.color;
    color.brightness = state.brightness;
    if (state.isOn) {
      strip.setAll(color);
    } else {
      ColorState off{};
      off.r = off.g = off.b = off.w = 0;
      off.brightness = 0;
      strip.setAll(off);
    }
    strip.show();
  }
};

std::unique_ptr<Effect> createSolidEffect() { return std::make_unique<SolidEffect>(); }

