#pragma once

#include <memory>

#include "Effect.h"

std::unique_ptr<Effect> createSolidEffect();
std::unique_ptr<Effect> createPulseEffect();
std::unique_ptr<Effect> createTwinkleEffect();

