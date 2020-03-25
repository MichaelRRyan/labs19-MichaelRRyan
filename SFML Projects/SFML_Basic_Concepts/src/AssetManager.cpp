#include "AssetManager.h"

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 20/03/2020
/// </summary>

// Initialise the static variables so they can be used
std::map<std::string, sf::Texture const&> AssetManager::m_textures;
std::map<std::string, sf::Font const&> AssetManager::m_fonts;

////////////////////////////////////////////////////////////
void AssetManager::addTexture(sf::Texture const& t_texture, std::string const& t_textureName)
{
	m_textures.emplace(t_textureName, t_texture);
}

////////////////////////////////////////////////////////////
void AssetManager::addFont(sf::Font const& t_font, std::string const& t_fontName)
{
	m_fonts.emplace(t_fontName, t_font);
}

////////////////////////////////////////////////////////////
sf::Texture const& AssetManager::getTexture(std::string const& t_textureName)
{
	if (m_textures.count(t_textureName))
	{
		return m_textures.at(t_textureName);
	}
	else
	{
		std::string s("ERROR: \"" + t_textureName + "\"" + " is not a valid texture name");
		throw std::exception(s.c_str());
	}

	return sf::Texture();
}

////////////////////////////////////////////////////////////
sf::Font const& AssetManager::getFont(std::string const& t_fontName)
{
	if (m_fonts.count(t_fontName))
	{
		return m_fonts.at(t_fontName);
	}
	else
	{
		std::string s("ERROR: \"" + t_fontName + "\"" + " is not a valid font name");
		throw std::exception(s.c_str());
	}

	return sf::Font();
}

////////////////////////////////////////////////////////////