﻿#pragma once

#include "Entity.hpp"

class Character : public Entity {
public:
	explicit Character(float x, float y, int width, int height);

	void takeDamage(float damage, float recoil);
	
	bool im() override;

protected:
	// Methods
	void update() override;
	void syncShape() override;

	// Data
	float moveInput;
	
	float moveSpeed = 0.3f;
	float jumpImpulse = 1.5f;

	float accelerationFactor = 0.15f;

	float frictionX = 0.80f;
	float frictionY = 0.91f;

	float gravity = 0.075f;

	float health = 1;
};
