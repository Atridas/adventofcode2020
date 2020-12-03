#include <cassert>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main()
{
	std::vector<std::vector<bool>> input;
	{
		std::ifstream infile("input.txt");

		std::string line;
		if (infile.is_open())
		{
			while (std::getline(infile, line))
			{
				std::vector<bool> row;
				for (char c : line)
				{
					if (c == '.')
						row.push_back(false);
					else
					{
						assert(c == '#');
						row.push_back(true);
					}
				}
				input.push_back(row);
			}
			infile.close();
		}
	}

	int treeCountSlope1 = 0;
	int treeCountSlope3 = 0;
	int treeCountSlope5 = 0;
	int treeCountSlope7 = 0;
	int treeCountSlopeJump = 0;

	for (int i = 0; i < (int)input.size(); ++i)
	{
		int xCoord1 = (i * 1) % input[i].size();
		int xCoord3 = (i * 3) % input[i].size();
		int xCoord5 = (i * 5) % input[i].size();
		int xCoord7 = (i * 7) % input[i].size();
		int xCoordJump = (i / 2) % input[i].size();

		bool tree1 = input[i][xCoord1];
		bool tree3 = input[i][xCoord3];
		bool tree5 = input[i][xCoord5];
		bool tree7 = input[i][xCoord7];
		bool treeJump = input[i][xCoordJump];

		if (tree1)
		{
			++treeCountSlope1;
		}
		if (tree3)
		{
			++treeCountSlope3;
		}
		if (tree5)
		{
			++treeCountSlope5;
		}
		if (tree7)
		{
			++treeCountSlope7;
		}
		if (treeJump && i % 2 == 0)
		{
			++treeCountSlopeJump;
		}
	}

    std::cout << "Total Trees Right 1, down 1: " << treeCountSlope1 << std::endl;
    std::cout << "Total Trees Right 3, down 1: " << treeCountSlope3 << std::endl;
    std::cout << "Total Trees Right 5, down 1: " << treeCountSlope5 << std::endl;
    std::cout << "Total Trees Right 7, down 1: " << treeCountSlope7 << std::endl;
    std::cout << "Total Trees Right 1, down 2: " << treeCountSlopeJump << std::endl;

	int result = treeCountSlope1 * treeCountSlope3 * treeCountSlope5 * treeCountSlope7 * treeCountSlopeJump;
    std::cout << "Total Trees Mult: " << result << std::endl;
}
