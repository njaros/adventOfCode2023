#include "helpers.hpp"

void parse(Grid<char>& grid, std::ifstream& input)
{
	char c;

	c = input.get();
	while (c != EOF)
	{
		grid.addBackElt(c, '\n');
		c = input.get();
	}
}

void setEmpties(const Grid<char>& grid, std::set<int>& columns, std::set<int>& lines)
{
	bool empty;

	for (size_t idx = 0; idx < grid.size(); ++idx)
	{
		empty = true;
		for (char c : grid[idx])
		{
			if (c != '.')
			{
				empty = false;
				break;
			}
		}
		if (empty)
			lines.insert((int)idx);
	}
	for (size_t idx = 0; idx < grid.front().size(); ++idx)
	{
		empty = true;
		for (size_t y = 0; y < grid.size(); ++y)
		{
			if (grid[y][idx] != '.')
			{
				empty = false;
				break;
			}
		}
		if (empty)
			columns.insert((int)idx);
	}
}

unsigned long long getRes(Coord a, Coord b, const std::set<int>& emptyColumns, const std::set<int>& emptyLines, unsigned int part)
{
	int minX = a.first <= b.first ? a.first : b.first;
	int maxX = minX == a.first ? b.first : a.first;
	int minY = a.second <= b.second ? a.second : b.second;
	int maxY = minY == a.second ? b.second : a.second;
	unsigned long long res = 0;
	unsigned long long emptySpace = part == 1 ? 1 : 999999;

	for (int i : emptyColumns)
	{
		if (i > minX && i < maxX)
			res += emptySpace;
	}
	for (int i : emptyLines)
	{
		if (i > minY && i < maxY)
			res += emptySpace;
	}
	res += manhattanDist(a, b);

	return res;
}

int day11()
{
	std::ifstream input;
	unsigned int part;
	unsigned long long result;
	std::set<int> emptyColumns;
	std::set<int> emptylines;
	std::set<Coord>::const_iterator cit;
	std::set<Coord>::const_iterator next;
	std::set<Coord> galaxies;
	Grid<char> grid;

	getFileAndPart(11, &input, &part);

	parse(grid, input);
	galaxies = grid.findAll('#');
	setEmpties(grid, emptyColumns, emptylines);
	result = 0;
	cit = galaxies.begin();
	while (cit != galaxies.end())
	{
		next = cit;
		while (++next != galaxies.end())
			result += getRes(*cit, *next, emptyColumns, emptylines, part);
		++cit;
	}


	std::cout << "result is " << result << std::endl;
	return 0;
}