#include "Enemy.hpp"

#include "Game.hpp"

Enemy::Enemy(float x, float y)
	:
	Character(x, y) {
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
