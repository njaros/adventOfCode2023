#include "helpers.hpp"

int day10()
{
	long long a;
	long long b;
	std::cin >> a;
	std::cin >> b;

	std::optional<long long> res = divisible(a, b);
	if (res.has_value())
		std::cout << "result is " << res.value() << std::endl;
	else
		std::cout << "not divisible" << std::endl;

	return 0;
}