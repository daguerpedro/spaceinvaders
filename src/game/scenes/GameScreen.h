#pragma once

#include "..\..\engine\Engine.h"

class GameScreen : public SceneManager::Scene {
private:
	sf::Texture sTexture{};
	sf::Sprite ship{};

	float shipspeed = 90;

	void normalize(sf::Vector2f* v) {
		auto mag = sqrtf(powf(v->x, 2) + powf(v->y, 2));

		if (mag != 0)
		{
			v->x = v->x / mag;
			v->y = v->y / mag;
		}
	}

public:
	/*
	Don't instantiate objects in the scene constructors, this will cause all the objects defined here to be instantiated when the program starts, causing slowdowns or making the program save them in memory even before it needs them.
	*/
	GameScreen() : Scene("gamescreen")	{	} 

	void onActive() override {
		if (!firstSetup) return;
		else
		{
			firstSetup = false;
		}

		printf("[GAMESCREEN] Setting up\n");

		sTexture.loadFromFile("./resources/sprites/ship1.png");
		sTexture.setSmooth(true);

		ship.setTexture(sTexture);
		ship.setScale(.1f, .1f);
		ship.setOrigin(ship.getGlobalBounds().width / 2, ship.getGlobalBounds().height / 2);

		addEntity(&ship);

		auto view = Engine::instance->getWindow()->getView();
		ship.setPosition(view.getCenter().x, view.getSize().y - ship.getGlobalBounds().getSize().y);
	}

	void onUpdate() override {
		auto view = Engine::instance->getWindow()->getView();
		sf::Vector2f dir{ 0,0 };

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) dir.x--;		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) dir.x++;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) dir.y--;		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) dir.y++;

		normalize(&dir); // Cancel over speed when Vertical and Horizontal directions at the same time.

		auto npos = sf::Vector2f{
			ship.getPosition().x + (dir.x * shipspeed * Engine::instance->deltaTime),
			ship.getPosition().y + (dir.y * shipspeed * 1.6f * Engine::instance->deltaTime)
		};

		if (npos.x < 0 || npos.x > view.getSize().x - ship.getGlobalBounds().width)
			npos.x = ship.getPosition().x;

		if (npos.y + ship.getGlobalBounds().height > view.getSize().y || npos.y < view.getSize().y / 2)
			npos.y = ship.getPosition().y;

		ship.setPosition(npos);
	}
};