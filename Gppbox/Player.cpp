#include "Player.hpp"

#include <utility>

Player::Player(const float x, const float y, sf::RectangleShape entityShape)
	:
	Entity(x, y, std::move(entityShape)) {
}

void Player::update(const double deltaTime) {
	Entity::update(deltaTime);

	pollInput();
	move(deltaTime);
}

void Player::pollInput() {
	moveInput = 0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		moveInput -= 1;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		moveInput += 1;
}

void Player::move(const double deltaTime) {
	const double rate = 1.0 / deltaTime;
	const double deltaFrame = 60 / rate;

	// Only apply movement if target speed is faster than current speed
	// Otherwise apply friction
	if (abs(moveInput * moveSpeed) > abs(dx))
		dx = moveInput * moveSpeed;
	else {
		dx *= pow(frx, deltaFrame);
		dy *= pow(fry, deltaFrame);
	}

	// Collisions
	if (hasCollision(cx + 1, cy) && rx > 0.5) {
		rx = 0.5;
		dx = 0;
	}
	if (hasCollision(cx - 1, cy) && rx < 0.5) {
		rx = 0.5;
		dx = 0;
	}

	fixGridPosition();
}
