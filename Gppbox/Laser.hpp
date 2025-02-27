﻿#pragma once
#include "Entity.hpp"
#include "Timer.h"

class Laser : public Entity {
public:
	// Methods
	Laser(sf::Vector2f position, int direction);

	void update() override;
	void syncShape() override;
	void damageEnemies();

	// Data
	const int direction;

	float length = 0;

	Timer finishTimer;
	Timer damageTimer;
};
