#pragma once

#include <chrono>
#include <random>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>

#include "Timer.h"

class Camera {
public:
	// Methods
	Camera(sf::Vector2f position, sf::Vector2f size);

	void update();
	void addScreenShake(float magnitude, sf::Vector2f direction, std::chrono::duration<float> duration);

	// Data
	sf::View view;

	sf::Vector2f position;
	sf::Vector2f size;

	Timer screenShakeTimer;
	float screenShakeMagnitude;
	sf::Vector2f screenShakeDirection;
};
