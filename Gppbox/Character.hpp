#pragma once

#include "Entity.hpp"

class Character : public Entity {
public:
	explicit Character(float x, float y, const sf::RectangleShape& entityShape);

protected:
	// Methods
	void update() override;
	virtual bool im() override;

	// Input Data
	float moveInput;

	// Movement Data
	float moveSpeed = 0.3f;
	float jumpImpulse = 1.5f;

	float accelerationFactor = 0.15f;

	float frictionX = 0.80f;
	float frictionY = 0.91f;

	float gravity = 0.09f;
};
