/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 13/09/2019
/// Time taken:
/// Session 1: 11:58 - 54 minutes
/// Session 2: 13:11 - 81 minutes
/// Total: 2 hours 15 minutes (135 minutes)
/// </summary>

#include <iostream>
#include <math.h>
#include <string>

int roundFloat(float t_num); // Question 1
double deg2rad(double t_degrees); // Question 2
void vectorNormalise(float inputVector[], float resultVector[]); // Question 3
void getNames(std::string t_names[], std::string* pNames[]); // Question 4
void sort(std::string* pNames[]); // Question 4

void displayQuestionSeparation();

int main() {

	std::cout << "Note: Enter only numbers, periods and spaces, or the program will not work as expected." << std::endl << std::endl;

	// Question 1
	// Initialise the variable
	float number;

	// Take input
	std::cout << "Question 1" << std::endl;
	std::cout << "Enter a number to round: ";
	std::cin >> number;

	// Run the function and display
	std::cout << number << " rounded is " << roundFloat(number) << std::endl;

	// Separate questions
	displayQuestionSeparation();
	
	// Question 2
	// Initialise the variable
	int angleDeg;

	// Take input
	std::cout << "Question 2" << std::endl;
	std::cout << "Enter a number a angle in degrees: ";
	std::cin >> angleDeg;

	// Run the function and display
	std::cout << angleDeg << " degrees is " << deg2rad(angleDeg) << " radians" << std::endl;

	// Separate questions
	displayQuestionSeparation();

	// Question 3
	// Initialise the variables
	float points[4];
	float resultVector[2];

	// Take input
	std::cout << "Question 3" << std::endl;
	std::cout << "Enter an X and Y value (separated by spaces): ";
	std::cin >> points[0] >> points[1];

	std::cout << "Enter another X and Y value (separated by spaces): ";
	std::cin >> points[2] >> points[3];

	// Run the function and display
	vectorNormalise(points, resultVector);
	std::cout << "The unit vector of the points { " << points[0] << ", " << points[1] << " } and { " << points[2] << ", " << points[3] << " } is { "
		<< resultVector[0] << ", " << resultVector[1] << " }" << std::endl;

	// Separate questions
	system("pause");
	system("cls");

	// Question 4
	// Initialise the variables
	std::string names[10];
	std::string* pNames[10];

	// Take input
	std::cout << "Question 4" << std::endl;
	getNames(names, pNames);

	// Run the function and display
	sort(pNames);

	system("cls");
	std::cout << "The names, in alphabetical order are (Lowercase names come after Z): " << std::endl;
	for (int i = 0; i < 10; i++) // Display names
	{
		std::cout << *pNames[i] << std::endl;
	}

	system("pause");

	return 0;
}

// Question 1
// Recieved helped to simplify from Aaron O'Neill
// Adds the decimal point to the original number, then truncates the remaining decimal
// (E.g. 4.4 + 0.4 = 4.8, truncated to 4)
// (E.g. 5.6 + 0.6 = 6.2, truncated to 6)
int roundFloat(float t_num)
{
	return static_cast<int>(t_num - static_cast<int>(t_num) + t_num);
}

// Question 2
// Converts degrees to radians
double deg2rad(double t_degrees)
{
	return t_degrees * acos(-1.0) / 180.0;
}

// Question 3
// Normalise a vector found from 4 passed points
void vectorNormalise(float inputVector[], float resultVector[])
{
	float distanceVector[2] = { inputVector[2] - inputVector[0], inputVector[3] - inputVector[1] }; // x2 - x1, y2 - y1

	// Work out vector between points, input four points
	float magnitude = sqrt((distanceVector[0] * distanceVector[0]) + (distanceVector[1] * distanceVector[1]));
	resultVector[0] = distanceVector[0] / magnitude;
	resultVector[1] = distanceVector[1] / magnitude;
}

// Question 4
// Loops through the array and gets the user to enter 10 names
void getNames(std::string t_names[], std::string* pNames[])
{
	std::cout << "Enter 10 names" << std::endl;
	for (int i = 0; i < 10; i++)
	{
		std::cout << "Enter a name (" << i + 1 << " of 10)" << std::endl;
		std::cin >> t_names[i];
		pNames[i] = &t_names[i];
	}
}

// Question 4
// Sorts the names alphabetically
void sort(std::string* pNames[])
{
	bool swapped = true; // Checks whether or not something was swapped this pass

	while (swapped) // Passes - Keeps going until nothing was swapped in a pass
	{
		swapped = false; // Set swapped to false every pass

		for (int j = 0; j < 9; j++) // Swaps - Switches the names if they are alphabetically out of order
		{
			if (*pNames[j] > * pNames[j + 1]) // Checks if the current index name is alphabetically higher than the next
			{
				std::swap(pNames[j], pNames[j + 1]); // Swaps the two pointers
				swapped = true; // Set swapped to true
			}
		}
	}
}

// Separates the questions
void displayQuestionSeparation()
{
	system("pause");
	system("cls");
	std::cout << "Note: Enter only numbers, periods and spaces, or the program will not work as expected." << std::endl << std::endl;
}