#pragma once

#include <vector>

#include "Camera.hpp"
#include "Level.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

#include "sys.hpp"

#include "ParticleMan.hpp"
#include "Player.hpp"
#include "Projectile.hpp"

class Entity;

class HotReloadShader;
class Game {

public:
	// Methods
	explicit Game(sf::RenderWindow* win);
	
	bool hasCollision(int gridX, int gridY) const;
	bool hasCollision(int gridX, int gridY, int width, int height) const;
	std::optional<Entity*> hasCollisionWithEnemy(float x, float y) const;
	
	void processInput(sf::Event ev);

	void pollInput(double dt);
	void onSpacePressed();

	void spawnEnemies();
	void addProjectile(Projectile* projectile);
	void update(double dt);

	void draw(RenderWindow& win);

	bool isWall(int cx, int cy) const;
	bool im();

	// Data
	static Game* instance;

	double deltaTime;
	double deltaFrame;

	sf::RenderWindow* win = nullptr;

	sf::RectangleShape bg;
	HotReloadShader* bgShader = nullptr;

	sf::Texture tex;

	bool closing = false;

	Level level;
	Player* player;
	Camera camera;
	
	std::vector<Entity*> entities;

	bool isEditingLevel = false;
	int selectedTileType = (int)TileType::Wall; // Has to be an int (and not a TileType) to be compatible with ImGui.
	char levelFilename[256] = "";

	ParticleMan beforeParts;
	ParticleMan afterParts;
};