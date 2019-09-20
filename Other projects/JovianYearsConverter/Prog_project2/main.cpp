/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 12/09/2019
/// </summary>
/// 

#include <iostream>

int main()
{
	const float EARTH_YEARS_PER_JOVIAN_YEAR{ 12.0f };

	float jovianYears = 0;
	
	while (jovianYears != -1)
	{
		system("cls");
		std::cout << "Enter a number of Jovian years (Fractions allowed)" << std::endl << "Enter -1 to quit" << std::endl;
		std::cin >> jovianYears;

		if (jovianYears == -1)
		{
			std::cout << "Goodbye!" << std::endl;
		}
		else if (jovianYears >= 0)
		{
			system("cls");
			std::cout << jovianYears << " Jovian years is earth years " << jovianYears * EARTH_YEARS_PER_JOVIAN_YEAR << std::endl;
		}
		else
		{
			std::cout << "Please enter a positive number." << std::endl;
		}

		
		system("pause");
	}
	

	
	return 0;
}