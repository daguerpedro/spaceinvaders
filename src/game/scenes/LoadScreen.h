#pragma once

#include "..\..\engine\Engine.h"

class LoadScreen : public SceneManager::Scene {
private:
	void waiter()
	{
		printf("[LOADSCREEN] WAITING.\n");		
		sf::sleep(sf::seconds(5));
		printf("[LOADSCREEN] COMPLETE.\n");

		SceneManager::loadScene("gamescreen");
	};

public:
	LoadScreen() : Scene("loadscreen")
	{
		backcolor = sf::Color::Blue;
	}

	void onActive() override {
		sf::Thread wait(&LoadScreen::waiter, this);
		wait.launch();
	}

	void onUpdate() override {
		
	}
};