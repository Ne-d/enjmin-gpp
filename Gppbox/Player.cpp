#include "Player.hpp"

#include <utility>

Player::Player(const float x, const float y, sf::RectangleShape entityShape)
	:
	Entity(x, y, std::move(entityShape)) {
}

void Player::update(const double deltaTime) {
	pollInput();
	move(deltaTime);

	//Entity::update(deltaTime);
}

void Player::pollInput() {
	moveInput = 0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		moveInput -= 1;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		moveInput += 1;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		jumping = true;
		dy = -2;
	}
}

void Player::move(const double deltaTime) {
	const double rate = 1.0 / deltaTime;
	const double deltaFrame = 60 / rate;

	// Only apply movement if target speed is faster than current speed
	// Otherwise apply friction
	const float targetSpeed = moveInput * moveSpeed;
	if (abs(targetSpeed) > abs(dx))
		dx = moveInput * moveSpeed;
	else {
		dx *= pow(frx, deltaFrame);
		dy *= pow(fry, deltaFrame);
	}

	updatePosition(deltaFrame);

	syncShape();
}
