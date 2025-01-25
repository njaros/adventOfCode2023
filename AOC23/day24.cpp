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

std::pair<long double, long double> solveEquation(
    long double x1, long double x2, long double x3, long double x4, long double x5,
    long double y1, long double y2, long double y3, long double y4, long double y5,
    long double vx1, long double vx2, long double vx3, long double vx4, long double vx5,
    long double vy1, long double vy2, long double vy3, long double vy4, long double vy5
    ) {
    Matrix<long double> solver({
        {vy2 - vy1, vx1 - vx2, y1 - y2, x2 - x1},
        {vy3 - vy1, vx1 - vx3, y1 - y3, x3 - x1},
        {vy4 - vy1, vx1 - vx4, y1 - y4, x4 - x1},
        {vy5 - vy1, vx1 - vx5, y1 - y5, x5 - x1}
    });
    Vector<long double> X({
        x2 * vy2 - x1 * vy1 - y2 * vx2 + y1 * vx1,
        x3 * vy3 - x1 * vy1 - y3 * vx3 + y1 * vx1,
        x4 * vy4 - x1 * vy1 - y4 * vx4 + y1 * vx1,
        x5 * vy5 - x1 * vy1 - y5 * vx5 + y1 * vx1
    });

    Vector<long double> res = solver.inverse() * X;
    return std::make_pair(res[0], res[1]);
}

long double solveEquations(const std::vector<HailStone<ll>>& h) {
    std::pair<long double, long double> res;
    res = solveEquation(
        h[0].pos.x, h[1].pos.x, h[2].pos.x, h[3].pos.x, h[4].pos.x,
        h[0].pos.y, h[1].pos.y, h[2].pos.y, h[3].pos.y, h[4].pos.y,
        h[0].speed.x, h[1].speed.x, h[2].speed.x, h[3].speed.x, h[4].speed.x,
        h[0].speed.y, h[1].speed.y, h[2].speed.y, h[3].speed.y, h[4].speed.y
        );
    long double x = res.first;
    long double y = res.second;
    long double z = solveEquation(
        h[0].pos.x, h[1].pos.x, h[2].pos.x, h[3].pos.x, h[4].pos.x,
        h[0].pos.z, h[1].pos.z, h[2].pos.z, h[3].pos.z, h[4].pos.z,
        h[0].speed.x, h[1].speed.x, h[2].speed.x, h[3].speed.x, h[4].speed.x,
        h[0].speed.z, h[1].speed.z, h[2].speed.z, h[3].speed.z, h[4].speed.z
    ).second;

    return x + y + z;
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
        long double resp2;
        std::vector<HailStone<ll>> batch;
        std::list<HailStone<ll>>::const_iterator cit;

        cit = hailStones.begin();
        for (int i = 0; i < 5; ++i) {
            batch.push_back(*cit);
            cit++;
        }
        resp2 = solveEquations(batch);

        std::cout << "result is " << (ll)resp2 << '\n';
    }
    else
        std::cout << "result is " << res << '\n';

    return 0;
}