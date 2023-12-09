#include "helpers.hpp"

typedef std::vector< int > Line;
typedef std::vector< Line > Lines;

bool allZero(const Line& l)
{
	for (const int& nb : l)
	{
		if (nb != 0)
			return false;
	}
	return true;
}

long getResult(const Line& l, unsigned int part)
{
	long result = 0;

	Lines ds;
	Line d;
	int current;
	int previous;

	ds.push_back(l);
	while (!allZero(ds.back()))
	{
		Line::size_type idx = 0;
		while (idx < ds.back().size())
		{
			current = ds.back()[idx];
			if (idx)
			{
				d.push_back(current - previous);
			}
			previous = current;
			++idx;
		}
		ds.push_back(d);
		d.clear();
	}

	for (Lines::reverse_iterator rit = ds.rbegin(); rit != ds.rend(); ++rit)
	{
		if (part == 1)
			result = rit->back() + result;
		else
			result = rit->front() - result;
	}

	return result;
}

int day9()
{
	std::ifstream input;
	unsigned int part;
	if (getFileAndPart(9, &input, &part))
		return errno;

	std::pair< std::optional< long >, char > nbRead(0, 0);
	Line line;
	long result = 0;

	while (nbRead.second != EOF)
	{
		nbRead = inputLib::ExtractNextNumber(input);
		if (nbRead.first.has_value())
		{
			line.push_back(nbRead.first.value());
			if (nbRead.second == '\n' || nbRead.second == EOF)
			{
				result += getResult(line, part);
				line.clear();
			}
		}
	}

	std::cout << "result is " << result << std::endl;

	return 0;
}