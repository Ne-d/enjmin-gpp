#include "Projectile.hpp"

#include "C.hpp"
#include "Enemy.hpp"
#include "Game.hpp"
#include "Math.hpp"

Projectile::Projectile(const Vector2f position, const Vector2f velocity, const float damage)
	:
	Entity(position.x, position.y, RectangleShape({ C::GRID_SIZE / 2, C::GRID_SIZE / 2 })),
	damage(damage) {
	type = EntityType::Projectile;
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

void Projectile::die() {
	shouldDie = true;
}

void Projectile::update() {
	Entity::update();

	updatePosition();

	if (Game::instance->hasCollision(cx, cy))
		die();

	const auto optionalEntityHit = Game::instance->hasCollisionWithEnemy(cx + rx, cy + ry);
	if (optionalEntityHit && optionalEntityHit.value()->type == EntityType::Enemy) {
		auto* enemy = (Enemy*)optionalEntityHit.value();
		enemy->takeDamage(damage, sign(dx) * 0.5);
		die();
	}
}

void Projectile::syncShape() {
	Entity::syncShape();

	const auto size = shape.getSize();
	shape.setPosition(((cx + rx) * C::GRID_SIZE) - size.x / 2, ((cy + ry) * C::GRID_SIZE) - size.y / 2);
}

void Projectile::draw() const {
	Entity::draw();
}
