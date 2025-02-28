#pragma once

#include <optional>

#include "Character.hpp"
#include "Enemy.hpp"
#include "Timer.h"

class Player : public Character {
public:
	explicit Player(float x, float y);

	void update() override;
	void shoot();
	void shootMissile();
	void shootLaser();

	void syncShape() override;
	void draw();

	std::optional<Enemy*> findClosestTarget();

private:
	// Methods
	void pollInput();

	// Data
	int lastDirection = 1;
	Timer shootTimer;
	Timer missileTimer;
	Timer laserTimer;
	int projectileSpeed = 2;

	sf::CircleShape muzzleFlashShape;
	float muzzleFlashSize = 16;
	float muzzleFlashShrinkSpeed = 1.5f;
};
