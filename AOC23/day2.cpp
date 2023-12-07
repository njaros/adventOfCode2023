#include "helpers.hpp"

int day2()
{
	std::ifstream input;
	unsigned int part;
	if (getFileAndPart(2, &input, &part))
		return errno;

	int sum = 0;
	int game = 0;
	int r = 0;
	int g = 0;
	int b = 0;
	int tmp;
	bool rip;
	std::string::size_type idx;
	std::string line;
	while (!input.eof())
	{
		getline(input, line);
		idx = line.find(':');
		line = line.substr(idx + 1);

		++game;
		rip = false;
		if (part == 2)
		{
			r = 0;
			g = 0;
			b = 0;
		}
		idx = 0;
		while (idx < line.size() && (!rip || part == 2))
		{
			if (isdigit(line[idx]))
			{
				tmp = atoi(&line[idx]);
				while (isdigit(line[++idx]));
				++idx;
				switch (line[idx])
				{
				case 'g':
					if (part == 1 && tmp > 13)
						rip = true;
					else if (tmp > g)
						g = tmp;
					break;
				case 'r':
					if (part == 1 && tmp > 12)
						rip = true;
					else if (tmp > r)
						r = tmp;
					break;
				case 'b':
					if (part == 1 && tmp > 14)
						rip = true;
					else if (tmp > b)
						b = tmp;
					break;
				default:
					"parsing broken or bad file format\r\n";
					input.close();
					return 1;
				}
			}
			++idx;
		}
		if (part == 2)
		{
			sum += r * g * b;
		}
		else if (!rip)
			sum += game;
	}
	std::cout << "result is " << sum << std::endl;
	input.close();
	return 0;
}