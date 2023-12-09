#include "helpers.hpp"

typedef std::pair< std::string, std::string > Ways;
typedef std::map< std::string, Ways > Map;

std::pair< std::vector< int >, int > PGCDed(const std::vector< int >& v)
{
	std::vector< int > r;
	int divide = v[0] / 2;
	bool broke;

	while (divide > 1)
	{
		broke = false;
		for (int p : v)
		{
			while (p > 0)
				p -= divide;
			if (p < 0)
			{
				broke = true;
				break;
			}
		}
		if (!broke)
		{
			for (const int& p : v)
				r.push_back(p / divide);
			return std::make_pair(r, divide);
		}
		--divide;
	}
	return std::make_pair(v, divide);
}

int day8()
{
	std::ifstream input;
	unsigned int part;
	if (getFileAndPart(8, &input, &part))
		return errno;

	std::string instruction;
	std::string line;
	std::string pos;
	std::vector< std::string > posVec;
	Map map;
	Ways ways;
	getline(input, instruction);
	inputLib::goToNextLine(input);

	while (!input.eof())
	{
		getline(input, line);
		pos = line.substr(0, 3);
		ways.first = line.substr(7, 3);
		ways.second = line.substr(12, 3);
		if (part == 2 && pos[2] == 'A')
			posVec.push_back(pos);
		map[pos] = ways;
	}

	std::string::size_type idx = 0;
	unsigned long long result = 0;
	if (part == 1)
	{
		pos = "AAA";

		while (pos != "ZZZ")
		{
			++result;
			if (instruction[idx] == 'L')
				pos = map[pos].first;
			else
				pos = map[pos].second;
			if (++idx >= instruction.size())
				idx = 0;
		}
	}
	else
	{
		std::vector< int > resVec;
		for (std::string& str : posVec)
		{
			result = 0;
			idx = 0;
			while (str[2] != 'Z')
			{
				++result;
				if (instruction[idx] == 'L')
					str = map[str].first;
				else
					str = map[str].second;
				if (++idx >= instruction.size())
					idx = 0;
			}
			resVec.push_back((int)result);
		}

		std::pair< std::vector< int >, int > reduced = PGCDed(resVec);
		
		result = reduced.second;

		for (int nb : reduced.first)
			result *= nb;
	}
	std::cout << "result is " << result << std::endl;
	return 0;
}