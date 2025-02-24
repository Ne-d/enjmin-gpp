#pragma once
#include "Character.hpp"

class Enemy : public Character {
public:
	explicit Enemy(float x, float y);

	void update() override;
};
