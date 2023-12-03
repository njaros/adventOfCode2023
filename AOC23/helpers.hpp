#pragma once
#pragma warning(disable: 4996)

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <deque>
#include <unordered_map>
#include <unordered_set>

//Usefull typedefs

typedef std::pair<int, int> Coord;

Coord& operator+=(Coord& lhs, const Coord& rhs)
{
	lhs.first += rhs.first;
	lhs.second += rhs.second;
	return lhs;
}

Coord& operator-=(Coord& lhs, const Coord& rhs)
{
	lhs.first -= rhs.first;
	lhs.second -= rhs.second;
	return lhs;
}

Coord operator+(const Coord& lhs, const Coord& rhs)
{
	Coord ret;
	ret.first = lhs.first + rhs.first;
	ret.second = lhs.second + rhs.second;
	return ret;
}

Coord operator-(const Coord& lhs, const Coord& rhs)
{
	Coord ret;
	ret.first = lhs.first - rhs.first;
	ret.second = lhs.second - rhs.second;
	return ret;
}

//Some << overloads

template <class T>
std::ostream& operator<<(std::ostream& o, const std::vector<T>& l)
{
	for (const T& elt : l)
	{
		o << elt;
	}
	o << std::endl;
	return o;
}

std::ostream& operator<<(std::ostream& o, const Coord& c)
{
	return o << '(' << c.first << ", " << c.second << ')';
}

//Usefull simple class and their typedefs

template <class T>
class Grid : public std::vector<std::vector<T>> {

public:

	typedef std::vector<T> line;

	void addBackElt(const T& elt, const T& newLine)
	{
		if (this->empty())
			this->push_back(line());
		if (elt == newLine)
			this->push_back(line());
		else
			this->back().push_back(elt);
	}

	void addBackElt(const T& elt, const T& newLine, const std::set<T>& ignoreSet)
	{
		if (ignoreSet.find(elt) == ignoreSet.end())
			this->addBackElt(elt, newLine);
	}

	const T& get(const Coord& c) const
	{
		return this->at(c.second).at(c.first);
	}


	T& get(const Coord& c)
	{
		return this->at(c.second).at(c.first);
	}

	const T& operator()(const Coord& c) const
	{
		return get(c);
	}

	T& operator()(const Coord& c)
	{
		return get(c);
	}

	T getCopy(const Coord& c) const
	{
		return this->at(c.second).at(c.first);
	}
};

template <class T>
std::ostream& operator<<(std::ostream& o, const Grid<T>& g)
{
	for (const std::vector<T>& elt : g)
	{
		o << elt;
	}
	return o;
}

//Some usefull functions I need each times

unsigned int secureGetNumber()
{
	std::string buffer;
	std::cin >> buffer;
	return atoi(buffer.c_str());
}

int getFileAndPart(int day, std::ifstream* in, unsigned int* part)
{
	if (in)
	{
		std::string toOpen = "./input" + std::to_string(day) + ".txt";
		in->open(toOpen);
		if (in->fail())
		{
			std::cerr << "Couldn't open file " << toOpen << " : " << strerror(errno) << std::endl;
			return 1;
		}
	}
	if (part)
	{
		std::cout << "wich part ? (1 or 2)\r\n";
		std::cin >> *part;
		if (*part != 2)
			*part = 1;
	}
	return 0;
}

//Usefull class and containers

class Graphe
{
	std::allocator<Graphe> _alloc;
	std::map<char, Graphe*> _childs;
	int _value;
	int _weight;

	int& _addRecu(const std::string& str, int v, std::string::const_iterator cit)
	{
		if (cit == str.end())
		{
			_value = v;
			++_weight;
			return _value;
		}
		else
		{
			++_weight;
			Graphe* child = 0;
			if (_childs.find(*cit) == _childs.end())
			{
				child = _alloc.allocate(1);
				_alloc.construct(child, Graphe());
				_childs[*cit] = child;
			}
			else
				child = _childs[*cit];
			return child->_addRecu(str, v, ++cit);
		}
	}

	int _search(const std::string& str, std::string::const_iterator cit) const
	{
		if (cit == str.end())
		{
			return _value;
		}
		else
		{
			if (_childs.find(*cit) == _childs.end())
			{
				return 0;
			}
			return _childs.find(*cit)->second->_search(str, ++cit);
		}
	}

	void _print(std::string str) const
	{
		if (_value)
			std::cout << str << " : " << _value << std::endl;
		else
		{
			for (std::pair<char, Graphe*> elt : _childs)
			{
				elt.second->_print(str + elt.first);
			}
		}
	}

public:

	Graphe() : _value(0), _weight(0) {}

	~Graphe()
	{
		for (std::map<char, Graphe*>::iterator it = _childs.begin(); it != _childs.end(); ++it)
		{
			_alloc.destroy(it->second);
			_alloc.deallocate(it->second, 1);
			it->second = 0;
		}
	}

	void add(const std::string& str, int v)
	{
		_addRecu(str, v, str.begin());
	}

	const Graphe* browse(char c) const
	{
		std::map<char, Graphe*>::const_iterator find = _childs.find(c);
		if (find != _childs.end())
			return find->second;
		return 0;
	}

	int& operator[](const std::string& str)
	{
		return _addRecu(str, int(), str.begin());
	}

	int operator[](const std::string& str) const
	{
		return _search(str, str.begin());
	}

	int getValue() const
	{
		return _value;
	}

	void print() const
	{
		_print("");
	}
};
