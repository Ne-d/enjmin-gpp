#pragma once

#include "Entity.hpp"

class Drone : public Entity {
public:
	// Methods
	Drone(float x, float y);

	void update() override;
	void syncShape() override;
	void shoot();

	// Data
	float maxSpeed = 1;
	float acceleration = 0.01;
	float friction = 0.98;
	float teleportDistance = 50.0f;
	float projectileSpeed = 4;
};
