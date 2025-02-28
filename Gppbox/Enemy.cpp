#include "Enemy.hpp"

#include <iostream>

#include "C.hpp"
#include "Game.hpp"
#include "Math.hpp"

Enemy::Enemy(const float x, const float y)
	:
	Character(x, y, 1, 2) {
	type = EntityType::Enemy;
	health = 5;
	moveInput = -1;
	shape.setFillColor(Color::Red);
}

void Enemy::update() {
	if (isOnLeftWall)
		moveInput = 1;

	if (isOnRightWall)
		moveInput = -1;

	Character::update();
}

void Enemy::takeDamage(const float damage, const float recoil) {
	Character::takeDamage(damage, recoil);

	if (shouldDie) {
		for (int y = 0; y < 8; ++y) {
			for (int x = 0; x < 4; ++x) {
				Particle part;
				part.x = (cx + rx) * C::GRID_SIZE + 4 * x;
				part.y = (cy + ry) * C::GRID_SIZE - 4 * y;
				part.dx = random(-300.0f, 300.0f);
				part.dy = -100 - abs(random(0, 200.0));

				constexpr float friction = 0.3f;
				constexpr float gravity = 500.0f;
				part.bhv = [](Particle* lthis, float dt) {
					const float scaleRate = random(1.0f, 3.0f);
					lthis->scaleX -= scaleRate * dt;
					lthis->scaleY -= scaleRate * dt;

					lthis->dy += gravity * dt;

					lthis->dx *= pow(friction, dt);
					lthis->dy *= pow(friction, dt);
				};

				Game::instance->afterParts.add(part);
			}
		}
	}
}
