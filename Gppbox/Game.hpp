#pragma once

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include "sys.hpp"

#include "Particle.hpp"
#include "ParticleMan.hpp"

class Entity;

class HotReloadShader;
class Game {

public:
	explicit Game(sf::RenderWindow* win);
	static Game* instance;
	
	sf::RenderWindow*				win = nullptr;

	sf::RectangleShape				bg;
	HotReloadShader *				bgShader = nullptr;

	sf::Texture						tex;

	bool							closing = false;

	std::vector<sf::Vector2i> walls;
	std::vector<sf::RectangleShape> wallSprites;
	std::vector<Entity*>			entities;

	ParticleMan beforeParts;
	ParticleMan afterParts;
	
	void cacheWalls();
	bool hasCollision(int gridX, int gridY);

	void processInput(sf::Event ev);
	bool wasPressed = false;
	void pollInput(double dt);
	void onSpacePressed();

	void update(double dt);

	void draw(sf::RenderWindow& win);

	bool isWall(int cx, int cy);
	void im();
};