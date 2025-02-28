#pragma once

#include "Entity.hpp"

class Drone : public Entity {
public:
	// Methods
	Drone(float x, float y);

	void update() override;
	void syncShape() override;
	void draw();

	void shoot();

	// Data
	float maxSpeed = 1;
	float acceleration = 0.01;
	float friction = 0.98;
	float teleportDistance = 50.0f;
	float projectileSpeed = 4;

	sf::Vector2f lastShootDirection;

	sf::CircleShape muzzleFlashShape;
	float muzzleFlashSize = 10;
	float muzzleFlashShrinkSpeed = 1.5f;
};
