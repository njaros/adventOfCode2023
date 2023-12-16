#include "helpers.hpp"
#include "Beam.hpp"
#include "Tile.hpp"

static ui count(Machine& m, Beam b)
{
	std::pair<bool, bool> ADadou;
	std::deque<Beam> tasks;
	ui res;

	b.allBeams = &tasks;
	tasks.push_back(b);

	res = 0;
	while (!tasks.empty())
	{
		ADadou = tasks.front().proceed(m);
		while (ADadou.first)
		{
			if (ADadou.second)
				++res;
			ADadou = tasks.front().proceed(m);
		}
		tasks.pop_front();
	}
	for (Machine::iterator it = m.begin(); it != m.end(); ++it)
	{
		for (Tile& t : *it)
		{
			t.energized = 0;
			t.mem.clear();
		}
	}
	return res;
}

static void solve(std::ifstream& input, ui part)
{
	Machine m;
	ui res;
	char c;

	c = input.get();
	while (c != EOF)
	{
		m.addBackElt(c, '\n');
		c = input.get();
	}

	res = 0;
	if (part == 1)
	{
		res = count(m, Beam());
	}
	else
	{
		for (int i = 0; i < m.size(); ++i)
		{
			res = std::max<ui>(res, count(m, Beam(Coord(1, 0), Coord(-1, i), 0)));
			res = std::max<ui>(res, count(m, Beam(Coord(-1, 0), Coord((int)m[i].size(), i), 0)));
		}
		for (int i = 0; i < m.front().size(); ++i)
		{
			res = std::max<ui>(res, count(m, Beam(Coord(0, 1), Coord(i, -1), 0)));
			res = std::max<ui>(res, count(m, Beam(Coord(0, -1), Coord(i, (int)m.size()), 0)));
		}
	}
	std::cout << "result is " << res << std::endl;
}

int day16()
{
	std::ifstream input;
	ui part;

	if (getFileAndPart(16, &input, &part))
		return errno;

	solve(input, part);

	return 0;
}