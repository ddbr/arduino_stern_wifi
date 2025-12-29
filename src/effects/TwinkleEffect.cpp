#include "Effect.h"

class TwinkleEffect : public Effect {
 public:
  TwinkleEffect() : Effect("twinkle") {}
  void update(StripController &strip, uint32_t deltaMs, const EffectState &state) override {
    static uint32_t accumulator = 0;
    accumulator += deltaMs;

    if (accumulator > 50) {
      accumulator = 0;
      for (uint16_t i = 0; i < strip.size(); ++i) {
        ColorState pixelColor = state.color;
        pixelColor.brightness = state.isOn ? random(0, state.brightness + 1) : 0;
        strip.setColor(i, pixelColor);
      }
      strip.show();
    }
  }
};

std::unique_ptr<Effect> createTwinkleEffect() { return std::make_unique<TwinkleEffect>(); }

