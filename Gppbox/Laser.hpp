#pragma once
#include "Entity.hpp"
#include "Timer.h"

class Laser : public Entity {
public:
	// Methods
	Laser(sf::Vector2f position, int direction);
	~Laser() override = default;

	void update() override;
	void syncShape() override;
	void damageEnemies() const;

	// Data
	const int direction;

	float length = 0;

	Timer finishTimer;
	Timer damageTimer;
};
