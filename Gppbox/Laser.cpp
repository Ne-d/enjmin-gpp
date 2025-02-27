#include "Laser.hpp"

#include <iostream>

#include "C.hpp"
#include "Enemy.hpp"
#include "Game.hpp"

Laser::Laser(const sf::Vector2f position, const int direction)
	:
	Entity(position.x, position.y, sf::RectangleShape{ { 1, 1 } }),
	direction(direction),
	finishTimer(2500ms),
	damageTimer(250ms) {
	finishTimer.start();
	shape.setFillColor(sf::Color::Green);
}

void Laser::update() {
	Entity::update();

	const Game* game = Game::instance;

	cx = game->player->cx + (direction == 1 ? 1 : 0);
	rx = game->player->rx;

	cy = game->player->cy - 1;
	ry = game->player->ry - 0.5;

	syncShape();

	if (finishTimer.isFinished())
		shouldDie = true;

	if (damageTimer.isFinished()) {
		damageEnemies();
		damageTimer.start();
	}
}

void Laser::syncShape() {
	Entity::syncShape();

	const Game* game = Game::instance;

	for (int x = 0; x < C::RESOLUTION_X / C::GRID_SIZE; ++x) {
		if (direction == 1 && (game->hasCollision(cx + x, cy) || game->hasCollision(cx + x, cy + 1))) {
			length = x;
			break;
		}

		if (direction == -1 && (game->hasCollision(cx - x, cy) || game->hasCollision(cx - x, cy + 1))) {
			length = x;
			break;
		}
	}

	if (direction == 1)
		length -= rx;

	if (direction == -1)
		length = -length - rx + 1;

	shape.setPosition(
		sf::Vector2f{ cx + rx, cy + ry } * (float)C::GRID_SIZE
	);
	shape.setSize(sf::Vector2f{ length, 0.5f } * (float)C::GRID_SIZE);
}

void Laser::damageEnemies() {
	const Game* game = Game::instance;

	for (auto* e : game->entities) {
		if (e->type == EntityType::Enemy) {
			auto* const enemy = (Enemy*)e;
			const Vector2f enemyPosition = { enemy->cx + enemy->rx, enemy->cy + enemy->ry };

			if (direction == 1 && enemyPosition.x > cx + rx && enemyPosition.x < cx + rx + length
				&& enemyPosition.y >= cy + ry && enemyPosition.y <= cy + ry + 2.0f) {
				enemy->takeDamage(1, direction);
			}

			if (direction == -1 && enemyPosition.x < cx + rx && enemyPosition.x > cx + rx + length
				&& enemyPosition.y >= cy + ry && enemyPosition.y <= cy + ry + 2.0f) {
				enemy->takeDamage(1, direction);
			}
		}
	}
}
