#include "Math.hpp"

#include <random>

float lerp(const float a, const float b, const float f) {
	return a * (1.0f - f) + (b * f);
}

float sign(const float f) {
	if (f > 0.0f)
		return 1.0f;

	if (f < 0.0f)
		return -1.0f;

	return 0.0f;
}

float random(const float min, const float max) {
	static std::default_random_engine randomEngine;
	static std::uniform_real_distribution<> distribution(min, max);
	return distribution(randomEngine);
}
