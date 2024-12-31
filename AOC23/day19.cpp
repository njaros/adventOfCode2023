#include "helpers.hpp"

#define X 0
#define M 1
#define A 2
#define S 3

typedef std::vector<int> Part;

struct Rule {
    int category;
    int norm;
    bool less;
    std::string dest;

    Rule(): category(0), norm(0), less(true), dest("") {}
    Rule(int c, int n, bool l, const std::string& d): category(c), norm(n), less(l), dest(d) {}
    Rule(const std::string& d): category(0), norm(0), less(true), dest(d) {}
    Rule(const Rule& o): category(o.category), norm(o.norm), less(o.less), dest(o.dest) {}
};

char catToChar(int cat) {
    switch (cat)
    {
    case 0:
        return 'x';
    case 1:
        return 'm';
    case 2:
        return 'a';
    case 3:
        return 's';
    }
    return '?';
}

int catToInt(char c) {
    switch (c)
    {
    case 'x':
        return 0;
    case 'm':
        return 1;
    case 'a':
        return 2;
    case 's':
        return 3;
    }
    return 4;
}

std::ostream& operator<<(std::ostream& o, const Rule& r) {
    char op;

    if (r.less == true)
        op = '<';
    else
        op = '>';
    o << catToChar(r.category) << op << r.norm << ':' << r.dest;

    return o;
}

typedef std::pair<ll, bool> Elt;

bool operator==(const Elt& lhs, const Elt& rhs) {
    return (lhs.first == rhs.first && lhs.second == rhs.second);
}
bool operator<(const Elt& lhs, const Elt& rhs) {
    return (
        lhs.first < rhs.first ||
        (lhs.first == rhs.first && !lhs.second && rhs.second)
    );
}
bool operator>=(const Elt& lhs, const Elt& rhs) {return !(lhs < rhs);}
bool operator>(const Elt& lhs, const Elt& rhs)  {return rhs < lhs;}
bool operator<=(const Elt& lhs, const Elt& rhs) {return rhs >= lhs;}

class Interval {

public:

    Elt l;
    Elt r;

    Interval(): l(0, false), r(0, false) {}
    Interval(const Interval& o): l(o.l), r(o.r) {}
    Interval(Elt a, Elt b): l(a), r(b) {}
    Interval(ll a, ll b): l(Elt(a, true)), r(Elt(b, true)) {}
    Interval& operator=(const Interval& o) {
        l = o.l;
        r = o.r;
        return *this;
    }

    bool overlapsWith(const Interval& o) const {
        if (empty() || o.empty())
            return false;
        if (l.first > o.r.first || r.first < o.l.first)
            return false;
        if (l.first == o.r.first && !l.second && !o.r.second)
            return false;
        if (r.first == o.l.first && !r.second && !o.l.second)
            return false;
        return true;
    }

    bool joinableWith(const Interval& o) const {
        if (empty())
            return true;
        if (o.empty())
            return false;
        if (
            (l.first == o.r.first + 1 && l.second && o.r.second) ||
            (r.first == o.l.first - 1 && r.second && o.l.second))
            return true;
        return overlapsWith(o);
    }

    bool include(const Interval& o) const {
        return (
            ((l.first < o.l.first) || (l.first == o.l.first && !(o.l.second && !l.second))) &&
            ((r.first > o.r.first) || (r.first == o.r.first && !(o.r.second && !r.second)))
        );
    }

    bool isIncludedIn(const Interval& o) const {
        return o.include(*this);
    }

    bool empty() const {
        return l > r || (l.first == r.first && !(l.second && r.second));
    }

    ll length() const {
        return r.first - l.first + r.second + l.second - 1;
    }

    Interval& operator|=(const Interval& o) {
        if (empty() && !o.empty())
            *this = o;
        else if (joinableWith(o)) {
            if (o.l < l)
                l = o.l;
            if (r < o.r)
                r = o.r;
        }
        return *this;
    }

    Interval operator|(const Interval& o) const {
        Interval cpy(*this);

        return cpy |= o;
    }

    Interval& operator&=(const Interval& o) {
        if (o.l > l)
            l = o.l;
        if (o.r < r)
            r = o.r;
        return *this;
    }

    Interval operator&(const Interval& o) const {
        Interval cpy(*this);

        return cpy &= o;
    }

    /**
     * Check where is the number n.
     * returns 0 if n is in the interval.
     * returns 1 if n is superior to the max of the interval.
     * returns -1 if n is inferior to the min of the interval.
     */
    bool where(ll n) const {
        if (
            !empty() && (
                (n < r.first && n > l.first) ||
                (n == r.first && r.second) ||
                (n == l.first && l.second)
            )
        )
            return 0;
        if (n < l.first)
            return -1;
        return 1;
    }

    /**
     * Set the interval to be inferior of n.
     */
    Interval& operator<=(ll n) {
        if (r.first >= n)
            r.first = n - 1;
        return *this;
    }

    Interval operator<(ll n) const {
        Interval cpy(*this);

        return cpy <= n;
    }

    /**
     * Set the interval to be superior of n.
     */
    Interval& operator>=(ll n) {
        if (l.first <= n)
            l.first = n + 1;
        return *this;
    }

    Interval operator>(ll n) const {
        Interval cpy(*this);

        return cpy >= n;
    }

};

std::ostream& operator<<(std::ostream& o, const Interval& i) {
    if (i.l.second)
        o << '[';
    else
        o << ']';
    o << i.l.first << "; " << i.r.first;
    if (i.r.second)
        o << ']';
    else
        o << '[';
    return o;
}

typedef std::vector<Interval> PartGroup;

bool emptyPartGroup(const PartGroup& p) {
    for (const Interval& i : p) {
        if (i.empty())
            return true;
    }
    return false;
}

PartGroup divideParts(PartGroup &g1, int idx, ll n, bool inf) {
    PartGroup g2(g1);

    if (inf) {
        g2[idx] <= n; 
        g1[idx] >= n - 1;
    }
    else {
        g2[idx] >= n;
        g1[idx] <= n + 1;
    }
    return g2;
}

class Workflow: public std::vector<Rule> {

public:

    std::string analyse(const Part& part) const {
        for (int i = 0; i < size() - 1; ++i) {
            if (at(i).less) {
                if (part[at(i).category] < at(i).norm)
                    return at(i).dest;
            }
            else {
                if (part[at(i).category] > at(i).norm)
                    return at(i).dest;
            }
        }
        return back().dest;
    }

    std::vector< std::pair<PartGroup, std::string> > divide(PartGroup& g) const {
        std::vector< std::pair<PartGroup, std::string> > divided;
        for (int i = 0; i < size() - 1; ++i) {
            if (emptyPartGroup(g))
                return divided;
            divided.push_back(
                std::make_pair(
                    divideParts(g, at(i).category, at(i).norm, at(i).less),
                    at(i).dest));
        }
        if (!emptyPartGroup(g))
            divided.push_back(std::make_pair(g, back().dest));
        return divided;
    }

};

typedef std::map<std::string, Workflow> Workflows;

void parseWorkflow(const std::string& line, Workflows& w) {
    std::string::size_type foundComa;
    std::string::size_type foundSemicolon = 0;
    std::string::size_type idx = 0;
    std::string key;

    foundComa = line.find('{');
    key = line.substr(idx, foundComa - idx);
    idx = foundComa + 1;
    foundComa = line.find(',');
    while (foundComa != std::string::npos) {
        foundSemicolon = line.find(':', foundSemicolon + 1);
        w[key].push_back(Rule(
            catToInt(line[idx]),
            atoi(line.substr(idx + 2, foundSemicolon - (idx + 2)).c_str()),
            line[idx + 1] == '<' ? true : false,
            line.substr(foundSemicolon + 1, foundComa - foundSemicolon - 1)
            ));
        idx = foundComa + 1;
        foundComa = line.find(',', idx);
    }
    foundComa = line.find('}');
    w[key].push_back(Rule(line.substr(idx, foundComa - idx)));
}

ull calcLeafScore(const PartGroup& g) {
    ull res = 1;

    for (const Interval& i : g) {
        res *= i.length();
    }
    return res;
}

void browseWorkflows(Workflows& w, std::string current, PartGroup allParts, ull& res)
{
    Workflow divider = w[current];
    std::vector< std::pair< PartGroup, std::string > > divided;

    if (current == "R" || emptyPartGroup(allParts))
        return;
    if (current == "A") {
        res += calcLeafScore(allParts);
        return;
    }
    divided = divider.divide(allParts);
    for (const std::pair< PartGroup, std::string >& job : divided) {
        browseWorkflows(w, job.second, job.first, res);
    }
}

int main() {
    ui part = 0;
    std::ifstream input;
    std::string line;
    inputLib::extracted ex;
    Workflows w;
    std::string current;
    Part read;
    ull res = 0;

    if (getFileAndPart(19, input, part))
        return errno;

    getline(input, line);
    inputLib::carriageReturnDel(line);
    while (!line.empty()) {
        parseWorkflow(line, w);
        getline(input, line);
        inputLib::carriageReturnDel(line);
    }

    // std::cout << w << '\n';
    if (part == 1)
    {
        while (ex.second != EOF) {
            if (ex.second == '\n') {
                current = "in";
                while (current != "A" && current != "R")
                    current = w[current].analyse(read);
                if (current == "A") {
                    for (int n : read)
                        res += n;
                }
                // std::cout << read << '\n';
                read.clear();
            }
            ex = inputLib::extractNextNumberEndline(input);
            if (ex.first.has_value())
                read.push_back(ex.first.value());
        }
        current = "in";
        while (current != "A" && current != "R")
            current = w[current].analyse(read);
        if (current == "A") {
            for (int n : read)
                res += n;
        }
    }

    else {
        PartGroup allParts({
            Interval(1, 4000),
            Interval(1, 4000),
            Interval(1, 4000),
            Interval(1, 4000)
        });
        current = "in";
        browseWorkflows(w, current, allParts, res);
    }
    // std::cout << read << '\n';
    std::cout << "result is " << res << '\n';
}