#pragma once

#include <optional>
#include <vector>

#include "Level.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

#include "sys.hpp"

#include "ParticleMan.hpp"

class Entity;

class HotReloadShader;
class Game {

public:
	explicit Game(sf::RenderWindow* win);
	static Game* instance;

	double deltaTime;
	double deltaFrame;
	
	sf::RenderWindow*				win = nullptr;

	sf::RectangleShape				bg;
	HotReloadShader *				bgShader = nullptr;

	sf::Texture						tex;

	bool							closing = false;

	Level level;
	std::vector<Entity*>			entities;

	ParticleMan beforeParts;
	ParticleMan afterParts;
	
	bool hasCollision(int gridX, int gridY) const;
	bool hasCollision(int gridX, int gridY, int width, int height) const;

	std::optional<size_t> getWallIndex(Vector2i pos) const;
	void processInput(sf::Event ev);
	bool wasPressed = false;
	void pollInput(double dt);
	void onSpacePressed();

	void update(double dt);

	void draw(RenderWindow& win);

	bool isWall(int cx, int cy) const;
	bool im() const;
};