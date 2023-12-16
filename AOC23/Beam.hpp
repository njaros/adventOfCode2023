#pragma once
#include "helpers.hpp"
#include "Tile.hpp"

class Tile;

typedef Grid< Tile > Machine;

class Beam
{

	bool _NextMoveInBounds(const Machine& m);

public:
	Coord direction;
	Coord position;
	std::deque<Beam>* allBeams;

	Beam();
	Beam(std::deque<Beam>* pouet);
	Beam(const Beam& other);
	Beam(const Coord& dir, const Coord& pos, std::deque<Beam>* all);
	~Beam();
	Beam& operator=(const Beam& o);

	std::pair<bool, bool> proceed(Machine& m);

};