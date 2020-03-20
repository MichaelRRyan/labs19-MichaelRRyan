#include "AssetManager.h"

std::map<std::string, sf::Texture const&> AssetManager::m_textures;

////////////////////////////////////////////////////////////
void AssetManager::addTexture(sf::Texture const& t_texture, std::string const& t_textureName)
{
	m_textures.emplace(t_textureName, t_texture);
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
		std::cout << "\"" + t_textureName + "\"" + " is not a valid texture name" << std::endl;
	}

	return sf::Texture();
}

////////////////////////////////////////////////////////////