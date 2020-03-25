#ifndef COLLECTABLE_H
#define COLLECTABLE_H

#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "ScreenSize.h"

class Collectable
{
public:

	Collectable();

	void reset();

	void draw(sf::RenderWindow & t_window) const;

	bool const isActive() const;

	void setActive(bool const t_state);

	sf::Sprite const & getSprite() const;

private:

	sf::Sprite m_sprite;
	bool m_active;

};

#endif // !COLLECTABLE_H