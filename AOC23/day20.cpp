#include "helpers.hpp"

enum NetType
{
	none,
	broadcaster,
	flipFlop,
	conjunction
};

struct Instruction {
	std::string from;
	std::string to;
	bool pulse;

	Instruction() {}
	Instruction(const std::string& f, const std::string& t, bool p):
	from(f), to(t), pulse(p) {}
};

typedef std::deque< Instruction > InstructionQueue;

class NetNode
{
public:
	NetType type;
	std::string name;
	std::vector<std::string> outputs;
	bool pulse;
	std::unordered_map<std::string, bool> mem;

	NetNode(): type(none), pulse(false) {}
	NetNode(const NetNode& o): type(o.type), name(o.name), outputs(o.outputs), pulse(o.pulse), mem(o.mem) {}
	NetNode(NetType e_type, const std::string& e_name, const std::vector<std::string>& e_outputs):
	type(e_type), name(e_name), outputs(e_outputs), pulse(false) {}

	void readInstruction(Instruction input, InstructionQueue& queue, ull& lowCount, ull& highCount)
	{
		char toType;
		bool ignored = false;
		switch (type)
		{
			case conjunction:
				toType = '&';
				break;
			case flipFlop:
				toType = '%';
				break;
		}
		// std::cout << input.from << " -" << input.pulse << "-> " << toType << input.to;
		switch (type)
		{
			case broadcaster:
				for (const std::string& out : outputs)
					queue.push_back(Instruction(name, out, input.pulse));
				if (input.pulse)
					highCount += outputs.size();
				else
					lowCount += outputs.size();
				break;
			case flipFlop:
				if (!input.pulse)
				{
					pulse = !pulse;
					for (const std::string& out : outputs)
						queue.push_back(Instruction(name, out, pulse));
					if (pulse)
						highCount += outputs.size();
					else
						lowCount += outputs.size();
				}
				else
					ignored = true;
				break;
			case conjunction:
				mem[input.from] = input.pulse;
				pulse = false;
				for (const std::pair<std::string, bool>& elt : mem)
				{
					if (!elt.second)
					{
						pulse = true;
						break;
					}
				}
				for (const std::string& out : outputs)
					queue.push_back(Instruction(name, out, pulse));
				if (pulse)
					highCount += outputs.size();
				else
					lowCount += outputs.size();
				break;
			default:
				ignored = true;
		}
		// if (ignored)
			// std::cout << " (ignored)\n";
		// else
			// std::cout << '\n';
	}
};

typedef std::unordered_map<std::string, NetNode> NetMap;

void initConjunctions(NetMap& netMap) {
	for (NetMap::iterator it = netMap.begin(); it != netMap.end(); ++it) {
		if (it->second.type == conjunction) {
			for (NetMap::iterator it2 = netMap.begin(); it2 != netMap.end(); ++it2) {
				if (it != it2) {
					for (const std::string& out : it2->second.outputs) {
						if (out == it->first)
							it->second.mem[it2->first] = false;
					}
				}
			}
		}
	}
}

int main()
{
	std::ifstream input;
	ui part = 0;
	std::string line;
	std::string name;
	std::string::size_type idx;
	NetType netType;
	NetMap netMap;
	Instruction current;
	Instruction button("button", {"broadcaster"}, false);
	InstructionQueue queue;
	ull lowCount = 0;
	ull highCount = 0;
	ull res = 0;

	if (getFileAndPart(20, input, part))
		return errno;

	while (!input.eof())
	{
		getline(input, line);
		idx = line.find(' ');
		switch (line[0])
		{
			case '%':
				netType = flipFlop;
				name = line.substr(1, idx - 1);
				break;
			case '&':
				netType = conjunction;
				name = line.substr(1, idx - 1);
				break;
			case 'b':
				netType = broadcaster;
				name = line.substr(0, idx);
				break;
			default:
				throw(std::invalid_argument("bad input."));
		}
		idx = line.find(' ', idx + 1) + 1;
		netMap[name] = NetNode(netType, name, inputLib::split(line.substr(idx), ", "));
	}

	initConjunctions(netMap);

	if (part == 1) {
		for (int i = 0; i < 1000; ++i) {
			lowCount += 1;
			queue.push_back(button);
			while (!queue.empty())
			{
				current = queue.front();
				queue.pop_front();
				netMap[current.to].readInstruction(current, queue, lowCount, highCount);
			}
		}
		res = lowCount * highCount;
	}

	else {
		std::vector<ull> loops;
		while (loops.size() < 4) {
			++res;
			queue.push_back(button);
			while (!queue.empty())
			{
				current = queue.front();
				if (current.to == "bn" && current.pulse)
					loops.push_back(res);
				queue.pop_front();
				netMap[current.to].readInstruction(current, queue, lowCount, highCount);
			}
		}
		res = math::PPCM(loops);
	}

	std::cout << "result is " << res << '\n';

	return 0;
}
