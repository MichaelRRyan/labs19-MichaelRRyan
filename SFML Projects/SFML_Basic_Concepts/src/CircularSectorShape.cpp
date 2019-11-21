#include "CircularSectorShape.h"

CircularSectorShape::CircularSectorShape(float t_radius, std::size_t t_pointCount) :
	m_radius{ t_radius },
	m_pointCount{ t_pointCount }
{
	update();
}

void CircularSectorShape::setRadius(float t_radius)
{
	m_radius = t_radius;

	update();
}

float CircularSectorShape::getRadius() const
{
	return m_radius;
}

std::size_t CircularSectorShape::getPointCount() const
{
	return m_pointCount;
}

sf::Vector2f CircularSectorShape::getPoint(unsigned int t_index) const
{
	static const float pi = 3.141592654f;

	float angle = t_index * 2 * pi / getPointCount() - pi / 2;
	float x = std::cos(angle) * m_radius;
	float y = std::sin(angle) * m_radius;

	return sf::Vector2f(m_radius + x, m_radius + y);
}
