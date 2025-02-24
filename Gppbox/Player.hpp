#pragma once

#include "Character.hpp"
#include "Entity.hpp"

class Player : public Character {
public:
	explicit Player(float x, float y);

	void update() override;

private:
	// Methods
	void pollInput();
};
