#include "helpers.hpp"

int day1()
{
	std::ifstream input;
	unsigned int part;
	int sum = 0;

	if (getFileAndPart(1, &input, &part))
		return errno;

	if (part == 1)
	{
		char c;
		int first = -1;
		int last = 0;

		c = input.get();
		while (c != EOF)
		{
			if (c == '\n')
			{
				sum += 10 * first + last;
				first = -1;
			}
			if (isdigit(c))
			{
				if (first == -1)
					first = c - '0';
				last = c - '0';
			}
			c = input.get();
		}
		sum += 10 * first + last;
	}
	else
	{
		Graphe valuesLeft;
		valuesLeft["one"] = 10;
		valuesLeft["two"] = 20;
		valuesLeft["three"] = 30;
		valuesLeft["four"] = 40;
		valuesLeft["five"] = 50;
		valuesLeft["six"] = 60;
		valuesLeft["seven"] = 70;
		valuesLeft["eight"] = 80;
		valuesLeft["nine"] = 90;

		Graphe valuesRight;
		valuesRight["eno"] = 1;
		valuesRight["owt"] = 2;
		valuesRight["eerht"] = 3;
		valuesRight["ruof"] = 4;
		valuesRight["evif"] = 5;
		valuesRight["xis"] = 6;
		valuesRight["neves"] = 7;
		valuesRight["thgie"] = 8;
		valuesRight["enin"] = 9;

		std::string line;
		std::string::const_iterator first;
		std::string::const_reverse_iterator last;
		std::string::size_type forward;
		const Graphe* browser = 0;

		while (!input.eof())
		{
			forward = 0;
			browser = 0;
			getline(input, line);
			first = line.begin();
			last = line.rbegin();
			while (first != line.end())
			{
				if (isdigit(*first))
				{
					sum += (*first - '0') * 10;
					browser = 0;
					break;
				}
				if (browser != 0)
				{
					++forward;
					browser = browser->browse(*first);
					if (!browser)
					{
						first -= forward - 1;
						forward = 0;
						browser = valuesLeft.browse(*first);
					}
					else if (browser->getValue())
					{
						sum += browser->getValue();
						browser = 0;
						break;
					}
				}
				else
					browser = valuesLeft.browse(*first);
				++first;
			}
			browser = 0;
			forward = 0;
			while (last != line.rend())
			{
				if (isdigit(*last))
				{
					sum += (*last - '0');
					break;
				}
				if (browser != 0)
				{
					++forward;
					browser = browser->browse(*last);
					if (!browser)
					{
						last -= forward - 1;
						forward = 0;
						browser = valuesRight.browse(*last);
					}
					else if (browser->getValue())
					{
						sum += browser->getValue();
						break;
					}
				}
				else
					browser = valuesRight.browse(*last);
				++last;
			}
		}
	}
	input.close();
	std::cout << "response : " << sum << "\r\n";
	return 0;
}