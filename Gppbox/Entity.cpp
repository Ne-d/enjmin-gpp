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
