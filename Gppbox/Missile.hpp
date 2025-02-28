#pragma once

#include "Projectile.hpp"

class Missile : public Projectile {
public:
	// Methods
	explicit Missile(sf::Vector2f position);
	~Missile() override = default;

	void findTarget();
	void update() override;
	void die() override;

	void draw() const override;

	// Data
	Entity* target;
	sf::Vector2f targetPosition;

	float turnRate = 0.01f;
	float turnRateIncrease = 0.0005f;

	float targetSpeed = 0.75f;

	bool exploded = false;
	sf::CircleShape explosionShape;
	float explosionSize = 64;
	float explosionShrinkSpeed = 1.5f;
};
