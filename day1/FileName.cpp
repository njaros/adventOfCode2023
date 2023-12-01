#pragma warning(disable: 4996)

#include <iostream>
#include <fstream>
#include <string>
#include <map>

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



int main()
{
	std::ifstream input("./input.txt");
	if (input.fail())
	{
		std::cerr << "Couldn't open file ./Input.txt : " << strerror(errno) << std::endl;
		return errno;
	}
	int part = 1;
	std::cout << "wich part ? (1 or 2)\r\n";
	std::cin >> part;
	int sum = 0;

	if (part == 1)
	{

		char c;
		int first = -1;
		int last = 0;

		c = input.get();
		while (c != EOF)
		{
			if (c == '\n')
			{
				sum += 10 * first + last;
				first = -1;
			}
			if (isdigit(c))
			{
				if (first == -1)
					first = c - '0';
				last = c - '0';
			}
			c = input.get();
		}
		sum += 10 * first + last;
	}
	else
	{
		Graphe valuesLeft;
		valuesLeft["one"] = 10;
		valuesLeft["two"] = 20;
		valuesLeft["three"] = 30;
		valuesLeft["four"] = 40;
		valuesLeft["five"] = 50;
		valuesLeft["six"] = 60;
		valuesLeft["seven"] = 70;
		valuesLeft["eight"] = 80;
		valuesLeft["nine"] = 90;

		Graphe valuesRight;
		valuesRight["eno"] = 1;
		valuesRight["owt"] = 2;
		valuesRight["eerht"] = 3;
		valuesRight["ruof"] = 4;
		valuesRight["evif"] = 5;
		valuesRight["xis"] = 6;
		valuesRight["neves"] = 7;
		valuesRight["thgie"] = 8;
		valuesRight["enin"] = 9;

		std::string line;
		std::string::const_iterator first;
		std::string::const_reverse_iterator last;
		int forward;
		const Graphe* browser = 0;

		while (!input.eof())
		{
			forward = 0;
			browser = 0;
			getline(input, line);
			first = line.begin();
			last = line.rbegin();
			while (first != line.end())
			{
				if (isdigit(*first))
				{
					sum += (*first - '0') * 10;
					browser = 0;
					break;
				}
				if (browser != 0)
				{
					++forward;
					browser = browser->browse(*first);
					if (!browser)
					{
						first -= forward - 1;
						forward = 0;
						browser = valuesLeft.browse(*first);
					}
					else if (browser->getValue())
					{
						sum += browser->getValue();
						browser = 0;
						break;
					}
				}
				else
					browser = valuesLeft.browse(*first);
				++first;
			}
			browser = 0;
			forward = 0;
			while (last != line.rend())
			{
				if (isdigit(*last))
				{
					sum += (*last - '0');
					break;
				}
				if (browser != 0)
				{
					++forward;
					browser = browser->browse(*last);
					if (!browser)
					{
						last -= forward - 1;
						forward = 0;
						browser = valuesRight.browse(*last);
					}
					else if (browser->getValue())
					{
						sum += browser->getValue();
						break;
					}
				}
				else
					browser = valuesRight.browse(*last);
				++last;
			}
		}
	}
	input.close();
	std::cout << "response : " << sum << "\r\n";
	return 0;
}