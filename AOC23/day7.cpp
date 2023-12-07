#include "helpers.hpp"

#define HIGH_CARD 0
#define ONE_PAIR 1
#define TWO_PAIR 2
#define THREE_OF_A_KIND 3
#define FULL_HOUSE 4
#define FOUR_OF_A_KIND 5
#define FIVE_OF_A_KIND 6

char toComparableValue(char c, unsigned int part)
{
	if (isdigit(c))
		return c;
	switch (c)
	{
	case 'T':
		return 'T';
	case 'J':
		if (part == 1)
			return 'U';
		else
			return '1';
	case 'Q':
		return 'V';
	case 'K':
		return 'W';
	case 'A':
		return 'X';
	default:
		throw(std::domain_error("bad input"));
	}
}

class CamelHand
{
public:

	char cards[5];
	unsigned int part;

	CamelHand() : part(0)
	{
		for (int i = 0; i < 5; ++i)
			cards[i] = 0;
	}
	CamelHand(const char c[5], unsigned int ePart) : part(ePart)
	{
		for (int i = 0; i < 5; ++i)
			cards[i] = c[i];
	}
	CamelHand(const CamelHand& o) : CamelHand(o.cards, o.part) {}
	~CamelHand() {}

	CamelHand& operator=(const CamelHand& o)
	{
		for (int i = 0; i < 5; ++i)
			cards[i] = o.cards[i];
		part = o.part;
		return *this;
	}

	CamelHand& operator=(char o[5])
	{
		for (int i = 0; i < 5; ++i)
			cards[i] = o[i];
		return *this;
	}

	short getType() const
	{
		std::map<char, int>::iterator JFind;
		std::map<char, int>::iterator itMax;
		int max = 0;
		int JCount = 0;
		std::map<char, int> sorted;
		for (int i = 0; i < 5; ++i)
		{
			sorted[cards[i]] += 1;
		}
		if (part != 1)
		{
			if (sorted.size() > 1)
			{
				JFind = sorted.find('J');
				if (JFind != sorted.end())
				{
					JCount = JFind->second;
					for (std::map<char, int>::iterator it = sorted.begin(); it != sorted.end(); ++it)
					{
						if (it->first != 'J' && it->second > max)
						{
							max = it->second;
							itMax = it;
						}
					}
					itMax->second += JCount;
					sorted.erase(JFind);
				}
			}
		}
		switch (sorted.size())
		{
		case 1:
			return FIVE_OF_A_KIND;
		case 2:
			if (sorted.begin()->second / 2 == 1)
				return FULL_HOUSE;
			return FOUR_OF_A_KIND;
		case 3:
			for (std::map<char, int>::const_iterator cit = sorted.begin(); cit != sorted.end(); ++cit)
			{
				if (cit->second == 2)
					return TWO_PAIR;
				if (cit->second == 3)
					return THREE_OF_A_KIND;
			}
			throw(std::domain_error("Code error."));
		case 4:
			return ONE_PAIR;
		default:
			return HIGH_CARD;
		}
	}
};

bool operator<(const CamelHand& lhs, const CamelHand& rhs)
{
	if (lhs.getType() < rhs.getType())
		return true;
	if (rhs.getType() < lhs.getType())
		return false;
	int i = 0;
	while (lhs.cards[i] == rhs.cards[i] && i < 5)
		++i;
	if (i < 5)
		return (toComparableValue(lhs.cards[i], lhs.part) < toComparableValue(rhs.cards[i], rhs.part));
	return false;
}

typedef std::map<CamelHand, int> Game;

int day7()
{
	std::ifstream input;
	unsigned int part;
	if (getFileAndPart(7, &input, &part))
		return errno;

	Game game;
	char cards[5];
	CamelHand handRead;
	handRead.part = part;
	int bidRead;
	long result = 0;
	std::string line;
	while (!input.eof())
	{
		getline(input, line);
		for (std::string::size_type idx = 0; idx < 5; ++idx)
		{
			cards[idx] = line[idx];
		}
		handRead = cards;
		bidRead = atoi(&line[6]);
		game.insert(std::make_pair(handRead, bidRead));
	}
	long score = 1;
	for (Game::const_iterator cit = game.begin(); cit != game.end(); ++cit)
	{
		result += score++ * cit->second;
	}

	std::cout << "result is " << result << std::endl;
	return 0;
}