#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

class Entity;

class Level {
public:
	// Methods
	explicit Level() = default;
	explicit Level(const std::string& filename);

	void saveToFile(const std::string& filename) const;
	void loadFromFile(const std::string& filename);

	void cacheWallShape(sf::Vector2i wall);
	void cacheWallShapes();

	// Data
	std::vector<sf::Vector2i> walls;
	std::vector<sf::RectangleShape> wallShapes;

	std::vector<sf::Vector2i> enemySpawners;
	std::vector<sf::RectangleShape> enemySpawnerShapes;
};
