#ifndef CIRCULAR_SECTOR_SHAPE
#define CIRCULAR_SECTOR_SHAPE
#include <SFML/Graphics.hpp>

class CircularSectorShape : public sf::Shape
{
public:
	CircularSectorShape(float t_radius = 0, std::size_t t_pointCount = 30);

	void setRadius(float t_radius);

	float getRadius() const;

	virtual std::size_t getPointCount() const;

	virtual sf::Vector2f getPoint(unsigned int t_index) const;

private:
	float m_radius; // Radius of the shape
	std::size_t m_pointCount; // Number of points composing the shape
};

#endif // !CIRCULAR_SECTOR_SHAPE