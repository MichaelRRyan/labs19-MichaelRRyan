#ifndef ASSET_LOADER_H
#define ASSET_LOADER_H

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 20/03/2020
/// </summary>

#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "LevelLoader.h"

class AssetLoader
{
public:

	AssetLoader();

	void loadTextures(LevelData const& t_levelData);

	void loadFonts();

private:

	sf::Texture m_spriteSheetTexture;
	sf::Texture m_backgroundTexture;
	sf::Texture m_guiTextures;
	sf::Texture m_menuBackground;

	sf::Font m_mainFont;

};

#endif // !ASSET_LOADER_H