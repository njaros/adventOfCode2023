#include "helpers.hpp"

class Brick;

typedef interval::Interval Inter;
typedef std::unordered_map<int, Brick> Bricks;
typedef std::unordered_map<int, ull> Cache;

Cache cache;

class Brick {
public:
    int id;
    Inter x;
    Inter y;
    Inter z;
    std::list<int> underBricks;
    std::list<int> upperBricks;
    Bricks* all;

    Brick() {}
    Brick(int id_, Inter x_, Inter y_, Inter z_, Bricks* b): id(id_), x(x_), y(y_), z(z_), all(b) {}
    Brick(const Brick& o):
    id(o.id), x(o.x), y(o.y), z(o.z),
    underBricks(o.underBricks), upperBricks(o.upperBricks), all(o.all) {}
    Brick& operator=(const Brick& o) {
        id = o.id;
        x = o.x;
        y = o.y;
        z = o.z;
        underBricks = o.underBricks;
        upperBricks = o.upperBricks;
        all = o.all;
        return *this;
    }

    bool tryMove() {
        ll zDist;
        ll neerest = -z.l.first;
        if (z.l.first != 1) {
            for (Bricks::iterator it = all->begin(); it != all->end(); ++it) {
                if (it->first != id) {
                    if (isTouchableWith(it->second)) {
                        zDist = it->second.z.r.first - z.l.first;
                        if (zDist < 0) {
                            if (zDist > neerest)
                                neerest = zDist;
                        }
                    }
                }
            }
            if (neerest < -1) {
                z.l.first += (neerest + 1);
                z.r.first += (neerest + 1);
                return true;
            }
        }
        return false;
    }

    bool isTouchableWith(Brick& o) {
        return x.overlapsWith(o.x) && y.overlapsWith(o.y);
    }

    void setUppers() {
        for (Bricks::iterator it = all->begin(); it != all->end(); ++it) {
            if (it->first != id) {
                if (it->second.z.l.first - 1 == z.r.first && isTouchableWith(it->second))
                    upperBricks.push_back(it->first);
            }
        }
    }

    void setUnders() {
        for (Bricks::iterator it = all->begin(); it != all->end(); ++it) {
            if (it->first != id) {
                if (z.l.first - 1 == it->second.z.r.first && isTouchableWith(it->second))
                    underBricks.push_back(it->first);
            }
        }
    }

    bool isDisintegratable() {
        if (upperBricks.size() == 0)
            return true;
        for (int idx : upperBricks) {
            if (all->at(idx).underBricks.size() < 2)
                return false;
        }
        return true;
    }

    ull countFalls() {
        ull res = 0;
        if (cache.find(id) != cache.end())
            return cache[id];
        for (int idx : upperBricks) {
            if (all->at(idx).underBricks.size() == 1) {
                res += 1 + all->at(idx).countFalls();
            }
        }
        cache[id] = res;
        return res;
    }

};

std::ostream& operator<<(std::ostream& o, const Brick& b) {
    o << "id: " << b.id << " x: " << b.x << " y: " << b.y << " z: " << b.z << " all: " << (size_t)b.all << '\n';
    o << "unders: " << b.underBricks << '\n';
    o << "uppers: " << b.upperBricks << '\n';

    return o;
}

void displayBricks(const Bricks& b) {
    for (Bricks::const_iterator cit = b.begin(); cit != b.end(); ++cit) {
        std::cout << cit->second;
    }
}

int main() {
    ui part = 0;
    std::ifstream input;
    inputLib::extracted ext;
    Bricks bricks;
    Brick current;
    int id = 0;
    Inter x;
    Inter y;
    Inter z;
    ull res = 0;

    if (getFileAndPart(22, input, part))
        return errno;
    
    while (!input.eof()) {
        ext = inputLib::extractNextNumber(input);
        x.l = interval::Elt(ext.first.value(), true);
        ext = inputLib::extractNextNumber(input);
        y.l = interval::Elt(ext.first.value(), true);
        ext = inputLib::extractNextNumber(input);
        z.l = interval::Elt(ext.first.value(), true);
        ext = inputLib::extractNextNumber(input);
        x.r = interval::Elt(ext.first.value(), true);
        ext = inputLib::extractNextNumber(input);
        y.r = interval::Elt(ext.first.value(), true);
        ext = inputLib::extractNextNumber(input);
        z.r = interval::Elt(ext.first.value(), true);
        bricks[id] = Brick(id, x, y, z, &bricks);
        ++id;
    }
    
    bool moveSucceed = true;

    while (moveSucceed) {
        moveSucceed = false;
        for (Bricks::iterator it = bricks.begin(); it != bricks.end(); ++it) {
            if (it->second.tryMove())
                moveSucceed = true;
        }
    }

    std::cout << "moves done.\n";

    if (part == 1) {
        for (Bricks::iterator it = bricks.begin(); it != bricks.end(); ++it) {
            it->second.setUppers();
            it->second.setUnders();
        }
        std::cout << "uppers/unders done.\n";
        for (Bricks::iterator it = bricks.begin(); it != bricks.end(); ++it) {
                res += it->second.isDisintegratable();
        }
    }
    else {
        std::set<int> movedId;
        int loop = 0;
        Bricks copy;
        for (Bricks::iterator it = bricks.begin(); it != bricks.end(); ++it) {
            copy = bricks;
            movedId.clear();
            copy.erase(it->first);
            for (Bricks::iterator it2 = copy.begin(); it2 != copy.end(); ++it2) {
                it2->second.all = &copy;
            }
            moveSucceed = true;
            while (moveSucceed) {
                moveSucceed = false;
                for (Bricks::iterator it2 = copy.begin(); it2 != copy.end(); ++it2) {
                    if (it2->second.tryMove()) {
                        moveSucceed = true;
                        movedId.insert(it2->first);
                    }
                }
            }
            res += movedId.size();
            std::cout << ++loop << '\n';
        } 
    }
    std::cout << "13274 too low\nresult is " << res << '\n';

    return 0;
}