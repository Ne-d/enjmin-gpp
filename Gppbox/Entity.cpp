#include "C.hpp"

#include "Entity.hpp"

#include "imgui-SFML.h"
#include "imgui.h"

Entity::Entity(const float x, const float y, sf::RectangleShape shape)
	:
	shape(std::move(shape)) {
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
		(cx + rx) * C::GRID_SIZE,
		(cy + ry) * C::GRID_SIZE
	);
}

bool Entity::hasCollision(int x, int y) {
	if (x < 0 || y < 0 || x > C::RESOLUTION_X * C::GRID_SIZE || y > C::RESOLUTION_Y * C::GRID_SIZE)
		return true;

	return false;
}

void Entity::syncShape() {
	shape.setPosition(
		(cx + rx - 0.5) * C::GRID_SIZE,
		(cy + ry - 2) * C::GRID_SIZE
	);
}

void Entity::fixGridPosition() {
	while (rx >= 1) {
		cx++;
		rx--;
	}
	while (rx < 0) {
		cx--;
		rx++;
	}

	while (ry >= 1) {
		cy++;
		ry--;
	}
	while (ry < 0) {
		cy--;
		ry++;
	}
}

void Entity::update(const double deltaTime) {
	const double rate = 1.0 / deltaTime;
	const double deltaFrame = 60 / rate;
	
	rx += dx * deltaFrame;
	ry += dy * deltaFrame;

	fixGridPosition();
	syncShape();
}

bool Entity::im() {
	ImGui::Value("cx", cx);
	ImGui::Value("cy", cy);
	
	ImGui::Value("rx", rx);
	ImGui::Value("ry", ry);
	
	ImGui::Value("dx", dx);
	ImGui::Value("dy", dy);

	sf::Vector2i pixelPosition = getPixelPosition();
	bool pixelChanged = false;
	pixelChanged |= ImGui::DragInt2("Pixel Position", &pixelPosition.x, 1.0f, -9999, 9999);
	if(pixelChanged) setPixelPosition(pixelPosition.x, pixelPosition.y);

	sf::Vector2f gridPosition = {cx + rx, cy + ry};
	bool gridChanged = false;
	gridChanged |= ImGui::DragFloat2("Grid Position", &gridPosition.x, 0.1f, -9999, 9999);
	if(gridChanged) setGridPosition(gridPosition.x, gridPosition.y);

	sf::Vector2f velocity = {dx, dy};
	bool velocityChanged = false;
	velocityChanged |= ImGui::DragFloat2("Velocity", &velocity.x, 0.1f, -0.5, 0.5);
	if(velocityChanged) setGridVelocity(velocity.x, velocity.y);

	return pixelChanged || gridChanged || velocityChanged;
}
