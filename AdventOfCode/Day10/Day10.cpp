#include <cassert>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main()
{
	std::vector<int> input;

	{
		std::ifstream infile("input.txt");

		std::string line;
		if (infile.is_open())
		{
			while (std::getline(infile, line))
			{
				input.push_back(std::stoi(line));
			}
		}
	}

	std::sort(input.begin(), input.end());
	
	{
		int last = 0;
		int count1 = 0;
		int count3 = 1;

		for (int joltage : input)
		{
			if (joltage - last == 1)
			{
				++count1;
			}
			else if (joltage - last == 3)
			{
				++count3;
			}
			else
			{
				assert(joltage - last == 2);
			}
			last = joltage;
		}

		std::cout << "1-diffs:" << count1 << std::endl;
		std::cout << "3-diffs:" << count3 << std::endl;
		std::cout << "solution 1:" << count1 * count3 << std::endl;
	}


	{
		// 0
		// 1 combination

		// 0 1
		// 1 combination

		// 0 1 3
		// 0 3
		// 2 combinations

		// 0 1 3 4
		// 0 1 4
		// 0 3 4
		// 3 combinations

		// 0 1 3 4 7
		// 3 combinations

		int last_1 = 0;
		unsigned long long combinationsLast_1 = 1;
		int last_2 = 0;
		unsigned long long combinationsLast_2 = 0;
		int last_3 = 0;
		unsigned long long combinationsLast_3 = 0;

		for (int joltage : input)
		{
			assert(joltage - last_1 <= 3);
			unsigned long long combinations = combinationsLast_1;

			if(joltage - last_2 <= 3)
			{
				combinations += combinationsLast_2;
			}
			
			if(joltage - last_3 <= 3)
			{
				combinations += combinationsLast_3;
			}

			assert(combinations >= combinationsLast_1);
			assert(combinationsLast_1 >= combinationsLast_2);
			assert(combinationsLast_2 >= combinationsLast_3);

			last_3 = last_2;
			combinationsLast_3 = combinationsLast_2;
			last_2 = last_1;
			combinationsLast_2 = combinationsLast_1;
			last_1 = joltage;
			combinationsLast_1 = combinations;

		}


		std::cout << "solution 2:" << combinationsLast_1 << std::endl;
	}
}
