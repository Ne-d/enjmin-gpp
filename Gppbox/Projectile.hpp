#pragma once

#include "Entity.hpp"

class Projectile : public Entity {
public:
	// Methods
	Projectile(sf::Vector2f position, sf::Vector2f velocity, float damage);
	void updatePosition();

	void update() override;
	void syncShape() override;
	void draw() const override;

	virtual void die();

	float damage = 1;
};
