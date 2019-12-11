#include "CircularSectorShape.h"

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 22/11/19
/// </summary>

////////////////////////////////////////////////////////////
CircularSectorShape::CircularSectorShape(float t_radius, float t_completeness, std::size_t t_pointCount) :
	m_radius{ t_radius },
	m_pointCount{ t_pointCount },
	m_completeness{ 0.0f },
	m_pointsCut{ 0 }
{
	setCompleteness(t_completeness);
	// We don't need to call update as it was done in the function above
}

////////////////////////////////////////////////////////////
void CircularSectorShape::setRadius(float t_radius)
{
	m_radius = t_radius;

	update();
}

////////////////////////////////////////////////////////////
float CircularSectorShape::getRadius() const
{
	return m_radius;
}

////////////////////////////////////////////////////////////
void CircularSectorShape::setCompleteness(float t_completeness)
{
	// Set the completeness within its range
	if (t_completeness > 1.0f) // Max value
	{
		m_completeness = 1.0f;
		std::cout << "WARNING: " << t_completeness << " is an invalid input." << std::endl
			<< "A CircularSectorShape only takes values for completeness between 0.0f and 1.0f. Value set to 1.0f" << std::endl;
	}
	else if (t_completeness < 0.0f) // min value
	{
		m_completeness = 0.0f;
		std::cout << "WARNING: " << t_completeness << " is an invalid input." << std::endl
			<< "A CircularSectorShape only takes values for completeness between 0.0f and 1.0f. Value set to 0.0f" << std::endl;
	}
	else // Set the exact completeness if within range
	{
		m_completeness = t_completeness;
	}

	// Work out the number of points cut from the shape
	m_pointsCut = static_cast<unsigned>(m_pointCount - (m_pointCount * m_completeness));

	update();
}

////////////////////////////////////////////////////////////
float CircularSectorShape::getCompleteness() const
{
	return m_completeness;
}

////////////////////////////////////////////////////////////
std::size_t CircularSectorShape::getPointCount() const
{
	return m_pointCount;
}

////////////////////////////////////////////////////////////
sf::Vector2f CircularSectorShape::getPoint(unsigned int t_index) const
{
	// Check if the point at the given index is cut
	if (t_index < m_pointsCut)
	{
		return sf::Vector2f(m_radius, m_radius);
	}
	else // Otherwise use circle formula to find point coordinate
	{
		static const float pi = 3.141592654f;

		float angle = t_index * 2 * pi / getPointCount() - pi / 2;
		float x = std::cos(angle) * m_radius;
		float y = std::sin(angle) * m_radius;

		return sf::Vector2f(m_radius + x, m_radius + y);
	}
}
