#include "engine/Engine.h"

#include "game/scenes/GameScreen.h"
#include "game/scenes/LoadScreen.h"

#include <Windows.h>

void tick()
{
	// Input & Math;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
	{
		if(SceneManager::getActive()->name == "gamescreen")
			SceneManager::loadScene("loadscreen");
	}
}

void start()
{
	//SceneManager::loadScene("loadscreen");
	SceneManager::loadScene("gamescreen");
}

int main()
{
#ifdef _DEBUG
	ShowWindow(GetConsoleWindow(), SW_SHOW);
#else
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

	Engine engine{};
	engine.onUpdate = tick;
	engine.onStart = start;

	LoadScreen l{};
	GameScreen g{};

	SceneManager::addScene(l.name, &l);
	SceneManager::addScene(g.name, &g);

	engine.init();

	return 0;
}