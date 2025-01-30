#pragma once

#include "Entity.hpp"

class Player : public Entity {
public:
	explicit Player(float x, float y, sf::RectangleShape entityShape);
	~Player() override = default;

	void update(double deltaTime) override;

private:
	// Methods
	void pollInput();
	void move(double deltaTime);

	// Input data
	float moveInput;
	bool jumpInput;

	// Gameplay data
	float moveSpeed = 1.0f;
};
