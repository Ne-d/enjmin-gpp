#include "Player.hpp"

#include "C.hpp"
#include "Enemy.hpp"
#include "Game.hpp"
#include "imgui.h"

Player::Player(const float x, const float y)
	:
	Character(x, y),
	shootTimer(200ms) {
}

void Player::update() {
	pollInput();

	Character::update();

	if (moveInput > 0)
		lastDirection = 1;
	if (moveInput < 0)
		lastDirection = -1;
}

void Player::pollInput() {
	Game* const game = Game::instance;
	moveInput = 0;

	// Movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		moveInput -= 1;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		moveInput += 1;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isOnGround) {
		dy = -jumpImpulse;
	}

	// Pew pew
	if (!Game::instance->isEditingLevel && !ImGui::GetIO().WantCaptureMouse) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && shootTimer.isFinished()) {
			shootTimer.start();

			const float projectileX = cx + rx + (lastDirection == 1 ? 1 : 0.5);
			const float projectileY = cy + ry - 0.75;

			game->addProjectile(new Projectile({ projectileX, projectileY }, { (float)lastDirection * 2, 0 }, 1));
		}
	}
}