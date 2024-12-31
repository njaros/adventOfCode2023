#include "helpers.hpp"

enum NetType
{
	none,
	broadcaster,
	flipFlop,
	conjunction
};

typedef std::pair< std::string, std::string > inOut;
typedef std::deque< inOut > instructionQueue;

class NetNode
{
public:
	NetType type;
	std::string name;
	std::vector<std::string> outputs;
	bool pulse;
	std::map<std::string, bool> mem;

	NetNode(): type(none), pulse(false) {}
	NetNode(const NetNode& o): type(o.type), name(o.name), outputs(o.outputs), pulse(o.pulse), mem(o.mem) {}
	NetNode(NetType e_type, const std::string& e_name): type(e_type), name(e_name), pulse(false) {}
	~NetNode() {}

	void addOutput(const std::string& out)
	{
		outputs.push_back(out);
	}

	void readInstruction(const NetNode& input, instructionQueue& queue)
	{
		switch (type)
		{
			case broadcaster:
				for (const std::string& out : outputs)
					queue.push_back(std::make_pair(name, out));
				break;
			case flipFlop:
				if (!input.pulse)
				{
					pulse = !pulse;
					for (const std::string& out : outputs)
						queue.push_back(std::make_pair(name, out));
				}
				break;
			case conjunction:
				mem[input.name] = input.pulse;
				pulse = false;
				for (const std::pair<std::string, bool>& elt : mem)
				{
					if (elt.second)
					{
						pulse = true;
						break;
					}
				}
				for (const std::string& out : outputs)
					queue.push_back(std::make_pair(name, out));
				break;
			default:
				throw(std::domain_error("algo failed."));
		}
	}
};

typedef std::map<std::string, NetNode *> NetMap;

struct Network
{
	NetMap netMap;
	NetNode entryPoint;
};

int day20()
{
	std::ifstream input;
	ui part = 0;
	Network net;
	std::string line;
	std::string::size_type idx;
	NetType netType;
	NetNode* current;

	if (getFileAndPart(100, input, part))
		return errno;

	while (!input.eof())
	{
		getline(input, line);
		switch (line[0])
		{
			case '%':
				netType = flipFlop;
				break;
			case '&':
				netType = conjunction;
				break;
			case 'b':
				netType = broadcaster;
				break;
			default:
				throw(std::invalid_argument("bad input."));
		}
		current = new NetNode(netType, line.substr(1, 2));

	}

	return 0;
}
