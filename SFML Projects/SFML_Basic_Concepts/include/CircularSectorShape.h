#ifndef CIRCULAR_SECTOR_SHAPE
#define CIRCULAR_SECTOR_SHAPE

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 22/11/19
/// </summary>

#include <SFML/Graphics.hpp>
#include <iostream>

class CircularSectorShape : public sf::Shape
{
public:
	CircularSectorShape(float t_radius = 0, float t_completeness = 1.0f, std::size_t t_pointCount = 30);

	void setRadius(float t_radius);

	float getRadius() const;

	void setCompleteness(float t_completeness);

	float getCompleteness() const;

	virtual std::size_t getPointCount() const;

	virtual sf::Vector2f getPoint(unsigned int t_index) const;

private:
	float m_radius; // Radius of the shape
	float m_completeness; // Value between 0.0f and 1.0f that defines how full the shape is
	unsigned int m_pointsCut; // Number of points that are cut from the shape by the missing sector
	std::size_t m_pointCount; // Number of points composing the shape
};

#endif // !CIRCULAR_SECTOR_SHAPE