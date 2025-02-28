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

Game* Game::instance = nullptr;

Game::Game(RenderWindow* win)
	:
	win(win),
	camera({ C::RESOLUTION_X / 2, C::RESOLUTION_Y / 2 }, { C::RESOLUTION_X, C::RESOLUTION_Y }) {
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

	player = new Player(30, 60);
	drone = new Drone(player->cx, player->cy - 10);

	spawnEnemies();
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

std::optional<Entity*> Game::hasCollisionWithEnemy(const float x, const float y) const {
	for (Entity* const e : entities) {
		const float startX = e->cx + e->rx;
		const float endX = startX + e->collisionWidth;
		const float startY = e->cy + e->ry;
		const float endY = startY - e->collisionHeight;

		if (x >= startX && x <= endX && y <= startY && y >= endY)
			return std::make_optional(e);
	}

	return std::nullopt;
}

optional<Vector2i> previousMousePos = std::nullopt;

void Game::processInput(const Event& ev) {
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

int blendModeIndex(const BlendMode& bm) {
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

	camera.update();
	beforeParts.update(dt);

	player->update();
	drone->update();

	for (const auto& entity : entities)
		entity->update();

	for (int i = entities.size() - 1; i >= 0; --i) {
		if (entities.at(i)->shouldDie) {
			delete entities.at(i);
			entities.erase(entities.begin() + i);
		}
	}

	afterParts.update(dt);
}

void Game::draw(RenderWindow& win) {
	if (closing) return;

	win.setView(camera.view);

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

	player->draw();
	drone->draw();
	
	for (const auto* const entity : entities)
		entity->draw();
	
	afterParts.draw(win);
}

void Game::onSpacePressed() {
	
}

void Game::spawnEnemies() {
	for (const auto& spawner : level.spawners) {
		entities.emplace_back(new Enemy(spawner.x, spawner.y));
	}
}

void Game::removeEnemies() {
	for (int i = entities.size() - 1; i >= 0; --i) {
		if (entities.at(i)->type == EntityType::Enemy) {
			delete entities.at(i);
			entities.erase(entities.begin() + i);
		}
	}
}

void Game::addProjectile(Projectile* projectile) {
	entities.emplace_back(projectile);
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
		changed |= InputText("Level File", levelFilename, 1024);
		
		const char* tileList[] = { "Empty", "Wall", "Spawner" };
		changed |= ListBox("Tile Type", &selectedTileType, tileList, 3);

		if (Button("Save Level")) {
			level.saveToFile(std::string("res/levels/") + levelFilename + ".txt");
		}

		if (Button("Load Level")) {
			level.loadFromFile(std::string("res/levels/") + levelFilename + ".txt");
		}
	}

	// Player
	changed |= player->im();

	return changed;
}

