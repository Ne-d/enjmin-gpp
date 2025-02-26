#pragma once

#include "Character.hpp"

class Player : public Character {
public:
	explicit Player(float x, float y);

	void update() override;

private:
	// Methods
	void pollInput();

	// Data
	int lastDirection = 1;
};
