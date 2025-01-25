#include "helpers.hpp"
#include <fstream>

typedef std::pair<std::string, std::string> Wire;

typedef std::map<std::string, std::set<std::string> > Nodes;

Wire flip(const Wire& wire) {
    return std::make_pair(wire.second, wire.first);
}

class Graph
{

public:

    std::string root;
    Nodes nodes;
    std::vector<Wire> wires;
    int size;

    Graph(): size(0) {}
    Graph(const Graph& o): root(o.root), nodes(o.nodes), wires(o.wires), size(o.size) {}
    Graph& operator=(const Graph& o) {
        root = o.root;
        nodes = o.nodes;
        wires = o.wires;
        size = o.size;

        return *this;
    }

    void setRoot(const std::string& str) {root = str;}
    void fixSize() {size = nodes.size();}
    void addWire(const Wire& wire) {
        wires.push_back(wire);
        addNodeRoute(wire);
    }

    void addNodeRoute(const Wire& wire) {
        nodes[wire.first].insert(wire.second);
        nodes[wire.second].insert(wire.first);
    }

    void deleteNodeRoute(const Wire& wire) {
        nodes[wire.first].erase(wire.second);
        nodes[wire.second].erase(wire.first); 
    }

    void browse() const {
        std::set<std::string> visited;
        std::deque<std::string> tasks;
        std::set<std::string> currentNode;
        std::string current;

        tasks.push_back(root);
        visited.insert(root);
        while (!tasks.empty()) {
            current = tasks.front();
            tasks.pop_front();
            currentNode = nodes.find(current)->second;
            for (const std::string& neigh : currentNode) {
                if (visited.insert(neigh).second) {
                    tasks.push_back(neigh);
                }
            }
        }
        std::cout << "result is " << visited.size() * (size - visited.size()) << '\n';
    }

    void parseForGraphViz() {
        std::ofstream out("viz.txt");
        for (Wire w : wires) {
            out << "  " << w.first << " -> " << w.second << ";\n";
        }
    }
};
    
int main() {
    std::ifstream input;
    ui part = 5;
    std::string line;
    std::vector<std::string> splitted;
    Graph g;
    Wire w;

    if (getFileAndPart(25, input, part)) {
        return errno;
    }

    while (!input.eof()) {
        getline(input, line);
        splitted = inputLib::split(line, " ");
        for (int i = 1; i < splitted.size(); ++i) {
            w.first = splitted[0].substr(0, splitted[0].size() - 1);
            w.second = splitted[i];
            g.addWire(w);
        }
        g.fixSize();
        g.setRoot("nnl");
    }

    // g.parseForGraphViz();
    g.deleteNodeRoute(Wire("nnl", "kpc"));
    g.deleteNodeRoute(Wire("hrs", "mnf"));
    g.deleteNodeRoute(Wire("sph", "rkh"));
    g.browse();

    return 0;
}