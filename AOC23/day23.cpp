#include "helpers.hpp"

const Coord dirs[] = {Coord(1, 0), Coord(0, 1), Coord(-1, 0), Coord(0, -1)};

bool possibleMove(const Grid<char>& g, Coord c, Coord d, ui part) {
	if (part == 1) {
		switch (g.get(c + d))
		{
		case '.':
			return true;
		case '<':
			return (d == Coord(-1, 0));
		case '>':
			return (d == Coord(1, 0));
		case '^':
			return (d == Coord(0, -1));
		case 'v':
			return (d == Coord(0, 1));
		default:
			return false;
		}
	}
	else
		return g.get(c + d) != '#' && !(c == Coord(135, 127) && d != Coord(0, -1));
}

void findWay(const Grid<char>& g, Coord current, Coord goal, ui steps, ui& longest, std::set<Coord> visited, ui part) {
	visited.insert(current);
	std::list<Coord> possibleWays;
	if (current == goal) {
		if (longest < steps) {
			longest = steps;
			std::cout << longest << '\n';
		}
		return;
	}
	for (Coord dir : dirs) {
		if (g.inBounds(current + dir) && possibleMove(g, current, dir, part) && visited.find(current + dir) == visited.end())
			possibleWays.push_back(current + dir);
	}
	while (possibleWays.size() == 1) {
		// std::cout << possibleWays << " | " << steps << '\n';
		current = possibleWays.front();
		possibleWays.clear();
		visited.insert(current);
		++steps;
		if (current == goal) {
			if (longest < steps) {
				longest = steps;
				std::cout << longest << '\n';
			}
			return;
		}
		for (Coord dir : dirs) {
			if (g.inBounds(current + dir) && possibleMove(g, current, dir, part) && visited.find(current + dir) == visited.end())
				possibleWays.push_back(current + dir);	
		}
	}
	if (possibleWays.size() > 1) {
		for (Coord c : possibleWays) {
			findWay(g, c, goal, steps + 1, longest, visited, part);
		}
	}
}

int main() {
	ui part = 0;
	std::ifstream input;
	ui res = 0;

	if (getFileAndPart(23, input, part))
		return errno;

	Grid<char> g(input);
	Coord start(1, 0);
	Coord goal(g.back().size() - 2, g.size() - 1);
	findWay(g, start, goal, 0, res, {}, part);	

	std::cout << "6306 too low\nresult is " << res << '\n';

	return 0;
}