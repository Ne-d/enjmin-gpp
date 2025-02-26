#include "Math.hpp"

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
