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
	Entity* closestEntity = nullptr;

	const Vector2f position = { cx + rx, cy + ry };
	float minDistance = std::numeric_limits<float>::max();
	for (auto* const entity : Game::instance->entities) {
		if (entity->type != EntityType::Enemy)
			continue;

		const Vector2f entityPosition = { entity->cx + entity->rx, entity->cy + entity->ry };
		const float dist = distance(position, entityPosition);

		minDistance = std::min(dist, minDistance);
		closestEntity = entity;
	}

	target = closestEntity;
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

		turnRate = lerp(turnRate, 1, turnRateIncrease);
	}

	Projectile::update();
}
