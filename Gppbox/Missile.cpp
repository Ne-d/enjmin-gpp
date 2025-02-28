#include "Missile.hpp"

#include <algorithm>

#include "C.hpp"
#include "Game.hpp"
#include "Math.hpp"

Missile::Missile(const sf::Vector2f position)
	:
	Projectile(position, { 0, 0 }, 10) {
	findTarget();
	dx = 0;
	dy = -1;

	explosionShape.setFillColor(Color(255, 128, 0));
}

void Missile::findTarget() {
	const std::optional<Enemy*> optionalTarget = Game::instance->player->findClosestTarget();

	if (optionalTarget)
		target = *optionalTarget;
}

void Missile::update() {
	if (!exploded) {
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
	else {
		explosionShape.setRadius(explosionShape.getRadius() - explosionShrinkSpeed * Game::instance->deltaFrame);
		explosionShape.setPosition(
			(cx + rx) * C::GRID_SIZE - explosionShape.getRadius(),
			(cy + ry) * C::GRID_SIZE - explosionShape.getRadius()
		);

		if (explosionShape.getRadius() <= 0) {
			Projectile::die();
		}
	}
}

void Missile::die() {
	explosionShape.setPosition((cx + rx) * C::GRID_SIZE - explosionSize, (cy + ry) * C::GRID_SIZE - explosionSize);
	explosionShape.setRadius(explosionSize);

	exploded = true;
}

void Missile::draw() const {
	const Game* const game = Game::instance;

	if (exploded)
		game->win->draw(explosionShape);

	else
		game->win->draw(shape);
}
