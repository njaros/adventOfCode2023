#include "helpers.hpp"

using namespace inputLib;

typedef std::pair< unsigned long, unsigned long > Range;
typedef std::set< Range > RangeSet;
typedef std::map< Range, RangeSet > Seeds;
typedef std::vector< Range > Todos;

void insertNotMatch(RangeSet& notMatch, const RangeSet& match, Range r)
{
	Range tool;

	for (Range elt : match)
	{
		/*
				[---]
			[-----------]
		*/
		if (r.first >= elt.first && r.second <= elt.second)
			return;
		/*
			[---]			||			[---]
					[---]	||	[---]
		*/
		if (r.second < elt.first || r.first > elt.second)
			continue;
		/*
			[-----]
				[----]
		*/
		if (r.first <= elt.first && r.second <= elt.second)
		{
			r.second = elt.first - 1;
		}
		/*
				[-----]
			[-----]
		*/
		else if (r.first >= elt.first && r.second >= elt.second)
		{
			r.first = elt.second + 1;
		}
		/*
			[-----------]
				[---]
		*/
		else if (r.first < elt.first && r.second > elt.second)
		{
			tool.first = elt.second + 1;
			tool.second = r.second;
			insertNotMatch(notMatch, match, tool);
			r.second = elt.first - 1;
		}
	}
	notMatch.insert(r);
}

void insertMatch(RangeSet& notMatch, RangeSet& match, Range r)
{
	Range tool;
	RangeSet toAddWhenFinished;
	RangeSet::iterator it = notMatch.begin();
	RangeSet::iterator toDelete;

	match.insert(r);
	while (it != notMatch.end())
	{
		/*
			[---]			||			[---]
					[---]	||	[---]
		*/
		if (r.second < it->first)
			return;
		if (r.first > it->second)
		{
			it++;
		}
		/*
			[-----------]
				[---]
		*/
		else if (r.first <= it->first && r.second >= it->second)
		{
			toDelete = it++;
			notMatch.erase(toDelete);
		}
		/*
				[---]
			[-----------]
		*/
		else if (r.first > it->first && r.second < it->second)
		{
			tool.first = it->first;
			tool.second = r.first - 1;
			toAddWhenFinished.insert(tool);
			tool.first = r.second + 1;
			tool.second = it->second;
			toAddWhenFinished.insert(tool);
			toDelete = it++;
			notMatch.erase(toDelete);
		}
		/*
			[-----]
				[----]
		*/
		else if (r.first <= it->first && r.second <= it->second)
		{
			tool.first = r.second + 1;
			tool.second = it->second;
			toAddWhenFinished.insert(tool);
			toDelete = it++;
			notMatch.erase(toDelete);
		}
		/*
				[-----]
			[-----]
		*/
		else if (r.first >= it->first && r.second >= it->second)
		{
			tool.first = it->first;
			tool.second = r.first - 1;
			toAddWhenFinished.insert(tool);
			toDelete = it++;
			notMatch.erase(toDelete);
		}
	}
	notMatch.insert(toAddWhenFinished.begin(), toAddWhenFinished.end());
}

void manageNextTodos(const Todos& todo, Todos& todoNext, RangeSet& match, RangeSet& notMatch, unsigned long minS, unsigned long maxS, unsigned long destStart)
{
	Range tool;
	for (Range elt : todo)
	{
		/*
			[---]			||			[---]
					[---]	||	[---]
		*/
		if (elt.second < minS || elt.first > maxS)
		{
			insertNotMatch(notMatch, match, elt);
		}
		/*
			[-----]
				[----]
		*/
		else if (elt.first >= minS && elt.second >= maxS)
		{
			tool.first = minS;
			tool.second = elt.second;
			insertMatch(notMatch, match, tool);
			tool.first = destStart;
			tool.second = destStart + (elt.second - minS);
			todoNext.push_back(tool);
			tool.first = elt.first;
			tool.second = minS - 1;
			insertNotMatch(notMatch, match, tool);
		}
		/*
				[-----]
			[-----]
		*/
		else if (elt.first <= minS && elt.second <= maxS)
		{
			tool.first = elt.first;
			tool.second = maxS;
			insertMatch(notMatch, match, tool);
			tool.first = destStart + (elt.first - minS);
			tool.second = tool.first + (maxS - elt.first);
			todoNext.push_back(tool);
			tool.first = maxS + 1;
			tool.second = elt.second;
			insertNotMatch(notMatch, match, tool);
		}
		/*
			[--------]
				[---]
		*/
		else if (elt.first < minS && elt.second > maxS)
		{
			tool.first = minS;
			tool.second = maxS;
			insertMatch(notMatch, match, tool);
			tool.first = destStart;
			tool.second = destStart + (maxS - minS);
			todoNext.push_back(tool);
			tool.first = elt.first;
			tool.second = minS - 1;
			insertNotMatch(notMatch, match, tool);
			tool.first = maxS + 1;
			tool.second = elt.second;
			insertNotMatch(notMatch, match, tool);
		}
		/*
				[---]
			[-----------]
		*/
		else
		{
			insertMatch(notMatch, match, elt);
			tool.first = destStart + (elt.first - minS);
			tool.second = tool.first + (elt.second - elt.first);
			todoNext.push_back(tool);
		}
	}
}

unsigned long part2(std::ifstream& input)
{
	unsigned long res = ~0;
	unsigned long sourceMin;
	unsigned long sourceMax;
	unsigned long destStart;
	Todos todo;
	Todos todoNext;
	RangeSet notMatched;
	RangeSet matched;
	Range rangeRead;
	std::pair< unsigned long, bool > nbRead;
	char c;
	bool anoeud = true;
	c = input.get();

	nbRead = ExtractNextNumber(input, c);
	while (nbRead.second)
	{
		if (anoeud)
			rangeRead.first = nbRead.first;
		else
		{
			rangeRead.second = rangeRead.first + nbRead.first - 1;
			todo.push_back(rangeRead);
		}
		anoeud = !anoeud;
		nbRead = ExtractNextNumber(input, c);
	}
	goToNextLine(input, c, 2);
	while (c != EOF)
	{
		if (goToNextLine(input, c) == '\n')
		{
			matched.clear();
			todo = todoNext;
			todo.insert(todo.end(), notMatched.begin(), notMatched.end());
			todoNext.clear();
			notMatched.clear();
		}
		nbRead = ExtractNextNumber(input, c);
		if (nbRead.second)
		{
			destStart = nbRead.first;
			sourceMin = ExtractNextNumber(input, c).first;
			sourceMax = ExtractNextNumber(input, c).first + sourceMin - 1;
			manageNextTodos(todo, todoNext, matched, notMatched, sourceMin, sourceMax, destStart);
		}
	}
	if (!notMatched.empty())
		res = notMatched.begin()->first;
	for (Range r : todoNext)
	{
		if (r.first < res)
			res = r.first;
	}
	return res;
}

int day5()
{
	unsigned int part;
	std::ifstream input;
	if (getFileAndPart(5, &input, &part))
		return errno;
	if (part == 2)
	{
		std::cout << "result is " << part2(input) << std::endl;
		std::cout << "not 0, 46692542" << std::endl;
		input.close();
		return 0;
	}
	char c;
	std::map< unsigned long, std::set < unsigned long > > seeds;
	std::set< unsigned long > temp;
	std::pair< unsigned long, bool > nbRead;
	unsigned long dest;
	unsigned long source;
	unsigned long range;
	unsigned long result = ~0;
	c = input.get();
	nbRead = ExtractNextNumber(input, c);
	while (nbRead.second)
	{
		seeds[nbRead.first] = std::set< unsigned long >();
		nbRead = ExtractNextNumber(input, c);
	}
	goToNextLine(input, c, 2);
	while (c != EOF)
	{
		nbRead = ExtractNextNumber(input, c);
		if (!nbRead.second)
		{
			goToNextLine(input, c, 2);
			for (std::pair< unsigned long, std::set< unsigned long > > elt : seeds)
			{
				if (elt.second.empty())
					temp.insert(elt.first);
				else
				{
					for (unsigned long Selt : elt.second)
						temp.insert(Selt);
				}
			}
			seeds.clear();
			for (unsigned long elt : temp)
			{
				seeds[elt] = std::set< unsigned long >();
			}
			temp.clear();
		}
		else
		{
			dest = nbRead.first;
			source = ExtractNextNumber(input, c).first;
			range = ExtractNextNumber(input, c).first;
			for (std::map< unsigned long, std::set< unsigned long > >::iterator elt = seeds.begin(); elt != seeds.end(); ++elt)
			{
				if (elt->first >= source && elt->first < source + range)
					elt->second.insert(dest + (elt->first - source));
			}
		}
		goToNextLine(input, c);
	}

	for (std::pair< unsigned long, std::set< unsigned long > > elt : seeds)
	{
		for (unsigned long dist : elt.second)
		{
			if (dist < result)
				result = dist;
		}
	}

	std::cout << "result is " << result << std::endl;
	input.close();
	return 0;
}