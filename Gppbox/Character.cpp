#include "Character.hpp"

#include "Game.hpp"

Character::Character(float x, float y, const sf::RectangleShape& entityShape)
	:
	Entity(x, y, entityShape) {
}

void Character::update() {
	// Only apply movement if target speed is faster than current speed
	// Otherwise apply friction
	const float targetSpeed = moveInput * moveSpeed;
	if (abs(targetSpeed) > abs(dx))
		dx += moveInput * accelerationFactor * moveSpeed;
	else {
		dx *= pow(frictionX, Game::instance->deltaFrame);
	}
	
	dy += gravity * Game::instance->deltaFrame;
	dy *= pow(frictionY, Game::instance->deltaFrame);

	Entity::update();
}
