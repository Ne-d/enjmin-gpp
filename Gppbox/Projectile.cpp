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
	const Game* game = Game::instance;

	rx += dx * game->deltaFrame;
	while (rx < 0.0f) {
		cx--;
		rx++;
	}
	while (rx > 1.0f) {
		cx++;
		rx--;
	}

	ry += dy * game->deltaFrame;
	while (ry < 0.0f) {
		cy--;
		ry++;
	}
	while (ry > 1.0f) {
		cy++;
		ry--;
	}

	syncShape();
}

void Projectile::update() {
	Entity::update();

	updatePosition();

	if (Game::instance->hasCollision(cx, cy))
		shouldDie = true;
}

void Projectile::syncShape() {
	Entity::syncShape();

	const auto size = shape.getSize();
	shape.setPosition(((cx + rx) * C::GRID_SIZE) - size.x / 2, ((cy + ry) * C::GRID_SIZE) - size.y / 2);
}
