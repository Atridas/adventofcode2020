#include <cassert>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


int main()
{
	std::vector<unsigned long long> input;

	/*
	const int preamble = 5;
	std::ifstream infile("input0.txt");
	/*/
	const int preamble = 25;
	std::ifstream infile("input.txt");
	//*/

	std::string line;
	if (infile.is_open())
	{
		while (std::getline(infile, line))
		{
			input.push_back(std::stoull(line));
		}
	}

	for (int i = preamble; i < (int)input.size(); ++i)
	{
		for (int j = i - preamble; j < i; ++j)
		{
			for (int k = j + 1; k < i; ++k)
			{
				if (input[j] + input[k] == input[i])
				{
					goto SUM_FOUND;
				}
			}
		}

		std::cout << "Number without sum: " << input[i] << std::endl;

		for (int j = 0; j < (int)input.size(); ++j)
		{
			unsigned long long accum = input[j];
			for (int k = j + 1; k < (int)input.size(); ++k)
			{
				accum += input[k];
				if (accum == input[i])
				{
					unsigned long long min = *std::min_element(input.begin() + j, input.begin() + k);
					unsigned long long max = *std::max_element(input.begin() + j, input.begin() + k);

					std::cout << "Solution: " << min + max << std::endl;
				}
				else if (accum > input[i])
				{
					break;
				}
			}
		}

	SUM_FOUND:
		continue;
	}
}
