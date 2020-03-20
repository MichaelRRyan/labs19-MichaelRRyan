#include "Collectable.h"

Collectable::Collectable()
{
	m_sprite.setTexture(AssetManager::getTexture("spriteSheet"));
	m_sprite.setTextureRect({ 0, 0, 52, 52 });
}

////////////////////////////////////////////////////////////
void Collectable::draw(sf::RenderWindow& t_window) const
{
	t_window.draw(m_sprite);
}

////////////////////////////////////////////////////////////
sf::Sprite const& Collectable::getSprite() const
{
	return m_sprite;
}

////////////////////////////////////////////////////////////