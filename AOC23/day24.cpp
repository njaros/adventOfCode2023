#include "helpers.hpp"

template <class T>
class HailStone {
public:

    Coord3D<T> pos;
    Coord3D<T> speed;
    double a;
    double b;

    HailStone() {}
    HailStone(Coord3D<T> p, Coord3D<T> s): pos(p), speed(s), a((double)s.y / (double)s.x), b((double)p.y - (a * (double)p.x)) {}
    HailStone(const HailStone& o): pos(o.pos), speed(o.speed), a(o.a), b(o.b) {}
    HailStone& operator=(const HailStone& o) {
        pos = o.pos;
        speed = o.speed;
        a = o.a;
        b = o.b;
        return *this;
    }

    HailStone& move(ll times) {
        pos += (speed * times);
    }

    HailStone moveCpy(ll times) {
        HailStone cpy(&this);

        return cpy.move(times);
    }

    std::optional<T> xyIntercept(const HailStone& o) const {
        double res;
        
        if (speed.x * o.speed.y - speed.y * o.speed.x == 0)
            return std::nullopt;
        res = (double)(o.b - b) / (double)(a - o.a);
        // std::cout << *this << '\n' << o << '\n' << "--> " << res << "\n\n";
        if (
            speed.x < 0 && res > pos.x ||
            speed.x > 0 && res < pos.x ||
            o.speed.x < 0 && res > o.pos.x ||
            o.speed.x > 0 && res < o.pos.x)
            return std::nullopt;
        return res;
    }

    bool unitP1(T min, T max, const HailStone& o) {
        T x;
        T y;
        
        std::optional<T> inter = xyIntercept(o);
        if (!inter.has_value())
            return false;
        x = inter.value();
        if (x > max || x < min)
            return false;
        y = a * x + b;
        return y <= max && y >= min;
    }

    ull countP1(T min, T max, const std::list<HailStone>& lst) {
        ull count = 0;

        for (const HailStone& h : lst)
            count += unitP1(min, max, h);

        return count;
    }

};

template <class T>
std::ostream& operator<<(std::ostream& o, const HailStone<T>& h) {
    o << "pos: " << h.pos << "\nspeed: " << h.speed << "\na: " << h.a << " | b: " << h.b;
    return o;
}

int main() {
    ui part = 0;
    std::ifstream input;
    inputLib::extracted ex;
    std::list<HailStone<ll> > hailStones;
    HailStone<ll> currentHailstone;
    Coord3D<ll> currentPos;
    Coord3D<ll> currentSpeed;

    ull res = 0;

    if (getFileAndPart(24, input, part))
        return errno;

    while (ex.second != EOF) {
        ex = inputLib::extractNextNumber(input);
        currentPos.x = ex.first.value();
        ex = inputLib::extractNextNumber(input);
        currentPos.y = ex.first.value();
        ex = inputLib::extractNextNumber(input);
        currentPos.z = ex.first.value();
        ex = inputLib::extractNextNumber(input);
        currentSpeed.x = ex.first.value();
        ex = inputLib::extractNextNumber(input);
        currentSpeed.y = ex.first.value();
        ex = inputLib::extractNextNumber(input);
        currentSpeed.z = ex.first.value();
        currentHailstone = HailStone<ll>(currentPos, currentSpeed);
        if (part == 1)
            res += currentHailstone.countP1(200000000000000, 400000000000000, hailStones);
        hailStones.push_back(currentHailstone);
    }

    if (part != 1)
    {
        
    }

    std::cout << "result is " << res << '\n';

    return 0;
}