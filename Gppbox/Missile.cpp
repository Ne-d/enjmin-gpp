#include "Missile.hpp"

#include <algorithm>

#include "Game.hpp"
#include "Math.hpp"

Missile::Missile(const sf::Vector2f position)
	:
	Projectile(position, { 0, 0 }, 10) {
	findTarget();
	dx = 0;
	dy = -1;
}

void Missile::findTarget() {
	const std::optional<Enemy*> optionalTarget = Game::instance->player->findClosestTarget();

	if (optionalTarget)
		target = *optionalTarget;
}

void Missile::update() {
	if (target == nullptr)
		findTarget();

	if (target != nullptr) {
		const Vector2f position = { cx + rx, cy + ry };
		targetPosition = { target->cx + target->rx, target->cy + target->ry - 1.5f };
		const Vector2f direction = normalize(targetPosition - position);

		Vector2f velocity = { dx, dy };
		const Vector2f desiredVelocity = direction * targetSpeed;
		const Vector2f velocityChange = (desiredVelocity - velocity) * turnRate;
		velocity += velocityChange;

		dx = velocity.x;
		dy = velocity.y;

		turnRate = lerpF(turnRate, 1, turnRateIncrease);
	}

	Projectile::update();
}
