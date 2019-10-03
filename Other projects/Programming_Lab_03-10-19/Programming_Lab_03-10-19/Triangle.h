#ifndef TRAINGLE_H
#define TRIANGLE_H

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 03/10/2019
/// </summary>

#include <math.h>
#include <iostream>

class Triangle
{
public:
	enum class Colour
	{
		Red,
		Green,
		Blue,
		White,
		Black
	};

	inline Triangle() = default; // Explicitly defaulted constructer
	Triangle(float t_width, float t_height, Colour t_triangleColour);

	float hypot() const;
	float area() const;

	bool compare(Triangle const& t_triangle) const;
	bool operator==(Triangle const& t_triangle) const;

	//inline const Colour getColour() { return m_colour; }
	Colour& getColour();
	inline const float getBaseWidth() { return m_baseWidth; }
	inline const float getHeight() { return m_height; }

	void setColour(Colour t_colour);
private:

	float m_baseWidth{ 0.0f };
	float m_height{ 0.0f };
	Colour m_colour{ Colour::Red };
};


#endif // !TRAINGLE_H

