#include "Collectable.h"

Collectable::Collectable() :
	m_active{ true }
{
	m_sprite.setTexture(AssetManager::getTexture("spriteSheet"));
	m_sprite.setTextureRect({ 0, 80, 43, 57 });

	reset();
}

////////////////////////////////////////////////////////////
void Collectable::reset()
{
	m_sprite.setPosition(static_cast<float>(rand() % (ScreenSize::s_width - 200) + 100), static_cast<float>(rand() % (ScreenSize::s_height - 200) + 100));
	m_active = true;
}

////////////////////////////////////////////////////////////
void Collectable::draw(sf::RenderWindow& t_window) const
{
	if (m_active)
	{
		t_window.draw(m_sprite);
	}
}

////////////////////////////////////////////////////////////
bool const Collectable::isActive() const
{
	return m_active;
}

////////////////////////////////////////////////////////////
void Collectable::setActive(bool const t_state)
{
	m_active = t_state;
}

////////////////////////////////////////////////////////////
sf::Sprite const& Collectable::getSprite() const
{
	return m_sprite;
}

////////////////////////////////////////////////////////////