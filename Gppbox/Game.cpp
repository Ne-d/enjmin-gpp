#include <imgui.h>
#include <array>
#include <vector>

#include "C.hpp"
#include "Game.hpp"

#include "Entity.hpp"
#include "HotReloadShader.hpp"
#include "Player.hpp"


static int cols = C::RESOLUTION_X / C::GRID_SIZE;
static int lastLine = C::RESOLUTION_Y / C::GRID_SIZE - 1;

Game* Game::instance = nullptr;


Game::Game(sf::RenderWindow* win) {
	this->win = win;
	instance = this;
	bg = sf::RectangleShape(Vector2f((float)win->getSize().x, (float)win->getSize().y));

	bool isOk = tex.loadFromFile("res/bg_stars.png");
	if (!isOk) {
		printf("ERR : LOAD FAILED\n");
	}
	bg.setTexture(&tex);
	bg.setSize(sf::Vector2f(C::RESOLUTION_X, C::RESOLUTION_Y));

	bgShader = new HotReloadShader("res/bg.vert", "res/bg.frag");

	for (int i = 0; i < C::RESOLUTION_X / C::GRID_SIZE; ++i)
		walls.push_back(Vector2i(i, lastLine));

	walls.push_back(Vector2i(0, lastLine - 1));
	walls.push_back(Vector2i(0, lastLine - 2));
	walls.push_back(Vector2i(0, lastLine - 3));

	walls.push_back(Vector2i(cols - 1, lastLine - 1));
	walls.push_back(Vector2i(cols - 1, lastLine - 2));
	walls.push_back(Vector2i(cols - 1, lastLine - 3));

	walls.push_back(Vector2i(cols >> 2, lastLine - 2));
	walls.push_back(Vector2i(cols >> 2, lastLine - 3));
	walls.push_back(Vector2i(cols >> 2, lastLine - 4));
	walls.push_back(Vector2i((cols >> 2) + 1, lastLine - 4));
	cacheWalls();

	entities.emplace_back(new Player(69, 42, sf::RectangleShape({ C::GRID_SIZE, 2 * C::GRID_SIZE })));
}

void Game::cacheWalls()
{
	wallSprites.clear();
	for (Vector2i w : walls) {
		sf::RectangleShape rect(Vector2f(16,16));
		rect.setPosition((float)w.x * C::GRID_SIZE, (float)w.y * C::GRID_SIZE);
		rect.setFillColor(sf::Color(0x07ff07ff));
		wallSprites.push_back(rect);
	}
}

bool Game::hasCollision(const int gridX, const int gridY) {
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

void Game::processInput(sf::Event ev) {
	if (ev.type == sf::Event::Closed) {
		win->close();
		closing = true;
		return;
	}
	if (ev.type == sf::Event::KeyReleased) {
		
	
	}
}


static double g_time = 0.0;
static double g_tickTimer = 0.0;


void Game::pollInput(double dt) {
	Entity* player = entities.front();
	
	float lateralSpeed = 8.0;
	float maxSpeed = 40.0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)) {
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
		if (!wasPressed) {
			onSpacePressed();
			wasPressed = true;
		}
	}
	else {
		wasPressed = false;
	}
}

static sf::VertexArray va;
static RenderStates vaRs;
static std::vector<sf::RectangleShape> rects;

int blendModeIndex(sf::BlendMode bm) {
	if (bm == sf::BlendAlpha) return 0;
	if (bm == sf::BlendAdd) return 1;
	if (bm == sf::BlendNone) return 2;
	if (bm == sf::BlendMultiply) return 3;
	return 4;
};

void Game::update(double dt) {
	pollInput(dt);

	g_time += dt;
	if (bgShader) bgShader->update(dt);

	beforeParts.update(dt);
	afterParts.update(dt);

	for (auto* entity : entities)
		entity->update(dt);
}

 void Game::draw(sf::RenderWindow & win) {
	if (closing) return;

	sf::RenderStates states = sf::RenderStates::Default;
	sf::Shader * sh = &bgShader->sh;
	states.blendMode = sf::BlendAdd;
	states.shader = sh;
	states.texture = &tex;
	sh->setUniform("texture", tex);
	//sh->setUniform("time", g_time);
	win.draw(bg, states);

	beforeParts.draw(win);

	for (sf::RectangleShape  const& r : wallSprites)
		win.draw(r);

	for (sf::RectangleShape const& r : rects) 
		win.draw(r);
	
	for(const auto entity : entities)
		win.draw(entity->shape);
	
	afterParts.draw(win);
}

void Game::onSpacePressed() {
	
}

//@formatter:off
//Come back here you moron, for some reason, "this" is NULL.
//@formatter:on

bool Game::isWall(const int cx, const int cy)
{
	for (unsigned int i = 0; i < walls.size(); ++i) {
		const Vector2i w = walls[i];
		if (w.x == cx && w.y == cy)
			return true;
	}
	return false;
}

void Game::im()
{
	for(const auto entity : entities)
		entity->im();
}

