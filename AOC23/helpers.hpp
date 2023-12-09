#ifndef HELPERS_HPP
#define HELPERS_HPP
#pragma warning(disable: 4996)

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <math.h>
#include <algorithm>
#include <tuple>
#include <map>
#include <set>
#include <vector>
#include <deque>
#include <optional>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>

//Usefull typedefs

typedef std::pair<int, int> Coord;
typedef std::pair<long, bool> NumberRead;

//Some << overloads

Coord& operator+=(Coord& lhs, const Coord& rhs);
Coord& operator-=(Coord& lhs, const Coord& rhs);
Coord operator+(const Coord& lhs, const Coord& rhs);
Coord operator-(const Coord& lhs, const Coord& rhs);
template <class T>
std::ostream& operator<<(std::ostream& o, const std::vector<T>& v)
{
	for (const T& elt : v)
	{
		o << elt;
	}
	return o;
}

template <class T>
std::ostream& operator<<(std::ostream& o, const std::set<T>& s)
{
	for (typename std::set<T>::const_iterator cit = s.begin(); cit != s.end(); ++cit)
	{
		o << *cit << ' ';
	}
	return o;
}

std::ostream& operator<<(std::ostream& o, const Coord& c);

template <class T, class U>
std::ostream& operator<<(std::ostream& o, const std::map<T, U>& m)
{
	for (typename std::map<T, U>::const_iterator cit = m.begin(); cit != m.end(); ++cit)
	{
		o << cit->first << " | " << cit->second << std::endl;
	}
	return o;
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

unsigned int secureGetNumber();

int getFileAndPart(int day, std::ifstream* in, unsigned int* part);

namespace inputLib
{

	std::pair<long, bool> ExtractNextNumber(std::ifstream& input, char& monitorChar);

	std::pair< std::optional<long> , char> ExtractNextNumber(std::ifstream& input);

	char goToNextLine(std::ifstream& input, char& monitorChar, unsigned int times = 1);

	void goToNextLine(std::ifstream& input, unsigned int times = 1);

}
//Usefull class and containers

template<class T, class U>
class Graphe
{
	std::allocator< Graphe < T, U > > Alloc;
	std::allocator_traits< std::allocator< Graphe< T, U > > > _alloc;
	std::map< T, Graphe< T, U >* > _childs;
	U _value;
	size_t _weight;
	bool _valueReadable;

	template<class container>
	U& _addRecu(const container& str, const U& v, typename container::const_iterator cit, bool override)
	{
		bool writeValue = override;
		if (cit == str.end())
		{
			_valueReadable = true;
			if (override)
				_value = v;
			return _value;
		}
		else
		{
			Graphe* child = 0;
			if (_childs.find(*cit) == _childs.end())
			{
				child = _alloc.allocate(Alloc, 1);
				_alloc.construct(Alloc, child, Graphe< T, U >());
				_childs[*cit] = child;
				writeValue = true;
			}
			else
				child = _childs[*cit];
			return child->_addRecu(str, v, ++cit, writeValue);
		}
	}

	template<class container>
	std::pair<U, bool> _search(const container& c, typename container::const_iterator cit) const
	{
		if (cit == c.end())
		{
			if (_valueReadable)
				return std::make_pair(_value, true);
			return std::make_pair(U(), false);
		}
		else
		{
			if (_childs.find(*cit) == _childs.end())
				return std::make_pair(U(), false);
			return _childs.find(*cit)->second->_search(c, ++cit);
		}
	}

	void _print(std::vector< T > v) const
	{
		if (_valueReadable)
			std::cout << v << " : " << _value << std::endl;
		for (std::pair< T, Graphe< T, U >* > elt : _childs)
		{
			v.push_back(elt.first);
			elt.second->_print(v);
			v.pop_back();
		}
	}

public:

	Graphe() : _value(U()), _weight(0), _valueReadable(false) {}

	~Graphe()
	{
		for (typename std::map< T, Graphe< T, U>* >::iterator it = _childs.begin(); it != _childs.end(); ++it)
		{
			_alloc.destroy(Alloc, it->second);
			_alloc.deallocate(Alloc, it->second, 1);
			it->second = 0;
		}
	}

	template <class container>
	void add(const container& c, U v)
	{
		_addRecu(c, v, c.begin(), true);
	}

	const Graphe< T, U >* browse(const T& c) const
	{
		typename std::map< T, Graphe< T, U >* >::const_iterator find = _childs.find(c);
		if (find != _childs.end())
			return find->second;
		return 0;
	}

	U& operator[](const char* str)
	{
		std::string cont(str);
		return _addRecu(cont, U(), cont.begin(), false);
	}

	U operator[](const char* str) const
	{
		std::string cont(str);
		return _search(cont, cont.begin()).first;
	}

	template <class container>
	U& operator[](const container& c)
	{
		return _addRecu(c, U(), c.begin(), false);
	}

	template <class container>
	U operator[](const container& c) const
	{
		return _search(c, c.begin()).first;
	}

	U getValue() const
	{
		return _value;
	}

	bool valueReadable() const
	{
		return _valueReadable;
	}

	void print() const
	{
		_print(std::vector< T >());
	}
};

#endif