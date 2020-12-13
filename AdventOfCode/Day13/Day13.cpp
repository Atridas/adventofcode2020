#include <cassert>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <tuple>

int main()
{
	int earliestDeparture = 0;
	std::vector<std::tuple<int, int>> buses;

	{
		std::ifstream infile("input.txt");

		if (infile.is_open())
		{
			std::string line;
			std::getline(infile, line);
			earliestDeparture = std::stoi(line);

			std::getline(infile, line);

			int offset = 0;
			while (line.size() > 0)
			{
				int comma = line.find(',');
				std::string bus = (comma >= 0) ? line.substr(0, comma) : line;

				if (bus != "x")
				{
					buses.emplace_back(std::stoi(bus), offset);
				}
				++offset;

				line = (comma >= 0) ? line.substr(comma + 1) : "";
			}

			assert(!std::getline(infile, line));
		}
	}


	for (int timestamp = earliestDeparture; ; ++timestamp)
	{
		for (auto element : buses)
		{
			int bus = std::get<0>(element);
			if (timestamp % bus == 0)
			{
				std::cout << "Bus " << bus << " at time " << timestamp << std::endl;
				std::cout << "result: " << bus * (timestamp - earliestDeparture) << std::endl;
				goto BUS_FOUND;
			}
		}
	}

BUS_FOUND:

	long long unsigned N = 1;
	for (auto element : buses)
	{
		int bus = std::get<0>(element);
		N *= bus;
	}

	long long unsigned solution = 0;
	for (auto element : buses)
	{
		int bus = std::get<0>(element);
		long long unsigned n = N / bus;

		int v;
		for (v = 1; (v * n) % bus > 1; v++);

		std::cout << "Bus " << bus << " n " << n << " v " << v << std::endl;
		std::cout << "\tN % bus " << N % bus << std::endl;
		std::cout << "\tn % bus " << n % bus << std::endl;
		std::cout << "\tvn % bus " << (v * n) % bus << std::endl;


		for (auto element : buses)
		{
			int bus2 = std::get<0>(element);
			std::cout << "\t\tbus2 " << bus2 << std::endl;
			std::cout << "\t\tvn % bus2 " << (v * n) % bus2 << std::endl;
		}

		solution += (bus - std::get<1>(element)) * (v * n);
	}

	solution %= N;

	std::cout << "solution 2: " << solution << std::endl;

	for (auto element : buses)
	{
		int bus = std::get<0>(element);
		int offset = std::get<0>(element);
		std::cout << "\tbus " << bus << " offset " << offset << std::endl;
		std::cout << "\t\tsolution % bus " << solution % bus << std::endl;
		std::cout << "\t\t1068781 % bus " << 1068781 % bus << std::endl;
	}

	return 0;
}
