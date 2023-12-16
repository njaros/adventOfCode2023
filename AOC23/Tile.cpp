#include "Tile.hpp"

Tile::Tile() : type('.'), energized(0) {}
Tile::Tile(const Tile& o) : type(o.type), energized(o.energized), mem(o.mem) {}
Tile::Tile(char t) : type(t), energized(0) {}
Tile::~Tile() {}

Tile& Tile::operator=(const Tile& other)
{
	type = other.type;
	mem = other.mem;
	return *this;
}

std::pair<bool, bool> Tile::out(Beam& b)
{
	Beam ew;
	Coord tmp;

	if (mem.find(b.direction) != mem.end())
		return std::make_pair(false, false);
	++energized;
	tmp = b.direction;
	switch (type)
	{
	case '-':
		if (b.direction == Coord(0, 1) || b.direction == Coord(0, -1))
		{
			if ((mem.find(b.direction) != mem.end()) || (mem.find(b.direction * -1) != mem.end()))
				return std::make_pair(false, false);
			b.direction = Coord(1, 0);
			ew = b;
			b.direction *= -1;
			b.allBeams->push_back(ew);
		}
		break;
	case '|':
		if (b.direction == Coord(1, 0) || b.direction == Coord(-1, 0))
		{
			if ((mem.find(b.direction) != mem.end()) || (mem.find(b.direction * -1) != mem.end()))
				return std::make_pair(false, false);
			b.direction = Coord(0, 1);
			ew = b;
			b.direction *= -1;
			b.allBeams->push_back(ew);
		}
		break;
	case '\\':
		if (b.direction == Coord(1, 0))
			b.direction = Coord(0, 1);
		else if (b.direction == Coord(-1, 0))
			b.direction = Coord(0, -1);
		else if (b.direction == Coord(0, 1))
			b.direction = Coord(1, 0);
		else
			b.direction = Coord(-1, 0);
		break;
	case '/':
		if (b.direction == Coord(1, 0))
			b.direction = Coord(0, -1);
		else if (b.direction == Coord(-1, 0))
			b.direction = Coord(0, 1);
		else if (b.direction == Coord(0, 1))
			b.direction = Coord(-1, 0);
		else
			b.direction = Coord(1, 0);
		break;
	}
	mem.insert(tmp);
	return std::make_pair(true, energized == 1);
}

bool operator==(const Tile& lhs, const Tile& rhs)
{
	return lhs.type == rhs.type;
}