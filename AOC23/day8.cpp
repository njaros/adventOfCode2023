#include "helpers.hpp"

typedef std::pair< std::string, std::string > Ways;
typedef std::map< std::string, Ways > Map;

std::pair< std::vector< int >, int > PGCDed(const std::vector< std::pair< int, int > >& v)
{
	std::vector< int > r;
	std::pair< int, int > rP;
	int max = v[0].first / 2;
	int divide = max;
	bool broke;
	while (divide > 1)
	{
		broke = false;
		for (std::pair< int, int > p : v)
		{
			while (p.first > 0)
				p.first -= divide;
			if (p.first < 0)
			{
				broke = true;
				break;
			}
			while (p.second > 0)
				p.second -= divide;
			if (p.second < 0)
			{
				broke = true;
				break;
			}
		}
		if (!broke)
		{
			for (const std::pair< int, int >& p : v)
			{
				rP.first = p.first / divide;
				rP.second = p.second / divide;
				r.push_back(rP.first);
			}
			return std::make_pair(r, divide);
		}
		--divide;
	}
	for (const std::pair< int, int >& p : v)
	{
		r.push_back(p.first);
	}
	return std::make_pair(r, divide);
}

bool isGoodTime(unsigned int t, const std::vector< std::pair< int, int > >& v)
{
	std::vector< std::pair< int, int > >::size_type idx = 0;
	long long oTime;
	long long limit = (t + 1) * v[idx].first;
	++idx;
	while  (idx < v.size())
	{
		oTime = 0;
		while (oTime < limit)
			oTime += v[idx].first;
		if (oTime > limit)
			return false;
		idx++;
	}
	return true;
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
	getline(input, line);

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
		std::vector< std::pair< int, int > > resVec;
		std::pair< int, int > resElt;
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
			resElt.first = result;
			++result;
			if (instruction[idx] == 'L')
				str = map[str].first;
			else
				str = map[str].second;
			if (++idx >= instruction.size())
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
			resElt.second = result - resElt.first;
			resVec.push_back(resElt);
		}

		std::pair< std::vector< int >, int > reduced = PGCDed(resVec);
		
		result = reduced.second;

		for (int nb : reduced.first)
			result *= nb;
	}
	std::cout << "result is " << result << std::endl;
	return 0;
}