#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <iostream>

class AssetManager
{
public:

	static void addTexture(sf::Texture const & t_texture, std::string const & t_textureName);

	static sf::Texture const & getTexture(std::string const & t_textureName);

private:

	static std::map<std::string, sf::Texture const &> m_textures;

};

#endif // !ASSET_MANAGER_H