#include "engine/Engine.h"

#include "game/scenes/GameScreen.h"
#include "game/scenes/LoadScreen.h"

void tick()
{
	// Input & Math;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		if(SceneManager::getActive()->name == "gamescreen")
			SceneManager::loadScene("loadscreen");
	}
}

void start()
{
	SceneManager::loadScene("loadscreen");
	//SceneManager::loadScene("gamescreen");
}

int main()
{
	Engine engine{};
	engine.onUpdate = tick;
	engine.onStart = start;

	GameScreen g{};
	LoadScreen l{};

	SceneManager::addScene(g.name, &g);
	SceneManager::addScene(l.name, &l);

	engine.init();

	return 0;
}