#include "Level.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include "C.hpp"
#include "Game.hpp"

Level::Level(const std::string& filename) {
	loadFromFile(filename);
}

void Level::clear() {
	walls.clear();
	spawners.clear();

	wallShapes.clear();
	spawnerShapes.clear();
}

void Level::saveToFile(const std::string& filename) const {
	std::ofstream fileStream(filename);

	if (!fileStream.is_open()) {
		std::cerr << "Level::saveToFile: Unable to open file " << filename << std::endl;
		return;
	}

	for (const auto& w : walls) {
		fileStream << w.x << " " << w.y << " " << (int)TileType::Wall << "\n";
	}

	for (const auto& s : spawners) {
		fileStream << s.x << " " << s.y << " " << (int)TileType::Spawner << "\n";
	}

	fileStream.flush();
	fileStream.close();
}

void Level::loadFromFile(const std::string& filename) {
	clear();

	std::ifstream fileStream(filename);
	std::string line;

	while (std::getline(fileStream, line)) {
		std::stringstream sstream{ line };
		std::string token;

		sstream >> token;
		const int x = std::stoi(token);

		sstream >> token;
		const int y = std::stoi(token);

		sstream >> token;
		const auto tile = (TileType)std::stoi(token);

		if (tile == TileType::Wall)
			walls.emplace_back(x, y);

		if (tile == TileType::Spawner)
			spawners.emplace_back(x, y);
	}

	cacheShapes();
}

void Level::addWall(const int x, const int y) {
	removeTile(x, y);

	if (!getWallIndex(Vector2i{ x, y })) {
		walls.emplace_back(x, y);
		cacheWallShape(Vector2i(x, y));
	}
}

void Level::addSpawner(int x, int y) {
	removeTile(x, y);

	if (!getSpawnerIndex(Vector2i{ x, y })) {
		spawners.emplace_back(x, y);
		cacheSpawnerShape(Vector2i(x, y));
	}
}

void Level::removeWall(const int x, const int y) {
	const auto wallIndex = getWallIndex(Vector2i{ x, y });

	if (wallIndex) {
		walls.erase(walls.begin() + wallIndex.value());
		wallShapes.erase(wallShapes.begin() + wallIndex.value());
	}
}

void Level::removeSpawner(int x, int y) {
	const auto spawnerIndex = getSpawnerIndex(Vector2i{ x, y });

	if (spawnerIndex) {
		spawners.erase(spawners.begin() + spawnerIndex.value());
		spawnerShapes.erase(spawnerShapes.begin() + spawnerIndex.value());
	}
}

void Level::removeTile(const int x, const int y) {
	removeWall(x, y);
	removeSpawner(x, y);
}

void Level::cacheWallShape(const sf::Vector2i position) {
	sf::RectangleShape rect(sf::Vector2f(16, 16));
	rect.setPosition((float)position.x * C::GRID_SIZE, (float)position.y * C::GRID_SIZE);
	rect.setFillColor(sf::Color(0x07ff07ff));
	wallShapes.push_back(rect);
}

void Level::cacheWallShapes() {
	wallShapes.clear();

	for (const sf::Vector2i w : walls)
		cacheWallShape(w);
}

void Level::cacheSpawnerShape(const sf::Vector2i position) {
	sf::RectangleShape rect(sf::Vector2f(16, 16));
	rect.setPosition((float)position.x * C::GRID_SIZE, (float)position.y * C::GRID_SIZE);
	rect.setFillColor(sf::Color::Blue);
	spawnerShapes.push_back(rect);
}

void Level::cacheSpawnerShapes() {
	spawnerShapes.clear();

	for (const sf::Vector2i spawner : spawners)
		cacheSpawnerShape(spawner);
}

void Level::cacheShapes() {
	cacheWallShapes();
	cacheSpawnerShapes();
}

void Level::draw(const bool editMode) const {
	for (RectangleShape const& r : wallShapes)
		Game::instance->win->draw(r);

	if (editMode)
		for (RectangleShape const& r : spawnerShapes)
			Game::instance->win->draw(r);
}

std::optional<size_t> Level::getWallIndex(const Vector2i pos) const {
	int index = -1;

	for (unsigned int i = 0; i < walls.size(); ++i)
		if (walls.at(i) == pos)
			index = i;

	if (index != -1)
		return std::make_optional(index);

	return std::nullopt;
}

std::optional<size_t> Level::getSpawnerIndex(const Vector2i pos) const {
	int index = -1;

	for (unsigned int i = 0; i < spawners.size(); ++i)
		if (spawners.at(i) == pos)
			index = i;

	if (index != -1)
		return std::make_optional(index);

	return std::nullopt;
}
