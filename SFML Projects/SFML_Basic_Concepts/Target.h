#ifndef TARGET_H
#define TARGET_H

#include <SFML/Graphics.hpp>

class Target
{
public:
	Target() = default;
	sf::Sprite m_sprite;
	float m_secondsToLive;
};

#endif // !TARGET_H