#include "helpers.hpp"

typedef std::pair< unsigned long long, unsigned long long > TimeDist;
typedef std::vector< TimeDist > Sheet;

int parse(Sheet& sheet, unsigned int& part)
{
	std::ifstream input;
	if (getFileAndPart(6, &input, &part))
		return errno;

	TimeDist entry;
	char c = 0;
	if (part == 1)
	{
		NumberRead nbr = inputLib::ExtractNextNumber(input, c);
		while (nbr.second)
		{
			entry.first = nbr.first;
			sheet.push_back(entry);
			nbr = inputLib::ExtractNextNumber(input, c);
		}
		inputLib::goToNextLine(input, c);
		nbr = inputLib::ExtractNextNumber(input, c);
		Sheet::size_type idx = 0;
		while (nbr.second)
		{
			sheet[idx++].second = nbr.first;
			nbr = inputLib::ExtractNextNumber(input, c);
		}
	}
	else
	{
		entry.first = 0;
		entry.second = 0;
		std::string line;
		getline(input, line);
		for (char c : line)
		{
			if (isdigit(c))
			{
				entry.first *= 10;
				entry.first += c - '0';
			}
		}
		getline(input, line);
		for (char c : line)
		{
			if (isdigit(c))
			{
				entry.second *= 10;
				entry.second += c - '0';
			}
		}
		sheet.push_back(entry);
	}
	return 0;
}

int day6()
{
	unsigned int part;

	Sheet sheet;
	if (parse(sheet, part))
		return errno;

	long long res = 1;
	unsigned int idx;
	long long partRes;
	unsigned long long dist;
	unsigned long long prevDist;
	bool stop;
	for (TimeDist td : sheet)
	{
		idx = 1;
		stop = false;
		partRes = 0;
		prevDist = 0;
		while (idx < td.first && !stop)
		{
			dist = idx * (td.first - idx);
			if (dist <= td.second && dist <= prevDist)
				stop = true;
			else if (dist > td.second)
				++partRes;
			prevDist = dist;
			++idx;
		}
		res *= partRes;
	}

	std::cout << "result is : " << res << std::endl;
	return 0;
}