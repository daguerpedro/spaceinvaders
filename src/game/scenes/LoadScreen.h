#pragma once

#include "..\..\engine\Engine.h"
#include <string>

class LoadScreen : public SceneManager::Scene {
private:
	sf::Font font{};

	sf::Text title{ "SPACE INVADERS", font, 25 };

	sf::Text play{ "PLAY", font, 25 };
	sf::Text credits{ "CREDITS", font, 25 };
	sf::Text quit{ "QUIT", font, 25 };

	const sf::View& view(){
		return Engine::instance->getWindow()->getView();
	};

	void centerText(sf::Text* text, float y)
	{
		text->setPosition((view().getSize().x - text->getGlobalBounds().width) / 2, y);
	}

	void centralize(sf::Text* t)
	{
		t->setOrigin(t->getGlobalBounds().width / 2, t->getGlobalBounds().height / 2);
	}

	bool mouseOver(sf::Text* text)
	{
		auto p = sf::Mouse::getPosition(*Engine::instance->getWindow());
		return text->getGlobalBounds().contains(p.x, p.y);
	}

	std::list<sf::Text*> texts;
	
public:
	/*
	Don't instantiate objects in the scene constructors, this will cause all the objects defined here to be instantiated when the program starts, causing slowdowns or making the program save them in memory even before it needs them.
	*/
	LoadScreen() : Scene("loadscreen") { 	};

	void onActive() override {
		if (!firstSetup) return;
		firstSetup = false;	

		printf("[LOADSCREEN] Setting up\n");

		font.loadFromFile("./resources/fonts/8bitOperatorPlus8-Bold.ttf");
		font.setSmooth(true);
			
		texts.push_back(&play);
		texts.push_back(&credits);
		texts.push_back(&quit);

		addEntity(&title);

		for (auto t : texts)
		{
			addEntity(t);
			centralize(t);
		}

		sf::sleep(sf::milliseconds(100));
		centerText(&title, 15);

		float dist = 25 + 10;
		int i = -1;
		for (auto t : texts)
		{
			i++;

			float c = view().getCenter().y - (3 * dist / 2); // Center divided by the number of texts, centralize the 3 texts at the same
			int y = c + dist * i;

			t->setPosition(view().getSize().x/2, y);
		}
	}

	void onUpdate() override {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (mouseOver(&quit)) Engine::instance->shouldRun = false;
			if (mouseOver(&play)) SceneManager::loadScene("gamescreen");
		}

		for (auto t : texts)
		{
			if (mouseOver(t))
			{
				t->setScale(1.1, 1.1);
				t->setFillColor(sf::Color{ 192,211,255, 255 });
			}
			else
			if (!mouseOver(t))
			{
				t->setScale(1, 1);
				t->setFillColor(sf::Color::White);
			}			
		}		

	}
};