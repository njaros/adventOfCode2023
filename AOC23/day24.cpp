#include "helpers.hpp"

template <class T>
class HailStone {
public:

    Coord3D<T> pos;
    Coord3D<T> speed;

    HailStone() {}
    HailStone(Coord3D p, Coord3D s): pos(p), speed(s) {}
    HailStone(const HailStone& o): pos(o.pos), speed(o.speed) {}
    HailStone& operator=(const HailStone& o) {
        pos = o.pos;
        speed = o.speed;
        return *this;
    }

    HailStone& move(ll times) {
        pos += (speed * times);
    }

    HailStone moveCpy(ll times) {
        HailStone cpy(&this);

        return cpy.move(times);
    }

};

int main() {
    ui part = 0;
    std::ifstream input;
    inputLib::extracted ex;
    std::list<Coord3D<ll>> hailStones;

    if (getFileAndPart(100, input, part))
        return errno;

    while (ex.second != EOF) {
        ex = inputLib::extractNextNumber
    }
}