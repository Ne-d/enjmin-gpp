#include "Level.hpp"

#include <fstream>
#include <sstream>

#include "C.hpp"

Level::Level(const std::string& filename) {
	loadFromFile(filename);
}

void Level::saveToFile(const std::string& filename) const {
	std::ofstream fileStream(filename);

	for (auto& w : walls) {
		fileStream << w.x << " " << w.y << " 1\n";
	}

	fileStream.flush();
	fileStream.close();
}

void Level::loadFromFile(const std::string& filename) {
	walls.clear();

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
		const int tileType = std::stoi(token);

		switch (tileType) {
		case 1:
			walls.emplace_back(x, y);
			break;

		default:
			break;
		}
	}
}

void Level::cacheWallShape(const sf::Vector2i wall) {
	sf::RectangleShape rect(sf::Vector2f(16, 16));
	rect.setPosition((float)wall.x * C::GRID_SIZE, (float)wall.y * C::GRID_SIZE);
	rect.setFillColor(sf::Color(0x07ff07ff));
	wallShapes.push_back(rect);
}

void Level::cacheWallShapes() {
	wallShapes.clear();

	for (const sf::Vector2i w : walls)
		cacheWallShape(w);
}
