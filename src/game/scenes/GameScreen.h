#pragma once

#include "..\..\engine\Engine.h"
#include <ctime> 
#include <cstdlib>

class GameScreen : public SceneManager::Scene {
private:
	int random(int min, int max)
	{
		return rand() % (max - min + 1) + min;			
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

	sf::Text t_pause{};
	sf::Text t_score{};
	sf::Text t_lifes{};

	sf::Font font{};

	sf::Texture sTexture{};
	sf::Texture e1Texture{};
	sf::Texture e2Texture{};
	sf::Texture e3Texture{};

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

	int level = 0;
	int lifes = 0;
	int score = 0;

	int toSpawn = 0;

	float shipspeed = 90;

	float checkaccumulator = 0;

	float spawnaccumulator = 0;
	float spawndelay = 1;

	bool paused = false;
	bool wasPressing = false; 

	void setLife(int l)
	{
		lifes = l;
		std::string t = { "LIFES: " };
		t += std::to_string(l);

		t_lifes.setString(t);
	}
	void setScore(int s)
	{
		score = s;
		std::string t = { "SCORE: " };
		t += std::to_string(s);

		t_score.setString(t);
	}
	
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
		for (auto e	= enemies.begin(); e != enemies.end();)
		{
			auto enemy = *e;
			enemy->sprite.move({0, enemy->speed * Engine::instance->deltaTime});

			if (enemy->colliding(ship) || enemy->sprite.getPosition().y >= view().getSize().y) //TODO: ENHANCE
			{				
				scene_entities.erase(find(scene_entities.begin(), scene_entities.end(), &enemy->sprite));
				enemies.erase(find(enemies.begin(), enemies.end(), enemy));

				setLife(lifes - 1);
				delete enemy;
				return;
			} 
			e++;
		}
	};
	void createEnemy()
	{		
		float base = 25;
		int eType = level > 2 ? random(0, 2) : 0;

		auto en = new Enemy{
			base + eType + ((base + eType) * level / 7)
		};

		enemies.push_back(en);
		 
		sf::Sprite* e = &en->sprite;

		e->setTexture(
			eType == 0 ? e1Texture :
			eType == 1 ? e2Texture :
			e3Texture
		);

		float s = eType == 0 ? 0.1 : eType == 1 ? .25 : 0.15;
		e->setScale(s, s);
		e->setPosition(random(e->getGlobalBounds().width, view().getSize().x - e->getGlobalBounds().width), -20);		
		std::cout << e->getPosition().x << std::endl;

		addEntity(&en->sprite);
	}

	void nextLevel()
	{
		level++;
		toSpawn = random(level + 1, 2 * level);
	}

public:
	GameScreen() : Scene("gamescreen") { srand(std::time(NULL));  }

	void onActive() override {
		if (!firstSetup) return;
		else firstSetup = false;

		printf("[GAMESCREEN] Setting up\n");
		random(0, 1);
				
		// SHIP SETUP
		sTexture.loadFromFile("./resources/sprites/ship1.png");
		sTexture.setSmooth(true);

		ship.setTexture(sTexture);
		ship.setScale(.1f, .1f);
		ship.setOrigin(ship.getGlobalBounds().width / 2, ship.getGlobalBounds().height / 2);
		ship.setPosition(view().getCenter().x, view().getSize().y - ship.getGlobalBounds().getSize().y);
		addEntity(&ship);


		// LEVEL SETUP BEFORE ENEMY CREATION!
		level = 0;


		// ENEMY SETUP
		e1Texture.loadFromFile("./resources/sprites/enemy1.png");
		e2Texture.loadFromFile("./resources/sprites/enemy2.png");
		e3Texture.loadFromFile("./resources/sprites/enemy3.png");

		e1Texture.setSmooth(true);
		e2Texture.setSmooth(true);
		e3Texture.setSmooth(true);

		createEnemy();


		// TEXTS SETUP
		font.loadFromFile("./resources/fonts/8bitOperatorPlus8-Bold.ttf");
		font.setSmooth(true);

		t_score.setFont(font);
		t_score.setString("SCORE:");
		t_score.setPosition(10, 10);
		t_score.setScale(0.7, 0.7);
		t_score.setFillColor(sf::Color::Cyan);

		t_lifes.setFont(font);
		t_lifes.setString("LIFES:");
		t_lifes.setPosition(10, 10 + 25);
		t_lifes.setScale(0.7, 0.7);
		t_lifes.setFillColor(sf::Color::Cyan);

		t_pause.setFont(font);
		t_pause.setString("PAUSED");
		t_pause.setOrigin(t_pause.getLocalBounds().width / 2, t_pause.getLocalBounds().height / 2);
		t_pause.setPosition(view().getCenter().x, 150);
		t_pause.setString(paused ? "PAUSED" : "");
		t_pause.setFillColor(sf::Color::Red);

		addEntity(&t_score);
		addEntity(&t_pause);
		addEntity(&t_lifes);

		setScore(0);
		setLife(23);
	}

	void onUpdate() override {

		//TODO: MAKE ABLE TO SHOOT, MAKE ENEMY ABLE TO SHOOT, MAKE DEATH SCREEN, MAKE SOUNDS.

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !wasPressing)
		{
			wasPressing = true;
			paused = !paused;

			t_pause.setString(paused ? "PAUSED" : "");
		}
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && wasPressing)
		{
			wasPressing = false;
		}

		if (!paused)
		{
			auto dt = Engine::instance->deltaTime;

			spawnaccumulator += dt;
			checkaccumulator += dt;

			movement();		
			enemyMove();

			if (checkaccumulator >= 1.5f)
			{
				checkaccumulator = 0.0;

				if (lifes > 0)
				{
					if (enemies.size() == 0 && toSpawn == 0)
					{
						nextLevel();
						return;
					}		
				}
			}

			if (spawnaccumulator >= spawndelay)
			{
				spawnaccumulator = 0;

				if (toSpawn > 0 && lifes > 0)
				{
					spawndelay = random(1, 5);
					toSpawn--;
					createEnemy();
				}
			}
		}
	}
};