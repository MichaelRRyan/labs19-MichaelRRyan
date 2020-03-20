#ifndef COLLECTABLE_H
#define COLLECTABLE_H

#include <SFML/Graphics.hpp>
#include "AssetManager.h"

class Collectable
{
public:

	Collectable();

	void draw(sf::RenderWindow & t_window) const;

	sf::Sprite const & getSprite() const;

private:

	sf::Sprite m_sprite;

};

#endif // !COLLECTABLE_H