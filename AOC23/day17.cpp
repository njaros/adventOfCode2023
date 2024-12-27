#include "helpers.hpp"

#define NONE 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define UP 4

const Coord dirs[] = {Coord(0, 0), Coord(1, 0), Coord(0, 1), Coord(-1, 0), Coord(0, -1)};

static int opposite(int dir)
{
	switch (dir)
	{
	case RIGHT:
		return LEFT;
	case LEFT:
		return RIGHT;
	case DOWN:
		return UP;
	case UP:
		return DOWN;
	}
	return NONE;
}

struct CloseVal
{
	Coord _pos;
	int _dir;
	int _dirCount;

	CloseVal() {}
	CloseVal(Coord pos, int dir, int dirCount): _pos(pos), _dir(dir), _dirCount(dirCount) {}
	CloseVal(const CloseVal& o): _pos(o._pos), _dir(o._dir), _dirCount(o._dirCount) {}

	bool operator==(const CloseVal& o) const {
		return
			_pos == o._pos &&
			_dir == o._dir &&
			_dirCount == o._dirCount;
	}

};

struct hashCloseVal
{
	size_t operator()(const CloseVal& c) const {
		return
			std::hash<long long>()(c._pos.first) ^
			(std::hash<long long>()(c._pos.second) << 1) ^
			(std::hash<int>()(c._dir) << 2) ^
			(std::hash<int>()(c._dirCount) << 3);
	}
};

class Node
{
public:

	Coord	_pos;
	ull 	_score;
	ull		_potential;
	int		_dir;
	int		_dirCount;

	Node() : _pos(Coord(0 ,0)), _score(0), _potential(0), _dir(NONE), _dirCount(0) {}
	Node(Coord pos, int heuristicValue, int score, int dir, int dirCount):
	_pos(pos), _score(score), _potential(score + heuristicValue), _dir(dir), _dirCount(dirCount) {}
	Node(const Node& other):
		_pos(other._pos),
		_score(other._score),
		_potential(other._potential),
		_dir(other._dir),
		_dirCount(other._dirCount)
	{}
	~Node() {}
	Node& operator=(const Node& other)
	{
		_pos = other._pos;
		_potential = other._potential;
		_score = other._score;
		_dir = other._dir;
		_dirCount = other._dirCount;
		return (*this);
	}

	CloseVal getCloseVal() const {
		return CloseVal(_pos, _dir, _dirCount);
	}
};

bool operator==(const Node& a, const Node& b) {
	return a._score == b._score && a._dir == b._dir && a._dirCount == b._dirCount && a._pos == b._pos && a._potential == b._potential;
}

bool operator!=(const Node& a,const Node& b) {
	return !(a == b);
}

bool operator<(const Node& a, const Node& b) {
	if (a._potential < b._potential)
		return true;
	if (a._potential > b._potential)
		return false;
	if (a._pos < b._pos)
		return true;
	if (a._pos > b._pos)
		return false;
	if (a._dir < b._dir)
		return true;
	if (a._dir > b._dir)
		return false;
	return a._dirCount < b._dirCount;
}

std::ostream& operator<<(std::ostream& o, const Node& n)
{
	return o << "cost: " << n._score << ", pos: " << n._pos << ", dir: " << n._dir << ", dirCount: " << n._dirCount << ", value: " << n._potential << '\n';
}

typedef std::set<Node> OpenSet;
typedef std::unordered_map<CloseVal, Node, hashCloseVal> CloseSet;

class AStar
{
	//Coplien not to use
	AStar() {}
	AStar(const AStar& other) {}
	AStar& operator&(const AStar& other) { return *this; }
	//End of Coplien not to use

	//Member's data
	OpenSet						_open;
	OpenSet::iterator			_current;
	CloseSet					_close;
	std::map<Coord, int>		_zone;
	Coord						_start;
	Coord						_end;
	bool						_endReached;
	ui							_part;

	//Private tool functions

	int _getDirCount(int dir)
	{
		if (dir == opposite(_current->_dir))
			return 0;
		if (_part == 1) {
			if (dir == _current->_dir) {
				if (_current->_dirCount == 3)
					return 0;
				return _current->_dirCount + 1;
			}
			return 1;
		}
		else {
			if (_current->_dir == NONE)
				return 1;
			if (dir == _current->_dir) {
				if (_current->_dirCount == 7)
					return 0;
				return _current->_dirCount + 1;
			}
			return 1;
		}
	}

	void _insert(Node n) {
		CloseVal closeVal = n.getCloseVal();
		CloseSet::iterator found = _close.find(closeVal);

		if (found != _close.end()) {
			if (found->second._score > n._score) {
				_open.insert(n);
				_close[closeVal] = n;
			}
		}
		else {
			_open.insert(n);
			_close[closeVal] = n;
		}
	}

	void _checkSide(int dir)
	{
		Coord	newCoord(_current->_pos + dirs[dir]);
		int dirCount;
		ull nextScore = 0;

		dirCount = _getDirCount(dir);
		if (dirCount == 0)
			return;
		if (_part != 1) {
			if (dirCount == 1)
				newCoord += (dirs[dir] * 3);
		}
		std::map<Coord, int>::const_iterator found = _zone.find(newCoord);
		int heuristic = math::ManhattanDist(_end, newCoord);
		nextScore = _current->_score + found->second;
		if (found != _zone.end()) {
			if (_part != 1)
			{
				if (dirCount == 1) {
					nextScore = _current->_score + found->second +
					_zone.find(newCoord - dirs[dir])->second +
					_zone.find(newCoord - (dirs[dir] * 2))->second +
					_zone.find(newCoord - (dirs[dir] * 3))->second;
				}
			}
			_insert(Node(newCoord, heuristic, nextScore, dir, dirCount));
		}
	}

public:

	AStar(const std::map<Coord, int>& zone, const Coord& start, const Coord& end, ui part) : _start(start), _end(end), _zone(zone),
		_endReached(false), _part(part)
	{
		_open.insert(Node(start, 0, 0, NONE, 1));
		_current = _open.begin();
		while (!_open.empty() && _current->_pos != end)
		{
			_checkSide(LEFT);
			_checkSide(UP);
			_checkSide(DOWN);
			_checkSide(RIGHT);
			_open.erase(_current);
			_current = _open.begin();
		}
	}

	ull getScore() const {
		if (_current->_pos == _end)
			return _current->_score;
		return 0;
	}
};

static void printPath(Grid<int> m, const std::vector<Coord>& v)
{
	for (Coord f : v)
		m(f) = '*' - '0';
	Coord c;
	for (int j = 0; j < m.size(); ++j)
	{
		for (int i = 0; i < m[j].size(); ++i)
		{
			c = Coord(i, j);
			std::cout << (char)((char)m(c) + '0');
		}
		std::cout << std::endl;
	}
}

static void solve(const std::map<Coord, int>& m, ui part)
{
	Coord start(0, 0);
	Coord end(140, 140);
	AStar a(m, start, end, part);
	std::cout << "result is " << a.getScore() << '\n';
}

int main()
{
	std::ifstream input;
	ui part = 0;
	std::map<Coord, int> map;
	Coord current(-1, 0);
	char c;

	if (getFileAndPart(17, input, part))
		return errno;
	c = input.get();
	while (c != EOF)
	{
		if (c == '\n')
		{
			current.first = 0;
			current.second += 1;
			c = input.get();
		}
		else
			current.first += 1;
		map[current] = c - '0';
		c = input.get();
	}

	solve(map, part);
	return 0;
}