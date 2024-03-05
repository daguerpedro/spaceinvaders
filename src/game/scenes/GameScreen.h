#pragma once

#include "..\..\engine\Engine.h"

class GameScreen : public SceneManager::Scene {
private:
	Entity ship{ "./resources/ship1.png", { 0.1f, 0.1f }, this };
public:
	GameScreen() : Scene("gamescreen")
	{

	}

	void onActive() override {
		auto view = Engine::instance->getWindow()->getView();
		ship.sprite.setPosition(view.getCenter().x, view.getSize().y - ship.sprite.getGlobalBounds().getSize().y);
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
			ship.sprite.getPosition().x + (dir.x * speed * Engine::instance->deltaTime),
			ship.sprite.getPosition().y + (dir.y * speed * 1.6f * Engine::instance->deltaTime)
		};

		if (!(
			npos.x > ship.sprite.getGlobalBounds().width / 2 &&
			npos.x < (view.getSize().x - ship.sprite.getGlobalBounds().width / 2)
			))
			npos.x = ship.sprite.getPosition().x;

		if (!(
			npos.y > (view.getSize().y / 2) &&
			npos.y < (view.getSize().y - ship.sprite.getGlobalBounds().height / 2)
			))
			npos.y = ship.sprite.getPosition().y;

		ship.sprite.setPosition(npos);
	}
};