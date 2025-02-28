#include "C.hpp"

#include "Entity.hpp"

#include "Game.hpp"
#include "imgui-SFML.h"
#include "imgui.h"

Entity::Entity(const float x, const float y, sf::RectangleShape shape)
	:
	shape(std::move(shape)),
	type(EntityType::Other) {
	setGridPosition(x, y);
}

// TODO: This ain't bussin at all fr fr
void Entity::setPixelPosition(const int x, const int y) {
	cx = x / C::GRID_SIZE;
	cy = y / C::GRID_SIZE;

	rx = (x - (cx * C::GRID_SIZE)) / (float)C::GRID_SIZE;
	ry = (y - (cy * C::GRID_SIZE)) / (float)C::GRID_SIZE;

	syncShape();
}

void Entity::setGridPosition(const float x, const float y) {
	cx = (int)x;
	rx = x - cx;

	cy = (int)y;
	ry = y - cy;

	syncShape();
}

void Entity::setGridVelocity(const float x, const float y) {
	dx = x;
	dy = y;
}

sf::Vector2i Entity::getPixelPosition() const {
	return sf::Vector2i(
		(int)(cx + rx) * C::GRID_SIZE,
		(int)(cy + ry) * C::GRID_SIZE
	);
}

void Entity::updatePositionWithCollision() {
	const Game* game = Game::instance;

	// Integrate position based on velocity
	rx += dx * game->deltaFrame;
	ry += dy * game->deltaFrame;

	// Collisions
	constexpr float collisionThresholdX = 0.0f;

	isOnLeftWall = false;
	// X(-) Movement collisions
	do {
		if (game->hasCollision(cx - 1, cy, collisionWidth, collisionHeight) && rx <= collisionThresholdX) {
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
		if (game->hasCollision(cx + 1, cy, collisionWidth, collisionHeight) && rx >= collisionThresholdX) {
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


void Entity::syncShape() {
}

void Entity::update() {
}

bool Entity::im() {
	if (ImGui::CollapsingHeader("Entity", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Value("cx", cx);
		ImGui::SameLine();
		ImGui::Value("| cy", cy);

		ImGui::Value("rx", rx);
		ImGui::SameLine();
		ImGui::Value("| ry", ry);

		ImGui::Value("dx", dx);
		ImGui::SameLine();
		ImGui::Value("| dy", dy);

		sf::Vector2i pixelPosition = getPixelPosition();
		bool pixelChanged = false;
		pixelChanged |= ImGui::DragInt2("Pixel Position", &pixelPosition.x, 1.0f, -9999, 9999);
		if (pixelChanged)
			setPixelPosition(pixelPosition.x, pixelPosition.y);

		sf::Vector2f gridPosition = { cx + rx, cy + ry };
		bool gridChanged = false;
		gridChanged |= ImGui::DragFloat2("Grid Position", &gridPosition.x, 0.1f, -9999, 9999);
		if (gridChanged)
			setGridPosition(gridPosition.x, gridPosition.y);

		sf::Vector2f velocity = { dx, dy };
		bool velocityChanged = false;
		velocityChanged |= ImGui::DragFloat2("Velocity", &velocity.x, 0.1f, -1, 1);
		if (velocityChanged)
			setGridVelocity(velocity.x, velocity.y);

		return pixelChanged || gridChanged || velocityChanged;
	}

	return false;
}
