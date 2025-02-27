#pragma once

#include "Projectile.hpp"

class Missile : public Projectile {
public:
	// Methods
	explicit Missile(sf::Vector2f position);

	void findTarget();
	void update() override;

	// Data
	Entity* target;
	sf::Vector2f targetPosition;

	float turnRate = 0.01f;
	float turnRateIncrease = 0.0005f;

	float targetSpeed = 0.75f;
};
