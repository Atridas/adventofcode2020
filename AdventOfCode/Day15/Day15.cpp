#include <cassert>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main()
{
	//std::vector<int> seq = { 0, 3, 6 }; // 436
	//std::vector<int> seq = { 1, 3, 2 }; // 1
	//std::vector<int> seq = { 2, 1, 3 }; // 10
	//std::vector<int> seq = { 1, 2, 3 }; // 27
	//std::vector<int> seq = { 2, 3, 1 }; // 78
	//std::vector<int> seq = { 3, 2, 1 }; // 438
	//std::vector<int> seq = { 3, 1, 2 }; // 1836
	std::vector<int> seq = { 0, 20, 7, 16, 1, 18, 15 };

	std::vector<int> lastOccurrence;

	for (int i = 0; i < (int)seq.size() - 1; ++i)
	{
		while ((int)lastOccurrence.size() <= seq[i])
		{
			lastOccurrence.push_back(-1);
		}

		lastOccurrence[seq[i]] = i + 1;
	}


	while (seq.size() < 30000000)
	{
		int lastNumber = seq.back();

		if (lastNumber < (int)lastOccurrence.size() && lastOccurrence[lastNumber] >= 0)
		{
			int nextNumber = seq.size() - lastOccurrence[lastNumber];
			seq.push_back(nextNumber);
		}
		else
		{
			seq.push_back(0);
		}


		while ((int)lastOccurrence.size() <= lastNumber)
		{
			lastOccurrence.push_back(-1);
		}

		lastOccurrence[lastNumber] = seq.size() - 1;
	}

	std::cout << "Solution 1: " << seq[2019] << std::endl;
	std::cout << "Solution 1: " << seq[30000000 - 1] << std::endl;
}
