#pragma once

#include "SceneManager.h"
#include <SFML/Graphics.hpp>

class Entity {
private:
	bool _visible = false;

public:
	sf::Sprite sprite;
	sf::Texture texture;

	const int& isVisible() {
		return _visible;
	};

	void setVisible(const bool& visible) {
		_visible = visible;
	}

	Entity(const std::string& texturepath, sf::Vector2f scale, SceneManager::Scene* scene)
	{
		if (!texture.loadFromFile(texturepath))
			printf("Failed to load texture %s\n", texturepath.c_str());
		else
		{
			texture.setSmooth(true);
			sprite.setTexture(texture);
			sprite.setScale(scale);
			sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
			scene->scene_entities.push_back(&sprite);
		}
	}
};