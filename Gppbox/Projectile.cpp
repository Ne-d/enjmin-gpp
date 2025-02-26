#include "Projectile.hpp"

#include "C.hpp"
#include "Game.hpp"

Projectile::Projectile(const sf::Vector2f position, const sf::Vector2f velocity, const float damage)
	:
	Entity(position.x, position.y, sf::RectangleShape({ C::GRID_SIZE / 2, C::GRID_SIZE / 2 })),
	damage(damage) {
	dx = velocity.x;
	dy = velocity.y;
}

void Projectile::updatePosition() {
	rx += dx;
	while (rx >= 1) {
		cx++;
		rx--;
	}
	while (rx < 0) {
		rx++;
		cx--;
	}

	ry += dy;
	while (ry >= 1) {
		cy++;
		ry--;
	}
	while (ry < 0) {
		ry++;
		cy--;
	}
}

void Projectile::update() {
	Entity::update();

	updatePosition();

	if (Game::instance->hasCollision(cx, cy))
		shouldDie = true;

	syncShape();
}

void Projectile::syncShape() {
	Entity::syncShape();

	const auto size = shape.getSize();
	shape.setPosition(cx * C::GRID_SIZE + rx - size.x / 2, cy * C::GRID_SIZE + ry - size.y / 2);
}
