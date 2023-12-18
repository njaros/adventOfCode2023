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

void addDir(Coord& c, int dir)
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

void substractDir(Coord& c, int dir)
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
	Node(const Node& other) : _pos(other._pos), _value(other._value), _cost(other._cost), _parent(other._parent)
	{}
	~Node() {}
	Node& operator=(const Node& other)
	{
		_pos = other._pos;
		_value = other._value;
		_cost = other._cost;
		_parent = other._parent;
		return (*this);
	}
};

typedef std::multimap<int, Node*> openType;
typedef std::map<Coord, Node*> closeType;

class AStar
{
	//Coplien not to use
	AStar() {}
	AStar(const AStar& other) {}
	AStar& operator&(const AStar& other) { return *this; }
	//End of Coplien not to use

	//Member's data
	openType				_open;
	closeType				_close;
	std::set<Coord>			_secureFreeTool;
	std::map<Coord, int>	_zone;
	Coord					_start;
	Coord					_end;
	bool					_endReached;
	Node*					_current;

	//Private tool functions

	bool _dirNotAllowed(int dir)
	{
		if (dir == opposite(_current->_dir))
			return false;
		if (dir == _current->_dir && _current->_dirCount == 3)
			return false;
		return true;
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
			closeType::iterator	found = _close.find(_current->_pos);
			if (found != _close.end())
			{
				if (found->second->_value > newVal + zoneIt->second + _current->_cost)
				{
					Node* newNode = new Node(newCoord, _current, newVal, zoneIt->second, dir);
					_open.insert(std::make_pair(newNode->_value, newNode));
					_close.erase(found);
				}
			}
			else
			{
				Node* newNode = new Node(newCoord, _current, newVal, zoneIt->second, dir);
				_open.insert(std::make_pair(newNode->_value, newNode));
			}
		}
	}

public:

	AStar(const std::map<Coord, int>& zone, const Coord& start, const Coord& end) : _start(start), _end(end), _zone(zone),
		_endReached(false)
	{
		Node* newNode = new Node(_start, NULL, math::ManhattanDist(start, end), 0, NONE);
		_open.insert(std::make_pair(newNode->_value, newNode));
		_current = _open.begin()->second;
		while (!_open.empty() && !_endReached)
		{
			_checkSide(LEFT);
			_checkSide(UP);
			_checkSide(DOWN);
			_checkSide(RIGHT);
			std::pair<closeType::iterator, bool> res = _close.insert(std::make_pair(_open.begin()->second->_pos, _open.begin()->second));
			_open.erase(_open.begin());
			if (!math::ManhattanDist(_current->_pos, end))
				_endReached = true;
			if (!_open.empty() && !_endReached)
				_current = _open.begin()->second;
		}
	}

	~AStar()
	{
		for (openType::iterator it = _open.begin(); it != _open.end(); ++it)
		{
			_secureFreeTool.insert(it->second->_pos);
			delete (it->second);
		}
		for (closeType::iterator it = _close.begin(); it != _close.end(); ++it)
		{
			if (_secureFreeTool.find(it->first) == _secureFreeTool.end())
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

static void setOrder(std::deque< int >& o, const Grid< int >& m, Coord c)
{
	int a = 0;
	c.first += 1;
	if (m.inBounds(c))
		a = m(c);
	c.first -= 1;
	c.second += 1;
	if (m.inBounds(c))
	{
		if (!a)
			o.push_back(DOWN);
		else if (a <= m(c))
		{
			o.push_back(RIGHT);
			o.push_back(DOWN);
		}
		else
		{
			o.push_back(DOWN);
			o.push_back(RIGHT);
		}
	}
	else if (a)
		o.push_back(RIGHT);
	c.second -= 1;
	a = 0;
	c.first -= 1;
	if (m.inBounds(c))
		a = m(c);
	c.first += 1;
	c.second -= 1;
	if (m.inBounds(c))
	{
		if (!a)
			o.push_back(UP);
		else if (a <= m(c))
		{
			o.push_back(LEFT);
			o.push_back(UP);
		}
		else
		{
			o.push_back(UP);
			o.push_back(LEFT);
		}
	}
	else if (a)
		o.push_back(LEFT);
}

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

static void dfs(const Grid<int>& m, Coord& current, int dirCount, int prevDir, ui score, ui& bestScore, Cache2& cache, std::vector< Coord >& path)
{
	Pouet::const_iterator pcit;
	DirScores::const_iterator dcit;
	//Cache::const_iterator cit;
	Cache2::const_iterator cit;
	std::deque< int > order;
	int memDirCount;

	if (score + (math::ManhattanDist(current, Coord((int)m.back().size() - 1, (int)m.size() - 1))) >= bestScore)
		return;
	if (current.first == m.back().size() - 1 && current.second == m.size() - 1)
	{
		bestScore = score;
		std::cout << bestScore << std::endl;
	//	printPath(m, path);
		return;
	}
	/*cit = cache.find(current);
	if (cit != cache.end())
	{
		dcit = cit->second.find(prevDir);
		if (dcit != cit->second.end())
		{
			pcit = dcit->second.begin();
			while (pcit != dcit->second.end())
			{
				if (pcit->first <= dirCount && pcit->second < score)
					return;
				++pcit;
			}
		}
	}
	cache[current][prevDir][dirCount] = score;*/
	cit = cache.find(current);
	if (cit != cache.end() && cit->second < score)
	{
		return;
	}
	cache[current] = score;
	setOrder(order, m, current);
	for (int side : order)
	{
		if (prevDir != opposite(side) && (prevDir != side || dirCount < 3))
		{
			if (prevDir != side)
			{
				memDirCount = dirCount;
				dirCount = 0;
			}
			++dirCount;
			addDir(current, side);
			path.push_back(current);
			dfs(m, current, dirCount, side, score + m(current), bestScore, cache, path);
			path.pop_back();
			substractDir(current, side);
			if (prevDir != side)
				dirCount = memDirCount;
			else
				--dirCount;
		}
	}
}

static void solve(const Grid<int>& m, ui part)
{
	Coord start(0, 0);
	std::vector< Coord > path;
	Cache2 cache;
	ui res(1011);

	dfs(m, start, 0, NONE, 0, res, cache, path);
	std::cout << "result is " << res << std::endl;
}

int main()
{
	std::ifstream input;
	ui part;
	Grid<int> map;
	char c;

	if (getFileAndPart(17, &input, &part))
		return errno;
	c = input.get();
	while (c != EOF)
	{
		if (c == '\n')
			map.addEmptyLine();
		else
			map.addBackElt(c - '0');
		c = input.get();
	}

	solve(map, part);
	return 0;
}