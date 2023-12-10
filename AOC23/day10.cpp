#include "helpers.hpp"

class Pipe
{
public:

	char type;
	bool visited;

	Pipe() : type(0), visited(false) {}
	Pipe(char t) : type(t), visited(false) {}
	Pipe(const Pipe& o) : type(o.type), visited(o.visited) {}

	Pipe& operator=(const Pipe& o)
	{
		type = o.type;
		visited = o.visited;
		return *this;
	}

	Coord out(Coord in, unsigned int part)
	{
		int jump = 1;
		if (part != 1)
			++jump;
		switch (type)
		{
		case '-':
			if (in.second == 0)
				return in;
			return Coord(0, 0);
		case '|':
			if (in.first == 0)
				return in;
			return Coord(0, 0);
		case 'L':
			if (in == Coord(0, jump))
				return Coord(jump, 0);
			if (in == Coord(-jump, 0))
				return Coord(0, -jump);
			return Coord(0, 0);
		case 'J':
			if (in == Coord(jump, 0))
				return Coord(0, -jump);
			if (in == Coord(0, jump))
				return Coord(-jump, 0);
			return Coord(0, 0);
		case '7':
			if (in == Coord(jump, 0))
				return Coord(0, jump);
			if (in == Coord(0, -jump))
				return Coord(-jump, 0);
			return Coord(0, 0);
		case 'F':
			if (in == Coord(-jump, 0))
				return Coord(0, jump);
			if (in == Coord(0, -jump))
				return Coord(jump, 0);
			return Coord(0, 0);
		default:
			throw("not a pipe");
		}
	}
};

std::ostream& operator<<(std::ostream& o, const Pipe& p)
{
	return o << p.type;
}

bool operator==(const Pipe& lhs, const Pipe& rhs)
{
	return lhs.type == rhs.type;
}

char initDirs(const Grid<Pipe>& g, const Coord& s, Coord& d1, Coord& d2, unsigned int part)
{
	bool d1Done = false;
	char type;
	int jump = 1;
	if (part != 1)
		++jump;
	if (s.first > jump - 1)
	{
		type = g(s + Coord(-jump, 0)).type;
		if (type == 'L' || type == '-' || type == 'F')
		{
			d1Done = true;
			d1 = Coord(-jump, 0);
		}
	}
	if (s.first < g[s.second].size() - jump)
	{
		type = g(s + Coord(jump, 0)).type;
		if (type == 'J' || type == '-' || type == '7')
		{
			if (!d1Done)
			{
				d1Done = true;
				d1 = Coord(jump, 0);
			}
			else
			{
				d2 = Coord(jump, 0);
				return '-';
			}
		}
	}
	if (s.second > jump - 1)
	{
		type = g(s + Coord(0, -jump)).type;
		if (type == 'F' || type == '|' || type == '7')
		{
			if (!d1Done)
			{
				d1Done = true;
				d1 = Coord(0, -jump);
			}
			else
			{
				d2 = Coord(0, -jump);
				if (d1 == Coord(jump, 0))
					return '7';
				else
					return 'F';
			}
		}
	}
	d2 = Coord(0, jump);
	if (d1 == Coord(jump, 0))
		return 'J';
	if (d1 == Coord(-jump, 0))
		return 'L';
	return '|';
}

bool inBounds(Coord c, size_t xMax, size_t yMax)
{
	if (c.first < 0 || c.second < 0)
		return false;
	if (c.first >= xMax || c.second >= yMax)
		return false;
	return true;
}

void setOutsiders(Grid<Pipe>& g, const std::set<Coord>& s)
{
	std::deque<Coord> tasks;
	Coord c;
	Coord dirs[4] =
	{
		Coord(-1, 0),
		Coord(1, 0),
		Coord(0, -1),
		Coord(0, 1)
	};
	g(Coord(0, 0)).visited = true;
	tasks.push_back(Coord(0, 0));
	while (!tasks.empty())
	{
		c = tasks.front();
		tasks.pop_front();
		for (Coord dir : dirs)
		{
			if (inBounds(c + dir, g.front().size(), g.size()))
			{
				if (!g(c + dir).visited && s.find(c + dir) == s.end())
				{
					g(c + dir).visited = true;
					tasks.push_back(c + dir);
				}
			}
		}
	}
}

int day10()
{
	std::ifstream input;
	unsigned int part;
	Grid<Pipe> grid;
	std::set<Coord> loop;
	Coord Spos;
	char Stype;
	Coord pos1;
	Coord pos2;
	Coord dir1;
	Coord dir2;
	char c;
	Grid<Pipe>::line::size_type lineSize = 0;
	int res;

	getFileAndPart(10, &input, &part);
	c = input.get();
	if (part == 1)
	{
		while (c != EOF)
		{
			if (c == 'S')
				Spos = grid.addBackElt(Pipe(c), Pipe('\n'));
			else
				grid.addBackElt(Pipe(c), Pipe('\n'));
			c = input.get();
		}
	}
	else
	{
		while (c != EOF)
		{
			grid.addBackElt('*');
			if (c == '\n')
			{
				lineSize = grid.back().size();
				grid.addBackElt(c, '\n');
				for (Grid<Pipe>::line::size_type idx = 0; idx < lineSize; ++idx)
				{
					grid.addBackElt('*');
				}
				grid.addBackElt('a', 'a');
			}
			else
			{
				if (c == 'S')
				{
					Spos = grid.addBackElt(c);
					++Spos.second;
				}
				else
					grid.addBackElt(c);
			}
			c = input.get();
		}
		Grid<Pipe>::line lineToAdd;
		for (Grid<Pipe>::line::size_type idx = 0; idx < lineSize; ++idx)
		{
			lineToAdd.push_back('*');
		}
		grid.addBackElt('*');
		grid.addFrontLine(lineToAdd);
		grid.addBackLine(lineToAdd);
	}

	pos1 = Spos;
	pos2 = Spos;
	res = 0;

	Stype = initDirs(grid, Spos, dir1, dir2, part);

	if (part != 1)
	{
		loop.insert(pos1 + Coord(dir1.first / 2, dir1.second / 2));
		loop.insert(pos2 + Coord(dir2.first / 2, dir2.second / 2));
	}
	pos1 += dir1;
	pos2 += dir2;
	if (part != 1)
	{
		loop.insert(Spos);
		loop.insert(pos1);
		loop.insert(pos2);
	}
	else
		++res;
	while (pos1 != pos2)
	{
		dir1 = grid(pos1).out(dir1, part);
		dir2 = grid(pos2).out(dir2, part);
		if (part != 1)
		{
			loop.insert(pos1 + Coord(dir1.first / 2, dir1.second / 2));
			loop.insert(pos2 + Coord(dir2.first / 2, dir2.second / 2));
		}
		pos1 += dir1;
		pos2 += dir2;
		if (part != 1)
		{
			loop.insert(pos1);
			loop.insert(pos2);
		}
		else
			++res;
	}
	if (part != 1)
	{
		setOutsiders(grid, loop);
		for (const Grid<Pipe>::line& l : grid)
		{
			for (const Pipe& p : l)
			{
				if (!p.visited && p.type != '*')
					++res;
			}
		}
		res -= (int)(loop.size() / 2);
	}
	std::cout << "result is " << res << std::endl;
	return 0;
}