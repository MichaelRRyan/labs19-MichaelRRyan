/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 12/09/2019
/// </summary>

#include <iostream>

int main() {
	// Define the conversion constant
	float const EARTLBS_TO_MOONLBS = 0.17f;

	// Print the the program title
	std::cout << "Earth to moon pounds converter" << std::endl;

	// Loop for each value to be displayed
	for (int i = 0; i < 100; i++)
	{
		// Pause for every 25 outputs
		if (i % 25 == 0)
		{
			std::cout << std::endl; // Add a new line
			system("pause"); // Pause the program
			system("cls"); // Clear the screen
			std::cout << "Earth LBS\t|\t\tMoon LBS" << std::endl; // Reprint the header
		}

		// Print to screen the values in earth pounds and moon pounds
		std::cout << i+1 << "\t\t|\t\t" << (i+1) * EARTLBS_TO_MOONLBS << std::endl;
	}

	return 0;
}