#include "Camera.hpp"

#include "Math.hpp"

Camera::Camera(const sf::Vector2f position, const sf::Vector2f size)
	:
	view(sf::View(position, size)),
	position(position),
	size(size), screenShakeTimer(std::chrono::milliseconds(0)), screenShakeMagnitude(0) {
}

void Camera::update() {
	view.setCenter(position);
	view.setSize(size);

	if (!screenShakeTimer.isFinished()) {
		sf::Vector2f posDelta;
		if (screenShakeDirection == sf::Vector2f{ 0, 0 }) {
			posDelta = random(0, screenShakeMagnitude) * sf::Vector2f{ 1, 0 };
			posDelta += random(0, screenShakeMagnitude) * sf::Vector2f{ 0, 1 };
		}
		else {
			posDelta = random(0, screenShakeMagnitude) * screenShakeDirection;
		}
		
		view.setCenter(position + posDelta);
	}
}

void Camera::addScreenShake(const float magnitude, const sf::Vector2f direction,
							const std::chrono::duration<float> duration) {
	screenShakeTimer.start(duration);
	screenShakeMagnitude = magnitude;
	screenShakeDirection = direction;
}
