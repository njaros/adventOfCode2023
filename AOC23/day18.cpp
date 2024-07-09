#include "helpers.hpp"

#define RIGHT 1
#define LEFT 2
#define DOWN 3
#define UP 4

static void addDir(Coord& c, int dir)
{
	switch (dir)
	{
		case RIGHT:
			c.first += 1;
			break;
		case LEFT:
			c.first -= 1;
			break;
		case DOWN:
			c.second += 1;
			break;
		case UP:
			c.second -= 1;
			break;
	}
}

static void substractDir(Coord& c, int dir)
{
	switch (dir)
	{
		case RIGHT:
			c.first -= 1;
			break;
		case LEFT:
			c.first += 1;
			break;
		case DOWN:
			c.second -= 1;
			break;
		case UP:
			c.second += 1;
			break;
	}
}

class Lagoon
{
public :
	Coord outsidePointMin;
	Coord outsidePointMax;
	Coord current;
	std::map< Coord, int > edges;

	Lagoon() : outsidePointMin(0, 0), outsidePointMax(2, 2), current(1, 1), edges({std::make_pair(current, 0)}) {}
	Lagoon(const Lagoon& o) : outsidePointMin(o.outsidePointMin), outsidePointMax(o.outsidePointMax), current(o.current), edges(o.edges) {}
	~Lagoon() {}
	Lagoon& operator=(const Lagoon& o)
	{
		outsidePointMin = o.outsidePointMin;
		outsidePointMax = o.outsidePointMax;
		current = o.current;
		edges = o.edges;
		return *this;
	}

	void dig(int dir, int color)
	{
		addDir(current, dir);
		if (current.first == outsidePointMin.first)
			--outsidePointMin.first;
		else if (current.second == outsidePointMin.second)
			--outsidePointMin.second;
		else if (current.first == outsidePointMax.first)
			++outsidePointMax.first;
		else if (current.second == outsidePointMax.second)
			++outsidePointMax.second;
		edges[current] = color;
	}
};

ui getColor(const std::string& str)
{
	char hexaBase[17] = "0123456789abcdef";
	ui color = 0;
	std::string::size_type idx = 0;
	size_t hexaIdx;

	while (idx < str.size())
	{
		color <<= 4;
		hexaIdx = 0;
		while (hexaBase[hexaIdx] != str[idx])
			++hexaIdx;
		color += hexaIdx;
		++idx;
	}
	return color;
}

ui getOutsides(const Lagoon& l)
{
	std::set< Coord > visited;
	std::vector<int> dirs = {RIGHT, LEFT, DOWN, UP};
	std::deque<Coord> todos;
	Coord todo;
	todos.push_back(l.outsidePointMin);

	while (!todos.empty())
	{
		todo = todos.front();
		todos.pop_front();
		for (int dir : dirs)
		{
			addDir(todo, dir);
			if (todo.first >= l.outsidePointMin.first && todo.first <= l.outsidePointMax.first &&
			    todo.second >= l.outsidePointMin.second && todo.second <= l.outsidePointMax.second &&
				visited.find(todo) == visited.end() && l.edges.find(todo) == l.edges.end())
			{
				visited.insert(todo);
				todos.push_back(todo);
			}
			substractDir(todo, dir);
		}
	}
	return visited.size();
}

void solve(std::ifstream& input)
{
	std::string line;
	Lagoon l;
	int dir;
	int repeat;
	int color;
	ui res = 0;
	int totalSize = 0;

	while (!input.eof())
	{
		getline(input, line);
		switch (line[0]) {
			case 'R':
				dir = RIGHT;
				break;
			case 'L':
				dir = LEFT;
				break;
			case 'U':
				dir = UP;
				break;
			case 'D':
				dir = DOWN;
				break;
			default:
				throw (std::invalid_argument("bad input."));
		}
		repeat = atoi(&line.c_str()[2]);
		totalSize += repeat;
		if (!repeat)
			throw(std::invalid_argument("bad input."));
		color = getColor(line.substr(line.find('#') + 1, 6));
		while (repeat--)
			l.dig(dir, color);
	}

	res = ((labs(l.outsidePointMin.first - l.outsidePointMax.first) + 1) * (labs(l.outsidePointMin.second - l.outsidePointMax.second) + 1)) - getOutsides(l);
	std::cout << "result is " << res << std::endl;
}

ull getSign(int* plus, int dir1, int dir2)
{
	bool dir1IsPlus = false;
	bool dir2IsPlus = false;
	if (plus[0] == dir1 || plus[1] == dir1)
		dir1IsPlus = true;
	if (plus[0] == dir2 || plus[1] == dir2)
		dir2IsPlus = true;
	if (dir1IsPlus != dir2IsPlus)
		return -1;
	return 1;
}

void solve2(std::ifstream& input)
{
	int dir1;
	int dir2;
	int plus[2];
	int loop;
	int lg1;
	int lg2;
	long long res = 0;
	std::string line;

	loop = 0;
	while (!input.eof())
	{
		getline(input, line);
		switch (line[0]) {
			case 'R':
				dir1 = RIGHT;
				break;
			case 'L':
				dir1 = LEFT;
				break;
			case 'U':
				dir1 = UP;
				break;
			case 'D':
				dir1 = DOWN;
				break;
		}
		lg1 = atoi(&line.c_str()[2]) + 1;
		getline(input, line);
		switch (line[0]) {
			case 'R':
				dir2 = RIGHT;
				break;
			case 'L':
				dir2 = LEFT;
				break;
			case 'U':
				dir2 = UP;
				break;
			case 'D':
				dir2 = DOWN;
				break;
		}
		lg2 = atoi(&line.c_str()[2]) + 1;
		if (!loop)
		{
			plus[0] = dir1;
			plus[1] = dir2;
		}
		res += lg1 * lg2 * getSign(plus, dir1, dir2);
		++loop;
	}
	res /= 2;
	std::cout << "is 92758" << std::endl;
	std::cout << "result is " << res << std::endl;
}

int day18()
{
	std::ifstream input;
	ui part;

	if (getFileAndPart(18, &input, &part))
		return errno;

	solve2(input);
	return 0;
}