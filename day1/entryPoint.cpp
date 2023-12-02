#include "Days.hpp"
#include "helpers.hpp"
#include <iostream>

int main()
{
	unsigned int day;
	while (1)
	{
		day = 1;

		std::cout << "Hello, select your day (0 to quit) : ";
		day = secureGetNumber();

		if (day > 31)
		{
			std::cout << "impossible: day can't exceed 31\r\n";
		}

		else
		{
			switch (day)
			{
			case 0:
				return 0;
			case 1:
				day1();
				break;
			case 2:
				day2();
				break;
			default:
				std::cout << "not implemented yet\r\n";
			}
		}
	}
}