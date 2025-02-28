#pragma once

#include <optional>
#include <string>
#include <vector>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

class Entity;

enum class TileType : uint8_t {
	Empty,
	Wall,
	Spawner
};

class Level {
public:
	// Methods
	explicit Level() = default;
	explicit Level(const std::string& filename);

	void clear();
	
	void saveToFile(const std::string& filename) const;
	void loadFromFile(const std::string& filename);

	void addWall(int x, int y);
	void addSpawner(int x, int y);

	void removeWall(int x, int y);
	void removeSpawner(int x, int y);

	void removeTile(int x, int y);

	void draw(bool editMode) const;

	void cacheWallShapes();
	void cacheSpawnerShapes();
	void cacheShapes();

	// Data
	std::vector<sf::Vector2i> walls;
	std::vector<sf::RectangleShape> wallShapes;

	std::vector<sf::Vector2i> spawners;
	std::vector<sf::RectangleShape> spawnerShapes;

private:
	void cacheWallShape(sf::Vector2i position);
	void cacheSpawnerShape(sf::Vector2i position);

	[[nodiscard]] std::optional<size_t> getWallIndex(sf::Vector2i pos) const;
	[[nodiscard]] std::optional<size_t> getSpawnerIndex(sf::Vector2i pos) const;
};
