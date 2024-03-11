#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <map>

class SceneManager {
public:
	class Scene {
		static void f() { }
	public:
		std::string name;
		Scene(const std::string& scene_name) : name(scene_name) {
			scene_entities = std::vector<sf::Drawable*>();
		}

		sf::Color backcolor;

		virtual void onUpdate() {};
		virtual void onActive() {};

		bool isActive = false;
		bool firstSetup = true;

		std::vector<sf::Drawable*> scene_entities;
		void addEntity(sf::Drawable* e) { scene_entities.push_back(e); }
	};

	static std::map<const std::string, Scene*> scenes;
	static Scene* getActive()
	{
		return active_scene;
	}

	static void addScene(const std::string& n, Scene* s)
	{
		printf("[SCENEMANAGER] Adding scene %s\n", n.c_str());
		scenes.insert(std::make_pair(n, s));
	}

	static void loadScene(const std::string& scene_name)
	{
		printf("[SCENEMANAGER] Searching for %s\n", scene_name.c_str());
		{

			if (scenes.find(scene_name) != scenes.end())
			{
				printf("[SCENEMANAGER] Loading %s\n", scene_name.c_str());

				if (active_scene != nullptr)
				{
					printf("[SCENEMANAGER] Unloading %s\n", active_scene->name.c_str());
					active_scene->isActive = false;
				}

				printf("[SCENEMANAGER] Loaded %s\n", scene_name.c_str());
				active_scene = scenes[scene_name];
				active_scene->isActive = true;
				active_scene->onActive();

			}
			else
			{
				printf("[SCENEMANAGER] Couldn't find %s\n", scene_name.c_str());
			}
		}
	}

private:
	static Scene* active_scene;
};

SceneManager::Scene* SceneManager::active_scene = nullptr;
std::map<const std::string, SceneManager::Scene*> SceneManager::scenes = std::map<const std::string, Scene*>();