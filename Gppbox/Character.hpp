#pragma once

#include "Entity.hpp"

class Character : public Entity {
public:
	explicit Character(float x, float y, const sf::RectangleShape& entityShape);

protected:
	// Methods
	void update() override;

	// Input Data
	float moveInput;

	// Gameplay Data
	float moveSpeed = 0.5f;
	float jumpImpulse = 2.0f;

	float frictionX = 0.91f;
	float frictionY = 0.91f;

	float gravity = 0.1f;
};
