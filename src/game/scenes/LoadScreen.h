#pragma once

#include "..\..\engine\Engine.h"
#include <string>

class LoadScreen : public SceneManager::Scene {
private:
	sf::Font font{};
	sf::Text title{ "SPACE INVADERS", font, 25 };
public:
	LoadScreen() : Scene("loadscreen")
	{
		font.loadFromFile("./resources/fonts/8bitOperatorPlus8-Bold.ttf");
		addEntity(&title);
	}

	void onActive() override {
		auto view = Engine::instance->getWindow()->getView();
		title.setPosition((view.getCenter().x)/2, 15);
	}

	void onUpdate() override {
		
	}
};