#ifndef RAYCAST_H
#define RAYCAST_H

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 30/04/2020
/// </summary>

#include <SFML/Graphics.hpp>

class Raycast
{
public:

	static sf::Vector2f const castRay(float const& x1, float const& y1, float const& x2, float const& y2, float const& x3, float const& y3, float const& x4, float const& y4);

};

#endif // !RAYCAST_H
