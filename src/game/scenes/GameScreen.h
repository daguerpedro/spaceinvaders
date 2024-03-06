#pragma once

#include "..\..\engine\Engine.h"

class GameScreen : public SceneManager::Scene {
private:
	sf::Texture sTexture{};
	sf::Sprite ship{sTexture};
public:
	GameScreen() : Scene("gamescreen")
	{
		sTexture.loadFromFile("./resources/sprites/ship1.png");
		sTexture.setSmooth(true);

		ship.setScale(.1f, .1f);
	}

	void onActive() override {
		auto view = Engine::instance->getWindow()->getView();
		ship.setPosition(view.getCenter().x, view.getSize().y - ship.getGlobalBounds().getSize().y);
	}

	void onUpdate() override {
		auto view = Engine::instance->getWindow()->getView();
		sf::Vector2i dir{ 0,0 };

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			dir.x--;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			dir.x++;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			dir.y--;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			dir.y++;
		}

		float speed = 45;


		auto npos = sf::Vector2f{
			ship.getPosition().x + (dir.x * speed * Engine::instance->deltaTime),
			ship.getPosition().y + (dir.y * speed * 1.6f * Engine::instance->deltaTime)
		};

		if (!(
			npos.x > ship.getGlobalBounds().width / 2 &&
			npos.x < (view.getSize().x - ship.getGlobalBounds().width / 2)
			))
			npos.x = ship.getPosition().x;

		if (!(
			npos.y > (view.getSize().y / 2) &&
			npos.y < (view.getSize().y - ship.getGlobalBounds().height / 2)
			))
			npos.y = ship.getPosition().y;

		ship.setPosition(npos);
	}
};