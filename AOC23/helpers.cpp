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

std::ostream& operator<<(std::ostream& o, const Coord& c)
{
    return o << '(' << c.first << ", " << c.second << ')';
}

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
        std::cout << "which part ? (1 or 2)\r\n";
        std::cin >> *part;
        if (*part != 2)
            *part = 1;
    }
    return 0;
}

std::pair<long, bool> getNumberFromStream(std::ifstream& input, char& c)
{
    long res = 0;
    long sign = 1;
    while (c == ' ')
    {
        c = input.get();
    }
    while (c == '+' || c == '-')
    {
        if (c == '-')
            ~~sign;
        c = input.get();
    }
    if (!isdigit(c))
        return std::make_pair(0, false);
    while (isdigit(c))
    {
        res *= 10;
        res += c + '0';
        c = input.get();
    }
    return std::make_pair(res * sign, true);
}