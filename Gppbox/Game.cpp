#include <imgui.h>
#include <array>
#include <vector>

#include "C.hpp"
#include "Game.hpp"

#include <iostream>
#include <optional>

#include "Enemy.hpp"
#include "Entity.hpp"
#include "HotReloadShader.hpp"
#include "Level.hpp"
#include "Player.hpp"


static int cols = C::RESOLUTION_X / C::GRID_SIZE;
static int lastLine = C::RESOLUTION_Y / C::GRID_SIZE - 1;

Game* Game::instance = nullptr;

Game::Game(RenderWindow* win)
	:
	win(win) {
	instance = this;
	bg = RectangleShape(Vector2f((float)win->getSize().x, (float)win->getSize().y));

	bool const isOk = tex.loadFromFile("res/bg_stars.png");
	if (!isOk) {
		printf("ERR : LOAD FAILED\n");
	}
	bg.setTexture(&tex);
	bg.setSize(Vector2f(C::RESOLUTION_X, C::RESOLUTION_Y));

	bgShader = new HotReloadShader("res/bg.vert", "res/bg.frag");

	for (int i = 0; i < C::RESOLUTION_X / C::GRID_SIZE; ++i)
		level.walls.emplace_back(i, lastLine);

	level.walls.emplace_back(0, lastLine - 1);
	level.walls.emplace_back(0, lastLine - 2);
	level.walls.emplace_back(0, lastLine - 3);

	level.walls.emplace_back(cols - 1, lastLine - 1);
	level.walls.emplace_back(cols - 1, lastLine - 2);
	level.walls.emplace_back(cols - 1, lastLine - 3);

	level.walls.emplace_back(cols >> 2, lastLine - 2);
	level.walls.emplace_back(cols >> 2, lastLine - 3);
	level.walls.emplace_back(cols >> 2, lastLine - 4);
	level.walls.emplace_back((cols >> 2) + 1, lastLine - 4);
	level.walls.emplace_back((cols >> 2) - 1, lastLine - 4);

	level.walls.emplace_back(10, 10);
	level.cacheWallShapes();

	// Let's assume the first entity in the array is always the Player.
	// TODO: Make that cleaner
	entities.emplace_back(new Player(30, 60));

	entities.emplace_back(new Enemy(69, 42));
}

bool Game::hasCollision(const int gridX, const int gridY) const {
	if (gridX < 0)
		return true;

	constexpr int screenBoundRight = C::RESOLUTION_X / C::GRID_SIZE - 1;
	if (gridX > screenBoundRight)
		return true;

	if (gridY < 0)
		return true;

	constexpr int screenBoundBottom = C::RESOLUTION_Y / C::GRID_SIZE - 1;
	if (gridY > screenBoundBottom)
		return true;

	if (isWall(gridX, gridY))
		return true;

	return false;
}

bool Game::hasCollision(const int gridX, const int gridY, const int width, const int height) const {
	bool collisionSoFar = false;
	
	for (int x = 0; x < width; ++x)
		for (int y = 0; y < height; ++y)
			collisionSoFar |= hasCollision(gridX + x, gridY - y);

	return collisionSoFar;
}

optional<Vector2i> previousMousePos = std::nullopt;

std::optional<size_t> Game::getWallIndex(const Vector2i pos) const {
	int index = -1;

	for (unsigned int i = 0; i < level.walls.size(); ++i)
		if (level.walls.at(i) == pos)
			index = i;

	if (index != -1)
		return std::make_optional(index);

	return std::nullopt;
}

void Game::processInput(Event ev) {
	if (ev.type == Event::Closed) {
		win->close();
		closing = true;
		return;
	}

	auto mousePos = Mouse::getPosition(*win) / C::GRID_SIZE;
	std::optional<int> wallIndex = std::nullopt;

	if (Mouse::isButtonPressed(Mouse::Left)) {
		wallIndex = getWallIndex(mousePos);

		// Only add a wall if we weren't clicking on the last frame or if we were on a different tile
		// and there was no wall already there.
		if ((!previousMousePos || previousMousePos.value() != mousePos) && !wallIndex) {
			level.walls.emplace_back(mousePos.x, mousePos.y);
			level.cacheWallShape(Vector2i(mousePos.x, mousePos.y));
		}

		previousMousePos.emplace(mousePos);
	}

	if (Mouse::isButtonPressed(Mouse::Right)) {
		wallIndex = getWallIndex(mousePos);

		if ((!previousMousePos || previousMousePos.value() != mousePos) && wallIndex) {
			level.walls.erase(level.walls.begin() + wallIndex.value());
			level.wallShapes.erase(level.wallShapes.begin() + wallIndex.value());
		}

		previousMousePos.emplace(mousePos);
	}

	previousMousePos = std::nullopt;
}


static double g_time = 0.0;
static double g_tickTimer = 0.0;


void Game::pollInput(const double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Key::Space)) {
		if (!wasPressed) {
			onSpacePressed();
			wasPressed = true;
		}
	}
	else {
		wasPressed = false;
	}
	if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) {
		win->close();
		closing = true;
	}
}

static std::vector<RectangleShape> rects;

int blendModeIndex(BlendMode bm) {
	if (bm == BlendAlpha)
		return 0;
	if (bm == BlendAdd)
		return 1;
	if (bm == BlendNone)
		return 2;
	if (bm == BlendMultiply)
		return 3;
	return 4;
}

void Game::update(const double dt) {
	// Set global delta time values
	deltaTime = dt;
	const double rate = 1.0 / deltaTime;
	deltaFrame = 60 / rate;
	
	pollInput(dt);

	g_time += dt;
	if (bgShader != nullptr)
		bgShader->update(dt);

	beforeParts.update(dt);
	afterParts.update(dt);

	for (auto* entity : entities)
		entity->update();
}

void Game::draw(RenderWindow& win) {
	if (closing) return;

	RenderStates states = RenderStates::Default;
	Shader* sh = &bgShader->sh;
	states.blendMode = BlendAdd;
	states.shader = sh;
	states.texture = &tex;
	sh->setUniform("texture", tex);
	//sh->setUniform("time", g_time);
	win.draw(bg, states);

	beforeParts.draw(win);

	for (RectangleShape const& r : level.wallShapes)
		win.draw(r);

	for (RectangleShape const& r : rects) 
		win.draw(r);

	for (const auto* const entity : entities)
		win.draw(entity->shape);
	
	afterParts.draw(win);
}

void Game::onSpacePressed() {
	
}

bool Game::isWall(const int cx, const int cy) const {
	for (const auto w : level.walls) {
		if (w.x == cx && w.y == cy)
			return true;
	}
	return false;
}

bool Game::im() const {
	return entities.at(0)->im();
}

