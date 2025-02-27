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

float distanceSquared(const sf::Vector2f a, const sf::Vector2f b) {
	const sf::Vector2f diff = a - b;
	return lengthSquared(diff);
}

float distance(const sf::Vector2f a, const sf::Vector2f b) {
	const sf::Vector2f diff = a - b;
	return length(diff);
}

float lengthSquared(const sf::Vector2f a) {
	return a.x * a.x + a.y * a.y;
}

float length(const sf::Vector2f a) {
	return sqrtf(lengthSquared(a));
}

sf::Vector2f normalize(const sf::Vector2f a) {
	return a / length(a);
}
