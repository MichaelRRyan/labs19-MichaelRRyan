#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 20/03/2020
/// </summary>

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <iostream>

class AssetManager
{
public:

	static void addTexture(sf::Texture const & t_texture, std::string const & t_textureName);
	static void addFont(sf::Font const & t_font, std::string const & t_fontName);

	static sf::Texture const & getTexture(std::string const & t_textureName);
	static sf::Font const & getFont(std::string const& t_fontName);

private:

	static std::map<std::string, sf::Texture const &> m_textures;
	static std::map<std::string, sf::Font const &> m_fonts;

};

#endif // !ASSET_MANAGER_H