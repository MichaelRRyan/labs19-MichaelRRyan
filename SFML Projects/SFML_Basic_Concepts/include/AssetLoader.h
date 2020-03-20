#ifndef ASSET_LOADER_H
#define ASSET_LOADER_H

#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "LevelLoader.h"

class AssetLoader
{
public:

	AssetLoader();

	void loadTextures(LevelData const& t_levelData);

private:

	sf::Texture m_spriteSheetTexture;
	sf::Texture m_backgroundTexture;
	sf::Texture m_guiTextures;
	sf::Texture m_menuBackground;

};

#endif // !ASSET_LOADER_H