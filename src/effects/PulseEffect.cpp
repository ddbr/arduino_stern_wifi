#include "Effect.h"

class PulseEffect : public Effect {
 public:
  PulseEffect() : Effect("pulse") {}
  void update(StripController &strip, uint32_t deltaMs, const EffectState &state) override {
    static float phase = 0.0f;
    const float speedFactor = (state.speed / 255.0f) * 0.01f;  // slow to fast
    phase += deltaMs * speedFactor;
    float intensity = (sin(phase) + 1.0f) * 0.5f;  // 0..1

    ColorState color = state.color;
    color.brightness = static_cast<uint8_t>(state.brightness * intensity);

    if (!state.isOn) {
      color.brightness = 0;
    }

    strip.setAll(color);
    strip.show();
  }
};

std::unique_ptr<Effect> createPulseEffect() { return std::make_unique<PulseEffect>(); }

