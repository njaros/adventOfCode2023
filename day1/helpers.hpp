#pragma once
#pragma warning(disable: 4996)

#include <iostream>
#include <fstream>
#include <string>
#include <map>

unsigned int secureGetNumber()
{
	std::string buffer;
	std::cin >> buffer;
	return atoi(buffer.c_str());
}

int getFileAndPart(int day, std::ifstream* in, unsigned int& part, bool withFile)
{
	if (withFile)
	{
		in->open("./input" + std::to_string(day) + ".txt");
		if (in->fail())
		{
			std::cerr << "Couldn't open file ./Input.txt : " << strerror(errno) << std::endl;
			return errno;
		}
	}
	std::cout << "wich part ? (1 or 2)\r\n";
	std::cin >> part;
	if (part != 2)
		part = 1;
	return 0;
}

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

	void _delete()
	{
		for (std::map<char, Graphe*>::iterator it = _childs.begin(); it != _childs.end(); ++it)
		{
			_alloc.destroy(it->second);
			_alloc.deallocate(it->second, 1);
			it->second = 0;
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
		_delete();
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
