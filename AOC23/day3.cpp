#include "helpers.hpp"

int getNumberFromSymbol(Coord idx, const Grid<char>& g, std::set<Coord>& visited)
{
	int ret = 0;
	Coord left(idx);
	int exp = 1;

	if (isdigit(g(idx)))
	{
		ret = g(idx) - '0';
		left -= Coord(1, 0);
		while (left.first >= 0 && isdigit(g(left)))
		{	
			exp *= 10;
			ret += (g(left) - '0') * exp;
			visited.insert(left);
			left -= Coord(1, 0);
		}
		idx += Coord(1, 0);
		while (idx.first < g[idx.second].size() && isdigit(g(idx)))
		{
			ret *= 10;
			ret += g(idx) - '0';
			visited.insert(idx);
			idx += Coord(1, 0);
		}
	}
	return ret;
}

bool isSymbol(char c, int part)
{
	if (part == 1)
		return (!isdigit(c) && c != '.');
	else
		return (c == '*');
}

int addNumbersNear(const Coord& idx, const Grid<char>& g, std::set<Coord>& visited, int part)
{
	Coord c;
	int ret = 0;
	std::vector<int> totalNumber;

	if (part == 2)
		visited.clear();

	for (int y = -1; y < 2; ++y)
	{
		for (int x = -1; x < 2; ++x)
		{
			if (x != 0 || y != 0)
			{
				c = idx + Coord(x, y);
				if (c.first >= 0 && c.second >= 0 &&
					c.second < g.size() && c.first < g[c.second].size() &&
					isdigit(g(c)) && visited.insert(c).second)
				{
					if (part == 1)
						ret += getNumberFromSymbol(c, g, visited);
					else
						totalNumber.push_back(getNumberFromSymbol(c, g, visited));
				}
			}
		}
	}
	if (part == 2 && totalNumber.size() == 2)
	{
		ret = 1;
		for (int i : totalNumber)
		{
			ret *= i;
		}
	}
	return ret;
}

int day3()
{
	unsigned int part = 0;
	std::ifstream input;
	getFileAndPart(3, input, part);

	Grid<char> grid;
	std::set<Coord> symbolsPos;
	char c = input.get();
	while (c != EOF)
	{
		grid.addBackElt(c, '\n', { '\r' });
		if (isSymbol(c, part))
			symbolsPos.insert(Coord((int)grid.back().size() - 1, (int)grid.size() - 1));
		c = input.get();
	}
	input.close();

	int sum = 0;
	std::set<Coord> visited;

	for (Coord idx : symbolsPos)
		sum += addNumbersNear(idx, grid, visited, part);

	std::cout << "result is : " << sum << std::endl;
	return 0;
}