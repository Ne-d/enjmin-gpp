#include "Player.hpp"

#include <cmath>
#include <iostream>

#include "C.hpp"
#include "Enemy.hpp"
#include "Game.hpp"
#include "imgui.h"
#include "Laser.hpp"
#include "Math.hpp"
#include "Missile.hpp"

Player::Player(const float x, const float y)
	:
	Character(x, y, 1, 2),
	shootTimer(200ms),
	missileTimer(2000ms),
	laserTimer(5000ms) {
	type = EntityType::Player;
	shootTimer.skip();
	missileTimer.skip();
	laserTimer.skip();

	muzzleFlashShape.setFillColor(Color(255, 255, 64));
}

void Player::update() {
	pollInput();

	Character::update();

	if (moveInput > 0)
		lastDirection = 1;
	if (moveInput < 0)
		lastDirection = -1;

	if (muzzleFlashShape.getRadius() > 0)
		muzzleFlashShape.setRadius(muzzleFlashShape.getRadius() - muzzleFlashShrinkSpeed * Game::instance->deltaFrame);
}

void Player::shoot() {
	Game* const game = Game::instance;
	shootTimer.start();

	const float projectileX = cx + rx + (lastDirection == 1 ? 1 : 0.5);
	const float projectileY = cy + ry - 1;

	game->addProjectile(new Projectile(
			{ projectileX, projectileY },
			{ (float)lastDirection * projectileSpeed + random(-0.1, 0.1), random(-0.1, 0.1) }, 1
		)
	);
	
	dx -= lastDirection * 0.1;
	game->camera.addScreenShake(5, { (float)-lastDirection, 0 }, 75ms);

	muzzleFlashShape.setRadius(muzzleFlashSize);
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
	game->camera.addScreenShake(20, { 0, 0 }, 2500ms);
}

void Player::syncShape() {
	Character::syncShape();

	const float x = (cx + rx + (lastDirection == 1 ? 0.5f : -1.5f));
	const float y = (cy + ry - 1.0f - (muzzleFlashShape.getRadius() / C::GRID_SIZE));

	muzzleFlashShape.setPosition(x * C::GRID_SIZE, y * C::GRID_SIZE);
}

void Player::draw() const {
	const Game* const game = Game::instance;

	if (muzzleFlashShape.getRadius() > 0)
		game->win->draw(muzzleFlashShape);

	game->win->draw(shape);
}

std::optional<Enemy*> Player::findClosestTarget() const {
	Entity* closestEntity = nullptr;

	const Vector2f position = { cx + rx, cy + ry };
	float minDistance = std::numeric_limits<float>::max();
	bool found = false;
	for (auto* const entity : Game::instance->entities) {
		if (entity->type != EntityType::Enemy)
			continue;

		found = true;
		const Vector2f entityPosition = { entity->cx + entity->rx, entity->cy + entity->ry };
		const float dist = distance(position, entityPosition);

		if (dist < minDistance) {
			minDistance = dist;
			closestEntity = entity;
		}
	}

	if (found)
		return std::make_optional((Enemy*)closestEntity);

	return std::nullopt;
}

void Player::pollInput() {
	moveInput = 0;

	// Controller Movement
	float const stickValue = Joystick::getAxisPosition(0, Joystick::Axis::X) / 100.0f;
	constexpr float stickDeadzone = 0.2f;

	if (std::abs(stickValue) > stickDeadzone)
		moveInput = lerpF(stickDeadzone * sign(stickValue), 1.0f, stickValue);

	// Keyboard Movement
	if (Keyboard::isKeyPressed(Keyboard::Q))
		moveInput -= 1;

	if (Keyboard::isKeyPressed(Keyboard::D))
		moveInput += 1;

	if ((Keyboard::isKeyPressed(Keyboard::Space) || Joystick::isButtonPressed(0, 0)) && isOnGround) {
		dy = -jumpImpulse;
	}

	// Normalise moveInput to avoid accumulation between controller and keyboard.
	moveInput = std::clamp(moveInput, -1.0f, 1.0f);

	// Pew pew
	if (!Game::instance->isEditingLevel && !ImGui::GetIO().WantCaptureMouse) {
		constexpr float triggerDeadzone = 50.0f;
		
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
