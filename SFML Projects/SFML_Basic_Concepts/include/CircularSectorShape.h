#ifndef CIRCULAR_SECTOR_SHAPE
#define CIRCULAR_SECTOR_SHAPE

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 22/11/19
/// </summary>

#include <SFML/Graphics.hpp>
#include <iostream>

/// <summary>
/// @brief a class that defines a circle sector shape using sfml
/// </summary>
class CircularSectorShape : public sf::Shape
{
public:
	/// <summary>
	/// @brief constructs the shape with given values
	/// </summary>
	/// <param name="t_radius">Radius</param>
	/// <param name="t_completeness">Percentage between 0 and 1 of completeness</param>
	/// <param name="t_pointCount">Number of points in the shape</param>
	CircularSectorShape(float t_radius = 0, float t_completeness = 1.0f, std::size_t t_pointCount = 30);

	/// <summary>
	/// @brief sets the shape radius
	/// </summary>
	/// <param name="t_radius"></param>
	void setRadius(float t_radius);

	/// <summary>
	/// @brief returns the shape radius
	/// </summary>
	/// <returns>Radius</returns>
	float getRadius() const;

	/// <summary>
	/// @brief set the percentage of the shape that is complete
	/// </summary>
	/// <param name="t_completeness">Percentage between 0 and 1 of completeness</param>
	void setCompleteness(float t_completeness);

	/// <summary>
	/// @brief return the completeness of the shape
	/// </summary>
	/// <returns>Completeness</returns>
	float getCompleteness() const;

	/// <summary>
	/// @brief get the number of points in the shape
	/// </summary>
	/// <returns></returns>
	virtual std::size_t getPointCount() const;

	/// <summary>
	/// @brief get the coordinates for a point by a given index
	/// </summary>
	/// <param name="t_index">index of the point</param>
	/// <returns>point position</returns>
	virtual sf::Vector2f getPoint(unsigned int t_index) const;

private:
	float m_radius; // Radius of the shape
	float m_completeness; // Value between 0.0f and 1.0f that defines how full the shape is
	unsigned int m_pointsCut; // Number of points that are cut from the shape by the missing sector
	std::size_t m_pointCount; // Number of points composing the shape
};

#endif // !CIRCULAR_SECTOR_SHAPE