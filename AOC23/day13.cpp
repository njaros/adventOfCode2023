#include "helpers.hpp"

int smudgeCompare(const std::vector<char>& lhs, const std::vector<char>& rhs)
{
	int smudges = 0;
	for (std::vector<int>::size_type idx = 0; idx < lhs.size(); ++idx)
	{
		if (lhs[idx] != rhs[idx])
			++smudges;
		if (smudges > 1)
			return 2;
	}
	return smudges;
}

unsigned long long browseLines(const Grid<char>& g, unsigned int part)
{
	Grid<char>::size_type mirror = 1;
	Grid<char>::size_type idxUp;
	Grid<char>::size_type idxDown;
	int smudges;
	int smudgeNeeded = part == 1 ? 0 : 1;

	while (mirror < g.size())
	{
		smudges = 0;
		idxUp = mirror - 1;
		idxDown = mirror;
		while (idxUp != ~0 && idxDown < g.size() && smudges < smudgeNeeded + 1)
		{
			smudges += smudgeCompare(g[idxUp], g[idxDown]);
			--idxUp;
			++idxDown;
		}
		if ((idxUp == ~0 || idxDown == g.size()) && smudges == smudgeNeeded)
		{
			return (unsigned long long)mirror * 100;
		}
		++mirror;
	}

	return 0;
}

unsigned long long browseColumns(const Grid<char>& g, unsigned int part)
{
	size_t mirror = 1;
	size_t idxLeft;
	size_t idxRight;
	size_t size = g.front().size();
	int smudges;
	int smudgeNeeded = part == 1 ? 0 : 1;

	while (mirror < size)
	{
		smudges = 0;
		idxLeft = mirror - 1;
		idxRight = mirror;
		while (idxLeft != ~0 && idxRight < size && smudges < smudgeNeeded + 1)
		{
			smudges += smudgeCompare(g.getcolumn(idxLeft), g.getcolumn(idxRight));
			--idxLeft;
			++idxRight;
		}
		if ((idxLeft == ~0 || idxRight == size) && smudges == smudgeNeeded)
		{
			return (unsigned long long)mirror;
		}
		++mirror;
	}

	return 0;
}

unsigned long long solve(const Grid<char>& g, unsigned int part)
{
	unsigned long long res;

	res = browseLines(g, part);
	if (!res)
		return browseColumns(g, part);
	return res;
}

int day13()
{
	unsigned long long res = 0;
	std::ifstream input;
	unsigned int part;
	std::string line;
	Grid<char> grid;

	if (getFileAndPart(13, &input, &part))
		return errno;

	while (!input.eof())
	{
		getline(input, line);
		if (!line.empty())
			grid.addBackLine(line);
		if (line.empty() || input.eof())
		{
			res += solve(grid, part);
			grid.clear();
		}
	}

	std::cout << "result is " << res << std::endl;

	return 0;
}