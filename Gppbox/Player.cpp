#include "Player.hpp"

Player::Player(const float x, const float y)
	:
	Character(x, y) {
}

void Player::update() {
	pollInput();

	Character::update();
}

void Player::pollInput() {
	moveInput = 0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		moveInput -= 1;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		moveInput += 1;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isOnGround) {
		dy = -jumpImpulse;
	}
}