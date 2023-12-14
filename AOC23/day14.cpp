#include "helpers.hpp"

typedef std::map< Grid<char>, ull > content;
typedef std::map< ull, content > Cache;

ui atobSum(ui a, ui b)
{
	return ((b + a) * (labs(b - a) + 1)) / 2;
}

void toN(Grid<char>& grid)
{
	Grid<char>::browser b;
	char* c;
	Coord coord;

	b = grid.columnBrowse(b);
	while (b.has_value())
	{
		c = std::get<2>(b.value());
		if (*c == 'O')
		{
			coord = std::get<0>(b.value()) - Coord(0, 1);
			while (coord.second >= 0 && grid(coord) == '.')
				--coord.second;
			++coord.second;
			*c = '.';
			grid(coord) = 'O';
		}
		b = grid.columnBrowse(b);
	}
}

void toS(Grid<char>& grid)
{
	Grid<char>::browser b;
	char* c;
	Coord coord;

	b = grid.columnBrowseReverse(b);
	while (b.has_value())
	{
		c = std::get<2>(b.value());
		if (*c == 'O')
		{
			coord = std::get<0>(b.value()) + Coord(0, 1);
			while (coord.second < grid.size() && grid(coord) == '.')
				++coord.second;
			--coord.second;
			*c = '.';
			grid(coord) = 'O';
		}
		b = grid.columnBrowseReverse(b);
	}
}

void toW(Grid<char>& grid)
{
	Grid<char>::line::size_type pouet;
	for (Grid<char>::line& l : grid)
	{
		for (Grid<char>::line::size_type idx = 0; idx < l.size(); ++idx)
		{
			if (l[idx] == 'O')
			{
				pouet = idx - 1;
				while (pouet != ~0 && l[pouet] == '.')
					--pouet;
				++pouet;
				l[idx] = '.';
				l[pouet] = 'O';
			}
		}
	}
}

void toE(Grid<char>& grid)
{
	Grid<char>::line::size_type pouet;
	for (Grid<char>::line& l : grid)
	{
		for (Grid<char>::line::size_type idx = l.size() - 1; idx != ~0; --idx)
		{
			if (l[idx] == 'O')
			{
				pouet = idx + 1;
				while (pouet < l.size() && l[pouet] == '.')
					++pouet;
				--pouet;
				l[idx] = '.';
				l[pouet] = 'O';
			}
		}
	}
}

ull weighing(const Grid<char>& g)
{
	ull res = 0;
	Coord idx(0, 0);

	while (idx.second < g.size())
	{
		idx.first = 0;
		while (idx.first < g[idx.second].size())
		{
			if (g(idx) == 'O')
				res += (g.size() - idx.second);
			++idx.first;
		}
		++idx.second;
	}
	return res;
}

ull part2(Grid<char>& grid)
{
	std::pair< content::iterator, bool > insertResult;;
	Grid<char> mem;
	Cache heavyCache;
	ull loop = 0;
	ull patternStart;
	ull patternEnd;
	ull restToDo;
	ull weight;

	while (loop < 1000000000)
	{
		toN(grid);
		toW(grid);
		toS(grid);
		toE(grid);
		weight = weighing(grid);
		insertResult = heavyCache[weight].insert(std::make_pair(grid, loop));
		if (!insertResult.second)
			break;
		++loop;
	}
	patternStart = insertResult.first->second;
	patternEnd = loop;
	restToDo = (1000000000 - patternStart - 1) % (patternEnd - patternStart);
	for (ull i = 0; i < restToDo; ++i)
	{
		toN(grid);
		toW(grid);
		toS(grid);
		toE(grid);
	}
	return weighing(grid);
}

int day14()
{
	std::ifstream input;
	ui part;
	char c;
	Grid<char> grid;
	Grid<char>::browser b;
	Coord idx(0, 0);
	ui loadStart;
	ui roundCount;
	ull res = 0;

	if (getFileAndPart(14, &input, &part))
		return errno;

	c = input.get();
	while (c != EOF)
	{
		grid.addBackElt(c, '\n');
		c = input.get();
	}

	if (part != 1)
	{
		res = part2(grid);
		std::cout << "result is " << res << std::endl;
		return 0;
	}

	b = grid.columnBrowse(b);
	roundCount = 0;
	loadStart = (ui)grid.size();
	while (b.has_value())
	{
		c = *std::get<2>(b.value());
		if (std::get<1>(b.value()))
		{
			if (roundCount)
				res += atobSum(loadStart - (roundCount - 1), loadStart);
			roundCount = 0;
			loadStart = (ui)grid.size();
		}
		if (c == '#')
		{
			if (roundCount)
				res += atobSum(loadStart - (roundCount - 1), loadStart);
			roundCount = 0;
			loadStart = (ui)grid.size() - (std::get<0>(b.value()).second + 1);
		}
		else if (c == 'O')
			++roundCount;
		b = grid.columnBrowse(b);
	}
	if (roundCount)
		res += atobSum(loadStart - (roundCount - 1), loadStart);

	std::cout << "result is " << res << std::endl;
	return 0;
}