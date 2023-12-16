#include "Beam.hpp"

bool Beam::_NextMoveInBounds(const Machine& m)
{
	Coord Next = direction + position;
	if (Next.second < 0 || Next.second >= m.size())
		return false;
	if (Next.first < 0 || Next.first >= m[Next.second].size())
		return false;
	return true;
}

Beam::Beam() : direction(Coord(1, 0)), position(Coord(-1, 0)), allBeams(0) {}
Beam::Beam(std::deque<Beam>* pouet) : direction(Coord(1, 0)), position(Coord(-1, 0)), allBeams(pouet) {}
Beam::Beam(const Beam& other) : direction(other.direction), position(other.position), allBeams(other.allBeams) {}
Beam::Beam(const Coord& dir, const Coord& pos, std::deque<Beam>* all) : direction(dir), position(pos), allBeams(all) {}
Beam::~Beam() {}
Beam& Beam::operator=(const Beam& o)
{
	direction = o.direction;
	position = o.position;
	allBeams = o.allBeams;
	return *this;
}


std::pair<bool, bool> Beam::proceed(Machine& m)
{
	if (!_NextMoveInBounds(m))
		return std::make_pair(false, false);
	position += direction;
	return m(position).out(*this);
}