#include "Raycast.h"

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 30/04/2020
/// </summary>

sf::Vector2f const Raycast::castRay(float const& x1, float const& y1, float const& x2, float const& y2, float const& x3, float const& y3, float const& x4, float const& y4)
{
	float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

	if (den == 0) // Lines are parallel
	{
		return sf::Vector2f();
	}

	float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
	float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;

	if (t > 0.0f && t < 1.0f && u > 0.0f && u < 1)
	{
		return { x1 + t * (x2 - x1), y1 + t * (y2 - y1) };
	}

	return sf::Vector2f();
}
