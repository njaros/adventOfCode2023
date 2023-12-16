#pragma once

#include "helpers.hpp"
#include "Beam.hpp"

class Beam;

class Tile
{
public:

	char type;
	ui energized;
	std::set< Coord > mem;

	Tile();
	Tile(const Tile& o);
	Tile(char t);
	Tile& operator=(const Tile& other);
	~Tile();

	std::pair<bool, bool> out(Beam& b);

};

bool operator==(const Tile& lhs, const Tile& rhs);
