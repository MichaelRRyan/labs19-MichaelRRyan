/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 03/10/2019
/// </summary>

#include <iostream>
#include "triangle.h"

void processTriangle(Triangle const& t_triangle)
{
	std::cout << "Area: " << t_triangle.area() << std::endl;
}

int main()
{
	Triangle triangleA{ 10.0f, 20.0f, Triangle::Colour::Green };
	Triangle triangleB{ 10.0f, 20.0f, Triangle::Colour::Red };

	std::cout << "Hypotonuse: " << triangleA.hypot() << std::endl;
	std::cout << "Area: " << triangleA.area() << std::endl;

	if (triangleA.compare(triangleB))
	{
		std::cout << "Triangle A and B have the same colour" << std::endl;
	}
	else
	{
		std::cout << "Triangle A and B are not the same" << std::endl;
	}

	triangleB.setColour(Triangle::Colour::Green);


	if (triangleA == triangleB)
	{
		std::cout << "Triangle A and B are the same" << std::endl;
	}
	else
	{
		std::cout << "Triangle A and B are not the same at all" << std::endl;
	}

	processTriangle(triangleA);

	// Gets assigned junk values for width and height and if no value for them is set in the class
	Triangle triangleC;

	// Change the colour of the object?
	Triangle::Colour & colour = triangleA.getColour();
	colour = Triangle::Colour::Blue;

	system("pause");

	return EXIT_SUCCESS;
}