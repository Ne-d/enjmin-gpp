// sfml_test.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream> // inclut un entete/header appelé iostream

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Audio.hpp>

#include "Bloom.hpp"
#include "Dice.hpp"
#include "Lib.hpp"
#include "Game.hpp"
#include "Interp.hpp"
#include "HotReloadShader.hpp"
#include "app.h"

using namespace std;
using namespace sf;

HotReloadShader * bloomShader = nullptr;
HotReloadShader * blurShader = nullptr;

int main()
{
    cout << "Hello World!\n";
	
    sf::RenderWindow window(sf::VideoMode(1280, 720,32), "SFML works!");
	window.setVerticalSyncEnabled(false);
    Font font;

    if (!font.loadFromFile("res/MAIAN.TTF")) {
        cout << "ERROR NO FONT" << endl;
        return 1;
    }

	if (!sf::Shader::isAvailable())
	{
		cout << "ERROR NO SHADER SYSTEM" << endl;
		return 1;
	}

	ImGui::SFML::Init(window);

    Game g(&window);

	Vector2i winPos;

	View v = window.getDefaultView();
	Vector2f viewCenter = v.getCenter();
	float shakeStrength = 0.0;

	sf::Clock timer;

	sf::Text fpsCounter;
	fpsCounter.setFont(font);
	fpsCounter.setString("FPS:");

	double frameStart = 0.0;
	double frameEnd = 0.0;

	sf::Texture winTex;
	winTex.create(window.getSize().x, window.getSize().y);

	bloomShader = new HotReloadShader("res/simple.vert", "res/bloom.frag");
	blurShader = new HotReloadShader("res/simple.vert", "res/blur.frag");
	sf::RenderTexture* destX = new sf::RenderTexture();
	destX->create(window.getSize().x, window.getSize().y);
	destX->clear(sf::Color(0, 0, 0, 0));

	sf::RenderTexture* destFinal = new sf::RenderTexture();
	destFinal->create(window.getSize().x, window.getSize().y);
	destFinal->clear(sf::Color(0, 0, 0, 0));	

	float bloomWidth = 12;
	sf::Glsl::Vec4 bloomMul(1,1,1,0.8);

    while (window.isOpen())
    {
		double dt = frameEnd - frameStart;
		frameStart = Lib::getTimeStamp();

		if (dt < 0.000001) {
			dt = 0.000001;
		}

        sf::Event event;
		while (window.pollEvent(event))//sort un evenement de la liste pour le traiter
		{
			ImGui::SFML::ProcessEvent(event);
			g.processInput(event);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
			shakeStrength = 15.0;
		}

		//don't use imgui before this;
		ImGui::SFML::Update(window, sf::seconds(dt));

        g.update(dt);
		
        window.clear();

		window.setView(v);

		Vector2f n(viewCenter);//viewCenter.x = shakeStrength * 10;
		n.x += cos(shakeStrength * 1 * Dice::randSign() );
		n.y += sin(-shakeStrength * 1 * Dice::randSign());
		v.setCenter(n);

		window.setView(v);

		if (ImGui::CollapsingHeader("Bloom Control")) {
			ImGui::LabelText("Update Time", "%0.6f", (Lib::getTimeStamp() - frameStart));
			ImGui::SliderFloat("bloomWidth", &bloomWidth, 0, 55);
			ImGui::SliderFloat4("bloomMul", &bloomMul.x, 0, 1.0);
		}
		g.im();

        g.draw(window);

		window.draw(fpsCounter);

		if (bloomWidth)
			Bloom::render(window,winTex,destX,destFinal,&blurShader->sh,&bloomShader->sh, bloomWidth, bloomMul);

		ImGui::SFML::Render(window);
        window.display();
		
		shakeStrength *= 0.85;
		if (shakeStrength <= 0.01) 
			shakeStrength = 0.0;

		frameEnd = Lib::getTimeStamp();
		
		fpsCounter.setString("FPS:"+std::to_string(1.0 / dt));
		
		ImGui::EndFrame();
    }

	ImGui::SFML::Shutdown();

    return 0;
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
