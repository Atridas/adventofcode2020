#include <cassert>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

int main()
{
	std::vector<std::vector<std::string>> input;

	std::ifstream infile("input.txt");

	std::string line;
	if (infile.is_open())
	{
		std::vector<std::string> group;
		while (std::getline(infile, line))
		{
			if (line.empty())
			{
				input.push_back(std::move(group));
				group.clear();
			}
			else
			{
				group.push_back(line);
			}
		}
		input.push_back(std::move(group));
	}

	int accum = 0;
	int accum2 = 0;

	for (std::vector<std::string> const& group : input)
	{
		std::unordered_set<char> answers;
		for (std::string const& person : group)
		{
			for (char const c : person)
			{
				answers.insert(c);
			}
		}
		accum += answers.size();


		for (std::string const& person : group)
		{
			std::unordered_set<char> localAnswers;
			for (char const c : person)
			{
				localAnswers.insert(c);
			}

			std::unordered_set<char> toRemove;
			for (char const c : answers)
			{
				if (localAnswers.find(c) == localAnswers.end())
				{
					toRemove.insert(c);
				}
			}
			for (char const c : toRemove)
			{
				answers.erase(c);
			}
		}

		accum2 += answers.size();
	}

	std::cout << "result 1: " << accum << std::endl;
	std::cout << "result 2: " << accum2 << std::endl;
}