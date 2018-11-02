#pragma once
#include <SFML/Graphics/Texture.hpp>
#include "ResourceManager.h"
#include <iostream>

class TextureManager : public ResourceManager<TextureManager, sf::Texture>{
public:
	TextureManager(): ResourceManager("Build/textures.cfg") {}

	bool Load(sf::Texture* l_resource, const std::string& l_path) {
		return l_resource->loadFromFile(Utils::GetWorkingDirectory() + l_path);
	}
};