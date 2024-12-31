//
// Created by njaros on 12/4/23.
//

#include "helpers.hpp"

int getNumber(std::ifstream& input, char& c)
{
	int nb = 0;
	while (c == ' ')
		c = input.get();
	while (isdigit(c))
	{
		nb *= 10;
		nb += c - '0';
		c = input.get();
	}
	return nb;
}

int calculatePoints(std::set<int>& win, std::map<int, int>& nbs, unsigned int part, std::deque<int>& copies)
{
	int res = 0;
	int copy = 0;
	for (std::pair<int, int> elt : nbs)
	{
		if (win.find(elt.first) != win.end())
		{
			if (part == 1)
			{
				if (!res)
				{
					res = static_cast<int>(pow(2, elt.second - 1));
				}
				else {
					res *= static_cast<int>(pow(2, elt.second));
				}
			}
			else
				++res;
		}
	}
	nbs.clear();
	win.clear();
	if (part == 1)
		return res;
	if (!copies.empty())
	{
		copy = copies.front();
		copies.pop_front();
	}
	for (int i = 0; i < res; ++i)
	{
		if (i < copies.size())
			copies[i] += 1 + copy;
		else
			copies.push_back(1 + copy);
	}
	return 1 + copy;
}

int day4() {
	unsigned int part = 0;
	std::ifstream input;
	if (getFileAndPart(4, input, part)) {
		return errno;
	}

	std::set<int> win;
	std::map<int, int> nbs;
	std::deque<int> copies;
	bool start = false;
	bool isNb = false;
	int numberRead;
	int sum = 0;
	char c;
	c = input.get();
	while (c != EOF)
	{
		if (start)
		{
			if (c == '\n')
			{
				sum += calculatePoints(win, nbs, part, copies);
				start = false;
				isNb = false;
				c = input.get();
			}
			else if (c == '|')
			{
				isNb = true;
				c = input.get();
			}
			else
			{
				numberRead = getNumber(input, c);
				if (isNb)
					nbs[numberRead] += 1;
				else
					win.insert(numberRead);
			}
		}
		else if (c == ':')
		{
			start = true;
			c = input.get();
		}
		else
			c= input.get();
	}
	sum += calculatePoints(win, nbs, part, copies);
	std::cout << "result is " << sum << std::endl;
	input.close();
	return 0;
}
