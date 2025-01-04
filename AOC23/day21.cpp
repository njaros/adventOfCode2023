#include "helpers.hpp"

#define FULLEVEN 0
#define FULLODD 1
#define QUARTERUPRIGHT 2
#define QUARTERDOWNRIGHT 3
#define QUARTERDOWNLEFT 4
#define QUARTERUPLEFT 5
#define THREEQUARTERUPRIGHT 6
#define THREEQUARTERDOWNRIGHT 7
#define THREEQUARTERDOWNLEFT 8
#define THREEQUARTERUPLEFT 9
#define RIGHT 10
#define DOWN 11
#define LEFT 12
#define UP 13

const Coord dirs[] = {Coord(1, 0), Coord(0, 1), Coord(-1, 0), Coord(0, -1)};

std::vector<ull> getAllMapKindCount(const Grid<char>& g) {
    std::vector<ull> res;
    ull count;
    std::set<Coord> endsOdd;
    std::set<Coord> endsEven;
    std::set<Coord> visited;
    std::deque<Coord> currentTasks;
    std::deque<Coord> nextTasks;
    Coord current;
    int i = 0;

    nextTasks.push_back(g.findOne('S').value());

    while (!nextTasks.empty()) {
        currentTasks = nextTasks;
        nextTasks.clear();
        while (!currentTasks.empty()) {
            current = currentTasks.front();
            currentTasks.pop_front();
            visited.insert(current);
            if (i % 2 == 1)
                endsOdd.insert(current);
            else
                endsEven.insert(current);
            for (Coord dir : dirs) {
                if
                (
                    g.inBounds(current + dir) &&
                    g.get(current + dir) != '#' &&
                    visited.find(current + dir) == visited.end()
                )
                {
                    nextTasks.push_back(current + dir);
                    visited.insert(current + dir);
                }
            }
        }
        ++i;
    }
    res.push_back(endsEven.size());
    res.push_back(endsOdd.size());
    res.insert(res.end(), 12, 0);
    int origin = (g.size() / 2);
    for (Coord c : endsEven) {
        if (c.first - c.second >= origin)
            ++res[QUARTERUPRIGHT];
        if (c.first + c.second >= 3 * origin)
            ++res[QUARTERDOWNRIGHT];
        if (c.second - c.first >= origin)
            ++res[QUARTERDOWNLEFT];
        if (c.second + c.first <= origin)
            ++res[QUARTERUPLEFT];
    }
    for (Coord c : endsOdd) {
        if (c.first - c.second <= origin) {
            ++res[THREEQUARTERUPRIGHT];
            if (c.first + c.second <= 3 * origin)
                ++res[RIGHT];    
        }
        if (c.first + c.second <= 3 * origin) {
            ++res[THREEQUARTERDOWNRIGHT];
            if (c.second - c.first <= origin)
                ++res[DOWN];
        }
        if (c.second - c.first <= origin) {
            ++res[THREEQUARTERDOWNLEFT];
            if (c.second + c.first >= origin)
                ++res[LEFT];
        }
        if (c.second + c.first >= origin) {
            ++res[THREEQUARTERUPLEFT];
            if (c.first - c.second <= origin)
                ++res[UP];
        }
    }
    return res;
}

ull part2(Grid<char>& g, ull max) {
    ull res = 0;
    std::vector<ull> allMapKindCount = getAllMapKindCount(g);
    ull stepsFromBorder = max - 65;
    ull mapFullyCoveredLine = (stepsFromBorder / 131) - 1;
    res += ullPow((mapFullyCoveredLine + 1), 2) * allMapKindCount[FULLEVEN];
    res += ullPow(mapFullyCoveredLine, 2) * allMapKindCount[FULLODD];
    res += allMapKindCount[UP] + allMapKindCount[DOWN] + allMapKindCount[RIGHT] + allMapKindCount[LEFT];
    res += mapFullyCoveredLine * (
        allMapKindCount[QUARTERDOWNLEFT] +
        allMapKindCount[QUARTERDOWNRIGHT] +
        allMapKindCount[QUARTERUPLEFT] +
        allMapKindCount[QUARTERUPRIGHT] +
        allMapKindCount[THREEQUARTERDOWNLEFT] +
        allMapKindCount[THREEQUARTERDOWNRIGHT] +
        allMapKindCount[THREEQUARTERUPLEFT] +
        allMapKindCount[THREEQUARTERUPRIGHT]);
    res +=  allMapKindCount[QUARTERDOWNLEFT] +
            allMapKindCount[QUARTERDOWNRIGHT] +
            allMapKindCount[QUARTERUPLEFT] +
            allMapKindCount[QUARTERUPRIGHT];
    return res;
}

int main() {
    ui part = 0;
    std::ifstream input;
    std::set<Coord> ends;
    std::set<Coord> visited;
    std::deque<Coord> currentTasks;
    std::deque<Coord> nextTasks;
    ull res = 0;
    Coord current;

    if (getFileAndPart(21, input, part))
        return errno;

    Grid<char> g(input);
    currentTasks.push_back(g.findOne('S').value());

    if (part == 1) {
        for (int i = 0; i <= 65; ++i) {
            while (!currentTasks.empty()) {
                current = currentTasks.front();
                currentTasks.pop_front();
                visited.insert(current);
                if (i % 2 == 1) {
                    ends.insert(current);
                    g.get(current) = 'O';
                }
                for (Coord dir : dirs) {
                    if
                    (
                        g.inBounds(current + dir) &&
                        g.get(current + dir) != '#' &&
                        visited.find(current + dir) == visited.end()
                    )
                    {
                        nextTasks.push_back(current + dir);
                        visited.insert(current + dir);
                    }
                }
            }
            currentTasks = nextTasks;
            nextTasks.clear();
        }
        std::cout << g << '\n';
        res = ends.size();
    }

    else {
        res = part2(g, 26501365);
    }

    std::cout << "result is " << res << '\n';

    return 0;
}