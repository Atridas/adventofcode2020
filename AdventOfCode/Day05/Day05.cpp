#include <cassert>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int main()
{
	std::vector<std::tuple<int, int>> input;

	std::ifstream infile("input.txt");

	std::string line;
	if (infile.is_open())
	{
		while (std::getline(infile, line))
		{
			assert(line.size() == 10);
			int row = 0;
			for (int i = 0; i < 7; ++i)
			{
				row <<= 1;
				if (line[i] == 'B')
				{
					row |= 1;
				}
				else
				{
					assert(line[i] == 'F');
				}
			}
			int column = 0;
			for (int i = 7; i < 10; ++i)
			{
				column <<= 1;
				if (line[i] == 'R')
				{
					column |= 1;
				}
				else
				{
					assert(line[i] == 'L');
				}
			}

			input.emplace_back(row, column);
		}
	}

	int maxSeatId = 0;

	for (std::tuple<int, int> const& seat : input)
	{
		int row = std::get<0>(seat);
		int column = std::get<1>(seat);
		int seatID = row * 8 + column;

		maxSeatId = std::max(maxSeatId, seatID);
	}

	std::sort(input.begin(), input.end(), [](std::tuple<int, int> const& a, std::tuple<int, int> const& b)
		{
			if (std::get<0>(a) < std::get<0>(b))
				return true;
			else if (std::get<0>(a) > std::get<0>(b))
				return false;
			if (std::get<1>(a) < std::get<1>(b))
				return true;
			else
				return false;
		}
	);

	for (int i = 0; i < (int)input.size() - 1; ++i)
	{
		int seatID = std::get<0>(input[i]) * 8 + std::get<1>(input[i]);
		int nextSeatID = std::get<0>(input[i + 1]) * 8 + std::get<1>(input[i + 1]);

		if (nextSeatID != seatID + 1)
		{
			std::cout << "missing seat id: " << seatID + 1 << std::endl;
		}
	}

	std::cout << "max seat id: " << maxSeatId << std::endl;
}
