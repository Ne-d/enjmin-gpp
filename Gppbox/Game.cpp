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

	level.loadFromFile("res/levels/default.txt");

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

void Game::processInput(Event ev) {
	if (ev.type == Event::Closed) {
		win->close();
		closing = true;
		return;
	}

	if (isEditingLevel && !ImGui::GetIO().WantCaptureMouse) {
		auto mousePos = Mouse::getPosition(*win) / C::GRID_SIZE;

		if (Mouse::isButtonPressed(Mouse::Left)) {
			// Only add a wall if we weren't clicking on the last frame or if we were on a different tile
			if (!previousMousePos || previousMousePos.value() != mousePos) {
				if ((TileType)selectedTileType == TileType::Empty)
					level.removeTile(mousePos.x, mousePos.y);

				if ((TileType)selectedTileType == TileType::Wall)
					level.addWall(mousePos.x, mousePos.y);

				if ((TileType)selectedTileType == TileType::Spawner)
					level.addSpawner(mousePos.x, mousePos.y);
			}

			previousMousePos.emplace(mousePos);
		}

		if (Mouse::isButtonPressed(Mouse::Right)) {
			if ((!previousMousePos || previousMousePos.value() != mousePos))
				level.removeTile(mousePos.x, mousePos.y);

			previousMousePos.emplace(mousePos);
		}

		previousMousePos = std::nullopt;
	}
}


static double g_time = 0.0;
static double g_tickTimer = 0.0;


void Game::pollInput(const double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) {
		win->close();
		closing = true;
	}
}

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

	level.draw(isEditingLevel);

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

bool Game::im() {
	using namespace ImGui;

	bool changed = false;

	// Level Editor
	changed |= CollapsingHeader("Level Editor", ImGuiTreeNodeFlags_DefaultOpen);
	changed |= Checkbox("Enabled", &isEditingLevel);

	if (isEditingLevel) {
		const char* tileList[] = { "Empty", "Wall", "Spawner" };
		changed |= ListBox("Tile Type", &selectedTileType, tileList, 3);

		if (Button("Save Level")) {
			level.saveToFile("res/levels/test.txt");
		}

		if (Button("Load Level")) {
			level.loadFromFile("res/levels/test.txt");
		}
	}

	// Player
	changed |= entities.at(0)->im();

	return changed;
}

