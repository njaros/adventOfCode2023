//
// Created by njaros on 12/4/23.
//

#include "helpers.hpp"

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

Coord& operator*=(Coord& c, int i)
{
	c.first *= i;
	c.second *= i;
	return c;
}

Coord operator*(const Coord& c, int i)
{
	Coord cpy(c);
	return (cpy *= i);
}

Coord& operator/=(Coord& c, int i)
{
	c.first /= i;
	c.second /= i;
	return c;
}

Coord operator/(const Coord& c, int i)
{
	Coord cpy(c);
	return (cpy /= i);
}

std::ostream& operator<<(std::ostream& o, const Coord& c)
{
	return o << '(' << c.first << ", " << c.second << ')';
}

long manhattanDist(const Coord& a, const Coord& b)
{
	return (labs(a.first - b.first) + labs(a.second - b.second));
}

unsigned int secureGetNumber()
{
	std::string buffer;
	std::cin >> buffer;
	return atoi(buffer.c_str());
}

int getFileAndPart(int day, std::ifstream& in, unsigned int& part)
{
	std::string toOpen = "./input" + std::to_string(day) + ".txt";
	in.open(toOpen);
	if (in.fail())
	{
		std::cerr << "Couldn't open file " << toOpen << " : " << strerror(errno) << std::endl;
		return 1;
	}
	if (part != 5)
	{
		std::cout << "which part ? (1 or 2)\r\n";
		std::cin >> part;
		if (part != 2)
			part = 1;
	}
	return 0;
}

long long intPow(long long a, long long b)
{
	long long res = 1;

	if (b < 0)
		return 0;
	while (b--)
		res *= a;
	return res;
}

ull ullPow(ull a, ull b)
{
	ull res = 1;

	while (b--)
		res *= a;
	return res;
}

ull concatNumbers(ull a, ull b) {
	int exp = 1;
	ull tmp = b;

	while (tmp / 10) {
		++exp;
		tmp /= 10;
	}

	return (a * ullPow(10, exp) + b);
}

std::optional< long long > divisible(long long a_, long long b_) /*work in progress*/
{
	long result = 1;
	int exp = 0;
	unsigned long long a;
	unsigned long long b;
	bool sign = ((a_ < 0) != (b_ < 0));
	unsigned long long bStart;
	unsigned long long bitReader = ((long long)1) << (0x3f);

	std::cerr << "divisible function isn't finished !!\n";
	if (!b_)
		return std::nullopt;
	if (!a_)
		return 0;
	if (a_ < 0)
		a = -a_;
	else
		a = a_;
	if (b_ < 0)
		b = -b_;
	else
		b = b_;
	if (a < b)
		return std::nullopt;

	while (!(bitReader & b))
		bitReader >>= 1;
	bStart = bitReader;
	while (bitReader)
	{
		if ((bitReader & b) != (bitReader & a))
			return std::nullopt;
		--bitReader;
	}
	bitReader = bStart << 1;
	while (bitReader)
	{
		if (bitReader & a)
			result += intPow(2, exp) + 1;
		bitReader <<= 1;
	}
	return result;
}

ui math::ManhattanDist(const Coord& a, const Coord& b)
{
	return (labs(a.first - b.first) + labs(a.second - b.second));
}

/**
 * @brief Modulo which handle negative modulos
 * 
 * @param a 
 * @param b 
 * @return long 
 */
long math::myModulo(long a, long b) {
	if (b <= 0)
		return -1;
	if (a >= 0)
		return a % b;
	else
		return (b - (-a % b)) % b;
}

std::pair<long long, bool> inputLib::extractNextNumber(std::ifstream& input, char& monitorChar)
{
	long long res = 0;
	long long sign = 1;
	while (monitorChar != EOF)
	{
		if (isdigit(monitorChar))
		{
			while (isdigit(monitorChar))
			{
				res *= 10;
				res += monitorChar - '0';
				monitorChar = input.get();
			}
			return std::make_pair(res * sign, true);
		}
		if (monitorChar != '+' && monitorChar != '-')
			sign = 1;
		else if (monitorChar == '-')
			sign = ~sign;
		monitorChar = input.get();
	}
	return std::make_pair(res, false);
}

inputLib::extracted inputLib::extractNextNumber(std::ifstream& input)
{
	char monitorChar = input.get();
	long long res = 0;
	long long sign = 1;
	while (monitorChar != EOF)
	{
		if (isdigit(monitorChar))
		{
			while (isdigit(monitorChar))
			{
				res *= 10;
				res += monitorChar - '0';
				monitorChar = input.get();
			}
			return std::make_pair(res * sign, monitorChar);
		}
		if (monitorChar != '+' && monitorChar != '-')
			sign = 1;
		else if (monitorChar == '-')
			sign *= -1;
		monitorChar = input.get();
	}
	return std::make_pair(std::nullopt, monitorChar);
}

inputLib::extracted inputLib::extractNextNumberEndline(std::ifstream& input)
{
	char monitorChar = input.get();
	long long res = 0;
	long long sign = 1;
	while (monitorChar != EOF && monitorChar != '\n')
	{
		if (isdigit(monitorChar))
		{
			while (isdigit(monitorChar))
			{
				res *= 10;
				res += monitorChar - '0';
				monitorChar = input.get();
			}
			return std::make_pair(res * sign, monitorChar);
		}
		if (monitorChar != '+' && monitorChar != '-')
			sign = 1;
		else if (monitorChar == '-')
			sign *= -1;
		monitorChar = input.get();
	}
	return std::make_pair(std::nullopt, monitorChar);
}

char inputLib::goToNextLine(std::ifstream& input, char& monitorChar, unsigned int times)
{
	while (times-- && monitorChar != EOF)
	{
		while (monitorChar != '\n' && monitorChar != EOF)
			monitorChar = input.get();
		if (monitorChar == '\n')
			monitorChar = input.get();
	}
	return monitorChar;
}

void inputLib::goToNextLine(std::ifstream& input, unsigned int times)
{
	char monitorChar = input.get();
	while (times-- && monitorChar != EOF)
	{
		while (monitorChar != '\n' && monitorChar != EOF)
			monitorChar = input.get();
	}
}

std::string& inputLib::carriageReturnDel(std::string& line) {
	std::string::iterator it = line.begin();
	while (it != line.end())
	{
		if (*it == '\r')
			it = line.erase(it);
		else
			++it;
	}
	return line;
}

std::vector<std::string> inputLib::split(const std::string& str, const char* delim) {
	char* cStr;
	char* token;
	std::vector<std::string> splitted;

	cStr = strdup(str.c_str());
	token = strtok(cStr, delim);
	while (token) {
		splitted.push_back(token);
		token = strtok(NULL, delim);
	}
	free(cStr);
	return splitted;
}

namespace experiment
{

	MyNatural::MyNatural()
	{
		for (int i = 0; i < _bitSize; ++i)
			_n[i] = false;
	}

	MyNatural::MyNatural(unsigned int n)
	{
		unsigned int	compareTo = 1;
		for (int i = 0; i < _bitSize; ++i)
		{
			_n[_bitSize - 1 - i] = n & compareTo;
			compareTo <<= 1;
		}
	}

	MyNatural::MyNatural(const MyNatural &other)
	{
		*this = other;
	}

	MyNatural&	MyNatural::operator=(const MyNatural &other)
	{
		if (&other != this) {
			for (int i = 0; i < _bitSize; ++i)
				_n[i] = other._n[i];
		}
		return *this;
	}

	const bool*	MyNatural::getData() const {return _n;}
	int	MyNatural::getSize() const {return _bitSize;}
	unsigned int	MyNatural::getUnsignedInt() const
	{
		unsigned int	displayable = 0;
		unsigned int	constructor = 1;
		for (int i = 0; i < _bitSize; ++i)
		{
			if (_n[_bitSize - 1 - i])
				displayable |= constructor;
			constructor <<= 1;
		}
		return displayable;
	}

	MyNatural&	MyNatural::operator+=(const MyNatural &rhs)
	{
		for (int i = _bitSize - 1; i >= 0; --i)
		{
			if (rhs._n[i])
			{
				if (!_n[i])
					_n[i] = true;
				else
				{
					int j = i;
					while (_n[j] && j >= 0)
					{
						_n[j] = false;
						--j;
					}
					if (j >= 0)
						_n[j] = true;
				}
			}
		}
		return *this;
	}

	std::ostream&	operator<<(std::ostream &o, const MyNatural &n)
	{
		unsigned int	displayable = 0;
		unsigned int	constructor = 1;
		int	bitSize = n.getSize();
		const bool	*data = n.getData();
		o << '[';
		for (int i = 0; i < bitSize; ++i)
		{
			if (i && (!(i % 4)))
				o << ' ';
			o << data[i];
			if (data[bitSize - 1 - i])
				displayable |= constructor;
			constructor <<= 1;
		}
		o << "] = " << displayable;
		return o;
	}
}