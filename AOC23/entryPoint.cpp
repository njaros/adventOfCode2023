#include "Days.hpp"
#include "helpers.hpp"
#include <iostream>

int main()
{
	unsigned int day;
	int err;
	while (1)
	{
		day = 1;
		err = 0;

		std::cout << "Hello, select your day (0 to quit) : ";
		day = secureGetNumber();

		if (day > 31)
		{
			std::cout << "impossible: day can't exceed 31" << std::endl;
		}

		else
		{
			switch (day)
			{
			case 0:
				return 0;
			case 1:
				err = day1();
				break;
			case 2:
				err = day2();
				break;
			case 3:
				err = day3();
				break;
			case 4:
				err = day4();
				break;
			case 5:
				err = day5();
				break;
			case 6:
				err = day6();
				break;
			case 7:
				err = day7();
				break;
			default:
				std::cout << "not implemented yet" << std::endl;
			}
		}
		if (err)
		{
			std::cout << "a problem occured." << std::endl;
		}
	}
}