#pragma once

#include "SceneManager.h"

#include <functional>
#include <iostream>

class Engine {
private:
	sf::RenderWindow win;
	sf::Clock time{};

	bool _init = false;

	static void renderThread()
	{
		Engine::instance->win.setActive(true);

		do {
			sf::sleep(sf::milliseconds(20));
		} while (!Engine::instance->win.isOpen());

		while (Engine::instance->win.isOpen())
		{
			// sf::Lock lock(mutex);

			if (SceneManager::getActive() != nullptr)
				Engine::instance->win.clear(SceneManager::getActive()->backcolor);
			else
				Engine::instance->win.clear(sf::Color::Black);

			if (SceneManager::getActive() != nullptr)
				if(SceneManager::getActive()->isActive)
					for (auto e : SceneManager::getActive()->scene_entities)
						if(e != nullptr)
							Engine::instance->win.draw(*e);

			Engine::instance->win.display();
		}
	};
public:
	inline static Engine* instance = nullptr;

	Engine() { Engine::instance = this; };

	bool shouldRun = false;

	/* As seconds */ float deltaTime = 0;

	sf::RenderWindow* getWindow() {	return &win; };

	std::function<void()> onUpdate = NULL;
	std::function<void()> onStart = NULL;

	void init()
	{
		if (_init) {
			printf("RETURNING, YOU'RE TRYING TO REINIT THE ENGINE!");
			return;
		}

		_init = true;

		printf("[ENGINE] Starting engine\n");
		win.create(sf::VideoMode{ 300, 600 }, "Space Invaders", sf::Style::Close | sf::Style::Titlebar);

		win.setVerticalSyncEnabled(true);
		win.setFramerateLimit(60);
		win.setKeyRepeatEnabled(false);

		// deactivate its OpenGL context
		win.setActive(false);

		shouldRun = true;
		time.restart();

		printf("[ENGINE] Starting graphics thread.\n");
		sf::Thread graphics{ &renderThread };
		graphics.launch();

		printf("[ENGINE] Starting main thread\n");

		bool setup = false;
		while (win.isOpen() && shouldRun)
		{
			deltaTime = time.restart().asSeconds();

			sf::Event e;
			while (win.pollEvent(e))
			{
				if (e.type == sf::Event::Closed || (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape))
				{
					shouldRun = false;
				}
			}

			if (!setup)
			{
				setup = true;
				if (onStart != NULL)  onStart();
			}

			if(onUpdate != NULL) 
				onUpdate();

			if (SceneManager::getActive() != nullptr)
				SceneManager::getActive()->onUpdate();

		}

		if (win.isOpen())
			win.close();
	}
};