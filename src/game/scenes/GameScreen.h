#pragma once

#include "..\..\engine\Engine.h"
#include <random>

class GameScreen : public SceneManager::Scene {
private:
	int random(int min, int max)
	{
		return min + (std::rand() % max);
	};

	class Enemy {
	public:
		sf::Sprite sprite;
		float speed;
		Enemy(float speed) : speed(speed) { };
		
		bool colliding(sf::Sprite s)
		{
			return sprite.getGlobalBounds().contains(s.getPosition());
		}
	};

	sf::Texture sTexture{};
	sf::Texture eTexture{};

	sf::Sprite ship{};
	std::vector<Enemy*> enemies;

	sf::View view() { return Engine::instance->getWindow()->getView(); } 

	void normalize(sf::Vector2f* v) {
		auto mag = sqrtf(powf(v->x, 2) + powf(v->y, 2));

		if (mag != 0)
		{
			v->x = v->x / mag;
			v->y = v->y / mag;
		}
	}

	int level = 1;

	float shipspeed = 90;

	bool paused = false;

	void movement()
	{
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

		if (npos.x < 0 || npos.x > view().getSize().x - ship.getGlobalBounds().width)
			npos.x = ship.getPosition().x;

		if (npos.y + ship.getGlobalBounds().height > view().getSize().y || npos.y < view().getSize().y / 2)
			npos.y = ship.getPosition().y;

		ship.setPosition(npos);
	}
	void enemyMove()
	{
		for (auto e : enemies)
		{
			e->sprite.move({0, e->speed * Engine::instance->deltaTime});
			if (e->colliding(ship)) //TODO: ENHANCE
			{
				//CREATE A DELETE FUNC
				e->sprite.setPosition(random(0.0f, view().getSize().x), 0);
			}
		}
	};

	void createEnemy()
	{		
		float base = 25;

		enemies.push_back((new Enemy{ 
			base + (base * level/7) 
		}));
		 
		for (auto en : enemies)
		{
			sf::Sprite* e = &en->sprite;
			e->setTexture(eTexture);
			e->setOrigin(e->getGlobalBounds().width / 2, e->getGlobalBounds().height / 2);
			e->setScale(0.1, 0.1);
			
			for (int i = 0; i < 10; i++)
				e->setPosition(random(0.0f, view().getSize().x), 0);

			addEntity(&en->sprite);
		}
	}
public:
	GameScreen() : Scene("gamescreen") { backcolor = sf::Color{ 119, 119, 119, 255 }; }

	void onActive() override {
		if (!firstSetup) return;
		else
		{
			firstSetup = false;
		}

		printf("[GAMESCREEN] Setting up\n");

#pragma region SHIP SETUP
		sTexture.loadFromFile("./resources/sprites/ship1.png");
		sTexture.setSmooth(true);

		ship.setTexture(sTexture);
		ship.setScale(.1f, .1f);
		ship.setOrigin(ship.getGlobalBounds().width / 2, ship.getGlobalBounds().height / 2);
		ship.setPosition(view().getCenter().x, view().getSize().y - ship.getGlobalBounds().getSize().y);
		addEntity(&ship);
#pragma endregion

		level = 0;
		enemies.clear();

		eTexture.loadFromFile("./resources/sprites/enemy1.png");
		eTexture.setSmooth(true);

		createEnemy();
	}

	void onUpdate() override {
		if (paused)
		{
			//MENU
			return;
		}

		movement();		
		enemyMove();
	}
};