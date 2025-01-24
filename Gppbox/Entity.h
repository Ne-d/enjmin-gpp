#pragma once

#include <SFML/Graphics/Sprite.hpp>

class Entity
{
public:
	Entity();

	void update();
	
	// Base coordinates
	int cx;
	int cy;
	float xr;
	float yr;

	// Resulting coordinates
	float xx;
	float yy;

	// Movement
	float dx;
	float dy;

	// Graphics
	sf::Sprite sprite;
};
