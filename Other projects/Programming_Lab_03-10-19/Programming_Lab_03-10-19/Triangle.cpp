/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 03/10/2019
/// </summary>
/// 
#include "Triangle.h"

Triangle::Triangle(float t_width, float t_height, Colour t_triangleColour) :
	m_baseWidth{ t_width },
	m_height{ t_height },
	m_colour{ t_triangleColour }
{
};

float Triangle::hypot() const
{
	std::cout << "The function hypot() was called" << std::endl;
	return sqrtf((m_baseWidth * m_baseWidth) + (m_height * m_height));
}

float Triangle::area() const
{
	std::cout << "The function area() was called" << std::endl;
	return (m_baseWidth * m_height) / 2.0f;
}

bool Triangle::compare(Triangle const& t_triangle) const
{
	return m_colour == t_triangle.m_colour;
}

bool Triangle::operator==(Triangle const& t_triangle) const
{
	return m_colour == t_triangle.m_colour
		&& m_baseWidth == t_triangle.m_baseWidth
		&& m_height == t_triangle.m_height;
}

Triangle::Colour& Triangle::getColour()
{
	return m_colour;
}

void Triangle::setColour(Colour t_colour)
{
	m_colour = t_colour;
}
