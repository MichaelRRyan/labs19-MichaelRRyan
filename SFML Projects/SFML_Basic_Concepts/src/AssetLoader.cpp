#include "AssetLoader.h"

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 20/03/2020
/// </summary>

////////////////////////////////////////////////////////////
AssetLoader::AssetLoader()
{
	AssetManager::addTexture(m_spriteSheetTexture, "spriteSheet");
	AssetManager::addTexture(m_backgroundTexture, "background");
	AssetManager::addTexture(m_guiTextures, "gui");
	AssetManager::addTexture(m_menuBackground, "menuBackground");

	AssetManager::addFont(m_mainFont, "mainFont");
}

////////////////////////////////////////////////////////////
void AssetLoader::loadTextures(LevelData const& t_levelData)
{
	// Load tank sprite
	if (!m_spriteSheetTexture.loadFromFile("./resources/images/SpriteSheet.png"))
	{
		std::string s("Error loading spritesheet texture");
		throw std::exception(s.c_str());
	}

	// Load the background texture
	if (!m_backgroundTexture.loadFromFile(t_levelData.m_background.m_fileName))
	{
		std::string s("Error loading background texture");
		throw std::exception(s.c_str());
	}

	// Load the gui texture
	if (!m_guiTextures.loadFromFile("./resources/images/GUI.png"))
	{
		std::string s("Error loading GUI textures");
		throw std::exception(s.c_str());
	}

	if (!m_menuBackground.loadFromFile("./resources/images/menuBackground.png"))
	{
		std::string s("Error loading menu background texture");
		throw std::exception(s.c_str());
	}
}

////////////////////////////////////////////////////////////
void AssetLoader::loadFonts()
{
	if (!m_mainFont.loadFromFile("./resources/fonts/Disarmer-pvVD.ttf"))
	{
		std::string s("Error loading font file");
		throw std::exception(s.c_str());
	}
}

////////////////////////////////////////////////////////////