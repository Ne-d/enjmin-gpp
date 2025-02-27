#include "Camera.hpp"

#include "Math.hpp"

Camera::Camera(const sf::Vector2f position, const sf::Vector2f size)
	:
	view(sf::View(position, size)),
	position(position),
	size(size) {
}

void Camera::update() {
	view.setCenter(position);
	view.setSize(size);

	if (!screenShakeTimer.isFinished()) {
		const sf::Vector2f posDelta = random(0, screenShakeMagnitude) * screenShakeDirection;
		view.setCenter(position + posDelta);
	}
}

void Camera::addScreenShake(const float magnitude, const sf::Vector2f direction,
							const std::chrono::duration<float> duration) {
	screenShakeTimer.start(duration);
	screenShakeMagnitude = magnitude;
	screenShakeDirection = direction;
}
