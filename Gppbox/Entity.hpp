#pragma once

#include <SFML/Graphics.hpp>

class Entity
{
public:
	// Methods
	explicit Entity(float x, float y, sf::RectangleShape shape);

	void setPixelPosition(int x, int y);
	
	void setGridPosition(float x, float y);
	void setGridVelocity(float x, float y);

	sf::Vector2i getPixelPosition() const;

private:
	void syncShape();
	void updatePosition(double deltaFrame);

public:
	// Methods
	virtual void update();
	virtual bool im();
	
	// Base coordinates
	int cx = 0;
	int cy = 0;
	float rx = 0.5f;
	float ry = 0.5f;
	
	// Movement
	float dx = 0;
	float dy = 0;

	int collisionWidth = 1;
	int collisionHeight = 2;
	
	bool isOnGround = false;
	
	// Graphics
	sf::RectangleShape shape;
};
