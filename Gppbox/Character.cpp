#include "Character.hpp"

#include "C.hpp"
#include "Game.hpp"
#include "imgui.h"

Character::Character(const float x, const float y)
	:
	Entity(x, y, RectangleShape({ C::GRID_SIZE, 2 * C::GRID_SIZE })),
	moveInput(0) {
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

	updatePosition();
	
	Entity::update();
}

void Character::updatePosition() {
	const Game* game = Game::instance;

	// Integrate position based on velocity
	rx += dx * game->deltaFrame;
	ry += dy * game->deltaFrame;

	// Collisions
	constexpr float collisionThresholdX = 0.0f;

	isOnLeftWall = false;
	// X(-) Movement collisions
	do {
		if (game->hasCollision(cx - 1, cy, collisionWidth, collisionHeight + 1) && rx <= collisionThresholdX) {
			rx = collisionThresholdX;
			dx = 0.0f;
			isOnLeftWall = true;
		}
		if (rx < 0.0f) {
			cx--;
			rx++;
		}
	}
	while (rx < 0.0f);

	isOnRightWall = false;
	// X(+) Movement collisions
	do {
		if (game->hasCollision(cx + 1, cy, collisionWidth, collisionHeight + 1) && rx >= collisionThresholdX) {
			rx = collisionThresholdX;
			dx = 0.0f;
			isOnRightWall = true;
		}
		if (rx > 1.0f) {
			cx++;
			rx--;
		}
	}
	while (rx > 1.0f);

	// Y(-) Movement collisions
	do {
		if (game->hasCollision(cx, cy - 2, collisionWidth + 1, collisionHeight) && ry < 0.0f) {
			ry = 0.0f;
			dy = 0.0f;
		}

		isOnGround = false;

		if (ry < 0.0f) {
			cy--;
			ry++;
		}
	}
	while (ry < 0.0f);

	// Y(+) Movement collisions
	do {
		if (game->hasCollision(cx, cy + 1, collisionWidth + 1, collisionHeight) && ry >= 0.99f) {
			ry = 0.99f;
			dy = 0.0f;
			isOnGround = true;
		}
		else
			isOnGround = false;

		if (ry > 1.0f) {
			cy++;
			ry--;
		}
	}
	while (ry > 1.0f);

	syncShape();
}

void Character::syncShape() {
	Entity::syncShape();

	shape.setPosition(
		(cx + rx) * C::GRID_SIZE,
		(cy + ry - 2) * C::GRID_SIZE
	);
}

bool Character::im() {
	const bool entityChanged = Entity::im();

	if (ImGui::CollapsingHeader("Character", ImGuiTreeNodeFlags_DefaultOpen)) {
		bool characterChanged = false;

		characterChanged |= ImGui::DragFloat("Move Speed", &moveSpeed, 0.0005f, 0.0f, 10.0f);

		characterChanged |= ImGui::DragFloat("Jump Impulse", &jumpImpulse, 0.001f, 0.0f, 10.0f);

		characterChanged |= ImGui::DragFloat("Acceleration", &accelerationFactor, 0.0001f, 0.0f, 10.0f);

		Vector2f friction = sf::Vector2f(frictionX, frictionY);
		characterChanged |= ImGui::DragFloat2("Friction", &friction.x, 0.0001f, 0.0f, 10.0f);

		characterChanged |= ImGui::DragFloat("Gravity", &gravity, 0.0001f, 0.0f, 10.0f);

		if (characterChanged)
			return true;
	}

	return entityChanged;
}
