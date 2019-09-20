/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 13/09/2019
/// </summary>

#include <iostream>

void q5(int* px, int* py);
void processArray(int* pBase, int const SIZE);

int main()
{
	float* p1;

	// Question 4
	float arr[] = { 12.5, 10.0, 13.5, 90.5, 0.5 };
	float* ptr1 = &arr[0];
	float* ptr2 = ptr1 + 3;

	std::cout << *ptr2 << std::endl;
	std::cout << (ptr2 - ptr1) << std::endl;

	system("pause");
	system("cls");

	// Question 5
	int x = 10, y = 20;
	q5(&x, &y); // &'s added to fix error

	std::cout << x << std::endl;
	std::cout << y << std::endl;

	system("pause");
	system("cls");

	// Array as pointer
	int arr2[] = { 1,2,3 };
	processArray(arr2, 3);

	system("pause");
	return 0;
}

// Question 5
void q5(int* px, int* py)
{
	*px = *px - *py;
	*py = *px + *py;
	*px = *py - *px;
}

// Array as pointer
void processArray(int* pBase, int const SIZE)
{
	for (int i = 0; i < SIZE; i++)
	{
		std::cout << pBase[i] << std::endl;
	}
}
