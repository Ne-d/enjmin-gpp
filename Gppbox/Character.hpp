#pragma once

#include "Entity.hpp"

class Character : public Entity {
public:
	explicit Character(float x, float y);
	bool im() override;

protected:
	// Methods
	void update() override;
	void updatePosition();
	virtual void syncShape() override;

	// Input Data
	float moveInput;

	// Movement Data
	float moveSpeed = 0.3f;
	float jumpImpulse = 1.5f;

	float accelerationFactor = 0.15f;

	float frictionX = 0.80f;
	float frictionY = 0.91f;

	float gravity = 0.075f;
};
