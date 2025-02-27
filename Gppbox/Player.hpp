#pragma once

#include "Character.hpp"
#include "Timer.h"

class Player : public Character {
public:
	explicit Player(float x, float y);

	void update() override;
	void shoot();
	void shootMissile();

private:
	// Methods
	void pollInput();

	// Data
	int lastDirection = 1;
	Timer shootTimer;
	Timer missileTimer;
};
