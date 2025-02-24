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

	Entity::update();
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
