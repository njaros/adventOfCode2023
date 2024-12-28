#include "helpers.hpp"

#define RIGHT 1
#define LEFT 2
#define DOWN 3
#define UP 4

const std::string hexa("0123456789abcdef");
const Coord dirs[] = {Coord(1, 0), Coord(0, 1), Coord(-1, 0), Coord(0, -1)};

ll hexaToll(const std::string& hex) {
	int hexVal;
	ll res = 0;

	for (char c: hex) {
		hexVal = 0;
		while (hexa[hexVal] != c)
			++hexVal;
		res *= 16;
		res += hexVal;
	}

	return res;
}

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

void solve2(std::ifstream& input)
{
	std::string line;
	std::vector<Coord> points;
	Coord current(0, 0);
	std::string::size_type readStart;
	ll lg;
	ll area = 0;

	points.push_back(current);
	while (!input.eof())
	{
		getline(input, line);
		readStart = line.find('#');
		lg = hexaToll(line.substr(readStart + 1, 5));
		area += lg;
		current += dirs[line[readStart + 6] - '0'] * lg;
		points.push_back(current);
	}

	for (int i = 0; i < points.size() - 1; ++i) {
		area += (points[i].first * points[i + 1].second) - (points[i + 1].first * points[i].second);
	}

	std::cout << "result is " << area / 2 + 1 << '\n';

}

int main()
{
	std::ifstream input;
	ui part = 0;

	if (getFileAndPart(18, input, part))
		return errno;

	if (part == 1)
		solve(input);
	else
		solve2(input);
	return 0;
}