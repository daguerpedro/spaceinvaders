#include "engine/Engine.h"

#include "game/scenes/GameScreen.h"
#include "game/scenes/LoadScreen.h"

void tick()
{
	// Input & Math;
	
}

void start()
{
	SceneManager::loadScene("loadscreen");
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