#include "Enemy.hpp"

#include "Game.hpp"

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
