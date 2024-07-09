#include "helpers.hpp"

#define NONE 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define UP 4

typedef std::map< int, ui > Pouet;
typedef std::map< int, Pouet > DirScores;
typedef std::map< Coord, DirScores > Cache; //empecheur de tourner en rond
typedef std::map< Coord, ui > Cache2;

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

class Node
{

	Node() : _value(0), _cost(0), _dir(NONE), _dirCount(0), _parent(0)
	{}

public:

	Coord	_pos;
	int		_value;
	int 	_cost;
	int		_dir;
	int		_dirCount;
	Node*	_parent;

	Node(Coord pos, Node* parent, int heuristicValue, int selfCost, int dir) : _pos(pos), _dir(dir), _parent(parent)
	{
		if (parent)
			_cost = parent->_cost + selfCost;
		else
			_cost = 0;
		if (parent && parent->_dir == _dir)
			++_dirCount;
		else
			_dirCount = 1;
		if (_dirCount > 3)
			throw(std::domain_error("dir > 3 one Node constructor."));
		_value = _cost + heuristicValue;
	}
	Node(const Node& other) : _pos(other._pos),
		_value(other._value),
		_cost(other._cost),
		_dir(other._dir),
		_dirCount(other._dirCount),
		_parent(other._parent)
	{}
	~Node() {}
	Node& operator=(const Node& other)
	{
		_pos = other._pos;
		_value = other._value;
		_cost = other._cost;
		_dir = other._dir;
		_dirCount = other._dirCount;
		_parent = other._parent;
		return (*this);
	}
};

bool operator==(const Node& a, const Node& b) {
	return a._cost == b._cost && a._dir == b._dir && a._dirCount == b._dirCount && a._pos == b._pos && a._value == b._value;
}

bool operator!=(const Node& a,const Node& b) {
	return !(a == b);
}

bool operator<(const Node& a, const Node& b) {
	if (a._cost < b._cost)
		return true;
	if (a._cost > b._cost)
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
	return o << "cost: " << n._cost << ", pos: " << n._pos << ", dir: " << n._dir << ", dirCount: " << n._dirCount << ", value: " << n._value << '\n';
}

typedef std::multimap<int, Node*> openType;
typedef std::multimap<Coord, Node*> closeType;

class AStar
{
	//Coplien not to use
	AStar() {}
	AStar(const AStar& other) {}
	AStar& operator&(const AStar& other) { return *this; }
	//End of Coplien not to use

	//Member's data
	openType					_open;
	closeType					_close;
	std::unordered_set<Node *>	_secureFreeTool;
	std::map<Coord, int>		_zone;
	Coord						_start;
	Coord						_end;
	bool						_endReached;
	Node*						_current;
	std::set<Node>				_duplicateDetector;

	//Private tool functions

	bool _dirNotAllowed(int dir)
	{
		if (dir == opposite(_current->_dir))
			return true;
		if (dir == _current->_dir && _current->_dirCount == 3)
			return true;
		return false;
	}

	openType::iterator _insertToOpen(Node *n) {
		if (_duplicateDetector.insert(*n).second)
		{
			// if (_duplicateDetector.size() == 10000)
				// std::cout << _duplicateDetector << '\n';
			return _open.insert(std::make_pair(n->_value, n));
		}
		else
		{
			// std::cout << "duplicate \n";
			delete n;
		}
		return _open.end();
	}

	void _checkSide(int dir)
	{
		Coord	newCoord(_current->_pos);
		addDir(newCoord, dir);

		if (_dirNotAllowed(dir))
			return;
		std::map<Coord, int>::const_iterator  zoneIt = _zone.find(newCoord);
		int newVal = math::ManhattanDist(_end, newCoord);
		if (zoneIt != _zone.end()) // check if I'm out of border + if the height is too big
		{
			//	std::cout << newVal << '\n';
			Node* newNode = new Node(newCoord, _current, newVal, zoneIt->second, dir);
			closeType::iterator	found = _close.find(newNode->_pos);
			if (found != _close.end()
				&& found->second->_dir == newNode->_dir)
			{
				if (found->second->_dirCount <= newNode->_dirCount
					&& found->second->_value <= newNode->_value)
					{
						delete newNode;
						return;
					}
				if (found->second->_dirCount >= newNode->_dirCount
					&& found->second->_value > newVal + zoneIt->second + newNode->_cost)
				{
					_insertToOpen(newNode);
					delete found->second;
					_close.erase(found);
				}
				else {
					_insertToOpen(newNode);
				}
			}
			else
			{
				_insertToOpen(newNode);
			}
		}
	}

public:

	AStar(const std::map<Coord, int>& zone, const Coord& start, const Coord& end) : _start(start), _end(end), _zone(zone),
		_endReached(false)
	{
		Node* newNode = new Node(_start, NULL, math::ManhattanDist(start, end), 0, NONE);
		std::cout << newNode->_cost << " | " << newNode->_pos << " | " << newNode->_value << '\n';
		ui dist;
		_open.insert(std::make_pair(newNode->_value, newNode));
		_current = _open.begin()->second;
		while (!_open.empty() && !_endReached)
		{
			_open.erase(_open.begin());
			_close.insert(std::make_pair(_current->_pos, _current));
			_checkSide(LEFT);
			_checkSide(UP);
			_checkSide(DOWN);
			_checkSide(RIGHT);
			dist = math::ManhattanDist(_current->_pos, end);
			std::cout << dist << " | " << *_current << '\n';
			if (!dist)
				_endReached = true;
			if (!_open.empty() && !_endReached)
				_current = _open.begin()->second;
		}
	}

	~AStar()
	{
		for (openType::iterator it = _open.begin(); it != _open.end(); ++it)
		{
			_secureFreeTool.insert(it->second);
			delete (it->second);
		}
		for (closeType::iterator it = _close.begin(); it != _close.end(); ++it)
		{
			if (_secureFreeTool.find(it->second) == _secureFreeTool.end())
				delete (it->second);
		}
	}

	std::pair<int, Coord> stepCount()	const
	{
		int 	stepCount = 0;
		Node* idx = _current;
		Node* nextMove;
		if (!_endReached)
			return std::make_pair(-1, Coord(0, 0));
		while (idx->_parent)
		{
			stepCount += _zone.find(idx->_pos)->second;
			nextMove = idx;
			idx = idx->_parent;
		}
		return make_pair(stepCount, nextMove->_pos);
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
	// std::vector< Coord > path;
	// Cache2 cache;
	// ui res(1011);
	AStar a(m, start, end);

	std::cout << a.stepCount();
	// dfs(m, start, 0, NONE, 0, res, cache, path);
	// std::cout << "result is " << res << std::endl;
}

int main()
{
	std::ifstream input;
	ui part;
	std::map<Coord, int> map;
	Coord current(-1, 0);
	char c;

	if (getFileAndPart(17, &input, &part))
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
		// std::cout << current << " | " << c - '0' << '\n';
		c = input.get();
	}

	solve(map, part);
	return 0;
}