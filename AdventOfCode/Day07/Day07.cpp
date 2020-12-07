#include <cassert>

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int main()
{
	std::unordered_map<std::string, std::unordered_map<std::string, int>> input;

	//std::ifstream infile("input0.txt");
	//std::ifstream infile("input1.txt");
	std::ifstream infile("input.txt");

	std::string line;
	if (infile.is_open())
	{
		while (std::getline(infile, line))
		{
			int space1 = line.find(' ');
			int space2 = line.find(' ', space1 + 1);
			int space3 = line.find(' ', space2 + 1);
			int space4 = line.find(' ', space3 + 1);

			std::string outerColor = line.substr(0, space2);
			//std::string bags = line.substr(space2 + 1, space3 - space2 - 1);
			//std::string contain = line.substr(space3 + 1, space4 - space3 - 1);

			std::string phaseEnd = line.substr(space4 + 1);

			if (phaseEnd == "no other bags.")
			{
				input[outerColor] = {};
			}
			else
			{
				int comma = space4 - 1;
				while (comma >= 0)
				{
					int contentStart = comma + 2;
					int space5 = line.find(' ', contentStart);
					int space6 = line.find(' ', space5 + 1);
					int space7 = line.find(' ', space6 + 1);

					std::string number = line.substr(contentStart, space5 - contentStart);
					std::string innerColor = line.substr(space5 + 1, space7 - space5 - 1);

					input[outerColor][innerColor] = std::stoi(number);

					comma = line.find(',', space7);
				}
			}
		}
	}

	std::function<bool(std::unordered_map<std::string, int> const&, std::string const&, std::unordered_set<std::string_view>&)> canContain;

	canContain = [&canContain, &input](std::unordered_map<std::string, int> const& rule, std::string const& colorSearch, std::unordered_set<std::string_view>& usedColors )
	{
		if (rule.find(colorSearch) != rule.end())
		{
			return true;
		}
		else
		{
			for (auto const& it : rule)
			{
				std::string const& innerColor = it.first;
				if (usedColors.find(innerColor) == usedColors.end())
				{
					usedColors.insert(innerColor);
					if (canContain(input[innerColor], colorSearch, usedColors))
					{
						return true;
					}
				}
			}
			return false;
		}
	};


	int result1 = 0;

	for (auto const& outer : input)
	{
		std::string const& outerColor = outer.first;
		if (outerColor != "shiny gold")
		{
			std::unordered_set<std::string_view> usedColors;
			if (canContain(outer.second, "shiny gold", usedColors))
				++result1;
		}
	}

	std::function<int(std::unordered_map<std::string, int> const&)> containCount;

	containCount = [&containCount, &input](std::unordered_map<std::string, int> const& rule)
	{
		int cont = 0;
		for (auto const& it : rule)
		{
			int inner = containCount(input[it.first]);
			cont += (inner + 1) * it.second;
		}
		return cont;
	};

	int result2 = containCount(input["shiny gold"]);


    std::cout << "Bags that can eventually contain a shiny gold bag " << result1 << std::endl;
    std::cout << "Bags that a shiny gold bag has to contain: " << result2 << std::endl;
}
