#pragma once
#include "Character.hpp"

class Enemy : public Character {
public:
	explicit Enemy(float x, float y);

	void update() override;
	void takeDamage(float damage, float recoil) override;;
};
