#ifndef BULLET_H
#define BULLET_H

/// <summary>
/// @Author Michael R, Ryan
/// @date 25/10/19
/// </summary>

#include <SFML/Graphics.hpp>
#include <iostream>
#include "ScreenSize.h"

/// <summary>
/// @brief A class to simulate a projectile
/// </summary>
class Bullet
{
public:
	/// <summary>
	/// @constructs the bullet object sprite, position and velocity
	/// </summary>
	/// <param name="t_texture">Texture sheet</param>
	/// <param name="t_position">Spawn position</param>
	/// <param name="t_angleDeg">Fire angle from position</param>
	/// <param name="t_barrelOffset">Length of tank barrel for positioning</param>
	Bullet(sf::Texture const& t_texture, sf::Vector2f t_position, float t_angleDeg, float t_barrelOffset);

	/// <summary>
	/// @brief Update the bullet position each frame
	/// </summary>
	/// <param name="dt"></param>
	/// <returns></returns>
	bool update(double dt);

	/// <summary>
	/// @brief returns the sprite of the bullet
	/// </summary>
	/// <returns>bullet sprite</returns>
	inline sf::Sprite getSprite() { return m_sprite; }

	/// <summary>
	/// @brief returns the bullet velocity
	/// </summary>
	/// <returns>bullet velocity</returns>
	inline sf::Vector2f getVelocity() { return m_velocity; }

private:

	// Private data members

	sf::Sprite m_sprite;
	sf::Vector2f m_velocity;
	float m_speed;
};

#endif // !BULLET_H