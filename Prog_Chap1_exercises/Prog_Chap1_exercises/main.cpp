/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 13/09/2019
/// </summary>

#include <iostream>
#include <math.h>

int roundFloat(float t_num); // Question 1
double deg2rad(double t_degrees); // Question 2
void vectorNormalise(float inputVector[], float resultVector[]); // Question 3

int main() {

	// Question 1
	std::cout << "Question 1" << std::endl;
	std::cout << roundFloat(5.5f) << std::endl;

	system("pause");
	system("cls");

	// Question 2
	std::cout << "Question 2" << std::endl;
	std::cout << deg2rad(180) << std::endl;

	system("pause");
	system("cls");

	// Question 3
	std::cout << "Question 2" << std::endl;
	float inputVector[] = { 5.0f, 5.0f };
	float resultVector[2];

	vectorNormalise(inputVector, resultVector);


	float magnitude = sqrt((resultVector[0] * resultVector[0]) + (resultVector[1] * resultVector[1]));
	std::cout << resultVector[0] << ", " << resultVector[1] << " + " << magnitude << std::endl;

	system("pause");

	return 0;
}

// Question 1
int roundFloat(float t_num)
{
	return (t_num - static_cast<int>(t_num) + t_num);
}

// Question 2
double deg2rad(double t_degrees)
{
	return t_degrees * acos(-1.0) / 180.0;
}

// Question 3
void vectorNormalise(float inputVector[], float resultVector[])
{
	// Work out vector between points, input four points
	float magnitude = sqrt((inputVector[0] * inputVector[0]) + (inputVector[1] * inputVector[1]));
	resultVector[0] = inputVector[0] / magnitude;
	resultVector[1] = inputVector[1] / magnitude;
}