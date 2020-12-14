#include <cassert>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

int main()
{
	std::ifstream infile("input.txt");

	unsigned long long maskA = 0;
	unsigned long long maskB = 0;
	std::vector<int> floating;
	std::unordered_map<int, unsigned long long> memory1;
	std::unordered_map<unsigned long long, unsigned long long> memory2;

	std::string line;
	if (infile.is_open())
	{
		while (std::getline(infile, line))
		{
			if (line.find("mask = ") == 0)
			{
				maskA = 0;
				maskB = 0;
				floating.clear();
				for (int i = 7; i < (int)line.size(); ++i)
				{
					switch (line[i])
					{
					case '0':
						maskA = maskA << 1;
						maskB = maskB << 1;
						break;
					case '1':
						maskA = (maskA << 1) | 1;
						maskB = (maskB << 1) | 1;
						break;
					case 'X':
						maskA = (maskA << 1);
						maskB = (maskB << 1) | 1;
						floating.push_back(35 - (i - 7));
						break;
					default:
						break;
					}
				}
			}
			else
			{
				assert(line.find("mem[") == 0);
				int interm = line.find("] = ");
				int index = std::stoi(line.substr(4, interm - 3));
				unsigned long long value = std::stoi(line.substr(interm + 4));


				memory1[index] = (value | maskA) & maskB;


				std::vector<unsigned long long> indexes = { index | maskA };
				for (int floatingBit : floating)
				{
					unsigned long long mask1 = 1ULL << floatingBit;
					unsigned long long mask0 = ~mask1;

					std::vector<unsigned long long> nextIndexes;
					nextIndexes.reserve(indexes.size() * 2);

					for (unsigned long long i : indexes)
					{
						nextIndexes.push_back(i & mask0);
						nextIndexes.push_back(i | mask1);
					}
					indexes = std::move(nextIndexes);
				}

				for (unsigned long long i : indexes)
				{
					memory2[i] = value;
				}

			}
		}
	}

	unsigned long long counter1 = 0;
	unsigned long long counter2 = 0;

	for (auto const & i : memory1)
	{
		counter1 += i.second;
	}

	for (auto const & i : memory2)
	{
		counter2 += i.second;
	}

	std::cout << "Solution 1 " << counter1 << std::endl;
	std::cout << "Solution 2 " << counter2 << std::endl;
}
