#include "Drone.hpp"


#include "C.hpp"
#include "Game.hpp"
#include "Math.hpp"

Drone::Drone(const float x, const float y)
	:
	Entity(x, y, sf::RectangleShape{ { 1 * C::GRID_SIZE, 1 * C::GRID_SIZE } }) {
	type = EntityType::Drone;
	collisionWidth = 1;
	collisionHeight = 1;
}

void Drone::update() {
	Entity::update();

	const Game* const game = Game::instance;
	const Player* const player = game->player;

	// Calculate movement to follow player
	const Vector2f position = { cx + rx, cy + ry };
	const Vector2f targetPosition = { player->cx + player->rx, player->cy + player->ry - 7 };
	const Vector2f direction = normalize(targetPosition - position);
	const Vector2f desiredVelocity = direction * maxSpeed;

	dx = lerpF(dx, desiredVelocity.x, acceleration);
	dy = lerpF(dy, desiredVelocity.y, acceleration);

	// Teleport to player if too far away
	float distanceToPlayer = distance(position, targetPosition);
	if (distanceToPlayer > teleportDistance)
		setGridPosition(targetPosition.x, targetPosition.y);


	// Apply movement
	dx *= friction;
	dy *= friction;

	updatePositionWithCollision();

	syncShape();
}

void Drone::syncShape() {
	Entity::syncShape();

	shape.setPosition(
		(cx + rx) * C::GRID_SIZE,
		(cy + ry) * C::GRID_SIZE
	);
}

void Drone::shoot() {
	Game* const game = Game::instance;

	const std::optional<Enemy*> optionalTarget = game->player->findClosestTarget();
	if (!optionalTarget)
		return;

	const Enemy* const target = *optionalTarget;

	const Vector2f position = { cx + rx, cy + ry };
	const Vector2f targetPosition = { target->cx + target->rx, target->cy + target->ry - target->collisionHeight / 2 };
	const Vector2f direction = normalize(targetPosition - position);

	const float projectileX = cx + rx;
	const float projectileY = cy + ry + 0.5;

	game->addProjectile(new Projectile({ projectileX, projectileY }, direction * projectileSpeed, 1));
}
