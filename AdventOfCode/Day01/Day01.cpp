// Day01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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
			infile.close();
		}
	}

	for (int i = 0; i < input.size(); ++i)
	{
		for (int j = i + 1; j < input.size(); ++j)
		{
			if (input[i] + input[j] == 2020)
			{
				int result = input[i] * input[j];
				std::cout << input[i] << " + " << input[j] << " = 2020" << std::endl;
				std::cout << input[i] << " * " << input[j] << " = " << result << std::endl;
			}

			for (int k = j + 1; k < input.size(); ++k)
			{
				if (input[i] + input[j] + input[k] == 2020)
				{
					int result = input[i] * input[j] * input[k];
					std::cout << input[i] << " + " << input[j] << " + " << input[k] << " = 2020" << std::endl;
					std::cout << input[i] << " * " << input[j] << " * " << input[k] << " = " << result << std::endl;
				}
			}
		}
	}

}
