#include "Player.hpp"

#include <cmath>
#include <iostream>

#include "Enemy.hpp"
#include "Game.hpp"
#include "imgui.h"
#include "Laser.hpp"
#include "Math.hpp"
#include "Missile.hpp"

Player::Player(const float x, const float y)
	:
	Character(x, y),
	shootTimer(200ms),
	missileTimer(2000ms),
	laserTimer(5000ms) {
	type = EntityType::Player;
	shootTimer.skip();
	missileTimer.skip();
	laserTimer.skip();
}

void Player::update() {
	pollInput();

	Character::update();

	if (moveInput > 0)
		lastDirection = 1;
	if (moveInput < 0)
		lastDirection = -1;
}

void Player::shoot() {
	Game* const game = Game::instance;
	shootTimer.start();

	const float projectileX = cx + rx + (lastDirection == 1 ? 1 : 0.5);
	const float projectileY = cy + ry - 0.75;

	game->addProjectile(new Projectile({ projectileX, projectileY }, { (float)lastDirection * 2, 0 }, 1));
	dx -= lastDirection * 0.1;
	game->camera.addScreenShake(3, { (float)-lastDirection, 0 }, 50ms);
}

void Player::shootMissile() {
	Game* const game = Game::instance;
	missileTimer.start();

	const float missileX = cx + rx + (lastDirection == 1 ? 1 : 0.5);
	const float missileY = cy + ry - 0.75;

	game->addProjectile(new Missile({ missileX, missileY }));
}

void Player::shootLaser() {
	Game* const game = Game::instance;
	laserTimer.start();

	const float laserX = cx + rx + (lastDirection == 1 ? 1 : 0.5);
	const float laserY = cy + ry - 2;

	game->entities.emplace_back(new Laser({ laserX, laserY }, lastDirection));
}

void Player::pollInput() {
	moveInput = 0;

	// Controller Movement
	float const stickValue = sf::Joystick::getAxisPosition(0, Joystick::Axis::X) / 100.0f;
	constexpr float stickDeadzone = 0.2f;

	if (std::abs(stickValue) > stickDeadzone)
		moveInput = lerp(stickDeadzone * sign(stickValue), 1.0f, stickValue);

	// Keyboard Movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		moveInput -= 1;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		moveInput += 1;

	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Joystick::isButtonPressed(0, 0)) && isOnGround) {
		dy = -jumpImpulse;
	}

	// Normalise moveInput to avoid accumulation between controller and keyboard.
	moveInput = std::clamp(moveInput, -1.0f, 1.0f);

	// Pew pew
	constexpr float triggerDeadzone = 50.0f;
	if (!Game::instance->isEditingLevel && !ImGui::GetIO().WantCaptureMouse) {
		if ((Mouse::isButtonPressed(Mouse::Left) || Joystick::getAxisPosition(0, Joystick::Axis::Z) < -triggerDeadzone)
			&& shootTimer.isFinished()) {
			shoot();
		}

		if ((Mouse::isButtonPressed(Mouse::Right) || Joystick::getAxisPosition(0, Joystick::Axis::Z) > triggerDeadzone)
			&& missileTimer.isFinished()) {
			shootMissile();
		}

		if ((Mouse::isButtonPressed(Mouse::Middle) || Joystick::isButtonPressed(0, 3)) && laserTimer.isFinished()) {
			shootLaser();
		}
	}
}
