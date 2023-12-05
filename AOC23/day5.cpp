#include "helpers.hpp"

typedef std::pair< unsigned long, unsigned long > Range;
typedef std::set< Range > RangeSet;
typedef std::map< Range, RangeSet > Seeds;

void insertRange(Seeds& seeds, Range r)
{
	unsigned long minR;
	unsigned long maxR;
	unsigned long minElt;
	unsigned long maxElt;

	for (std::pair<Range, RangeSet> elt : seeds)
	{
		minR = r.first;
		maxR = minR + r.second - 1;
		minElt = elt.first.first;
		maxElt = minElt + elt.first.second - 1;
		if (minR > maxElt || maxR < minElt)
			continue;
		if (minR >= minElt)
		{
			if (maxR > maxElt)
			{
				r.first = minElt;
				r.second = maxR - minElt + 1;
				seeds.erase(elt.first);
				seeds[r] = RangeSet();
			}
			return;
		}
		else if (minR < minElt)
		{
			if (maxR < maxElt)
				r.second = maxElt - minR + 1;
			seeds.erase(elt.first);
			seeds[r] = RangeSet();
			return;
		}
	}
	seeds[r] = RangeSet();
}

unsigned long part2(std::ifstream& input)
{
	unsigned long res = ~0;
	Seeds seeds;
	Seeds rests;
	RangeSet temp;
	Range tool;
	std::pair< unsigned long, bool > nbRead;
	unsigned long dest;
	unsigned long source;
	unsigned long range;
	unsigned long minElt;
	unsigned long maxElt;
	unsigned long minS;
	unsigned long maxS;
	char c;
	int loop = 0;

	c = input.get();
	nbRead = getNextNumberOnLineFromStream(input, c);
	while (nbRead.second)
	{
		tool.first = nbRead.first;
		tool.second = getNextNumberOnLineFromStream(input, c).first;
		seeds[tool] = RangeSet();
		nbRead = getNextNumberOnLineFromStream(input, c);
	}
	goToNextLine(input, c);
	goToNextLine(input, c);
	while (c != EOF)
	{
		std::cout << "loop " << ++loop << std::endl;
		nbRead = getNextNumberOnLineFromStream(input, c);
		if (!nbRead.second)
		{
			goToNextLine(input, c);
			goToNextLine(input, c);
			for (std::pair< Range, RangeSet > elt : seeds)
			{
				if (elt.second.empty())
					temp.insert(elt.first);
				else
				{
					for (Range Selt : elt.second)
						temp.insert(Selt);
				}
			}
			seeds.clear();
			for (Range elt : temp)
			{
				insertRange(seeds, elt);
			}
			for (std::pair< Range, RangeSet > elt : rests)
			{
				insertRange(seeds, elt.first);
			}
			rests.clear();
			temp.clear();
		}
		else
		{
			dest = nbRead.first;
			source = getNextNumberOnLineFromStream(input, c).first;
			range = getNextNumberOnLineFromStream(input, c).first;
			for (Seeds::iterator elt = seeds.begin(); elt != seeds.end(); ++elt)
			{
				minElt = elt->first.first;
				maxElt = minElt + elt->first.second - 1;
				minS = source;
				maxS = source + range - 1;
				if (minElt > maxS || maxElt < minS)
					continue;
				if (minElt >= minS && maxElt <= maxS)
				{
					tool.first = dest + minElt - minS;
					tool.second = elt->first.second;
					elt->second.insert(tool);
				}
				else if (minElt >= minS && maxElt > maxS)
				{
					tool.first = dest + minElt - minS;
					tool.second = 1 + maxS - minElt;
					elt->second.insert(tool);
					tool.first = maxS + 1;
					tool.second = maxElt - maxS + 1;
					rests[tool] = RangeSet();
				}
				else if (minElt < minS && maxElt <= maxS)
				{
					tool.first = dest;
					tool.second = maxElt - minS + 1;
					elt->second.insert(tool);
					tool.first = minElt;
					tool.second = minS - minElt;
					rests[tool] = RangeSet();
				}
				else if (minElt < minS && maxElt > maxS)
				{
					tool.first = dest;
					tool.second = range;
					elt->second.insert(tool);
					tool.first = minElt;
					tool.second = minS - minElt;
					rests[tool] = RangeSet();
					tool.first = maxS + 1;
					tool.second = maxElt - maxS;
					rests[tool] = RangeSet();
				}
			}
		}
		goToNextLine(input, c);
	}

	for (Seeds::const_iterator cit = seeds.begin(); cit != seeds.end(); ++cit)
	{
		for (Range r : cit->second)
		{
			if (r.first < res)
				res = r.first;
		}
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
		std::cout << "not 0" << std::endl;
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
	nbRead = getNextNumberOnLineFromStream(input, c);
	while (nbRead.second)
	{
		seeds[nbRead.first] = std::set< unsigned long >();
		nbRead = getNextNumberOnLineFromStream(input, c);
	}
	goToNextLine(input, c);
	goToNextLine(input, c);
	while (c != EOF)
	{
		nbRead = getNextNumberOnLineFromStream(input, c);
		if (!nbRead.second)
		{
			goToNextLine(input, c);
			goToNextLine(input, c);
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
			source = getNextNumberOnLineFromStream(input, c).first;
			range = getNextNumberOnLineFromStream(input, c).first;
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