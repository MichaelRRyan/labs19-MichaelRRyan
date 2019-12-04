#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "ScreenSize.h"

class Bullet
{
public:
	Bullet(sf::Texture const& t_texture, sf::Vector2f t_position, float t_angleDeg, float t_barrelOffset);
	~Bullet();
	inline sf::Sprite getBody() { return m_sprite; }
	bool update(double dt);
private:
	sf::Texture const& m_texture;
	sf::Sprite m_sprite;

	sf::Vector2f m_velocity;
	float m_speed;
};

#endif // !BULLET_H