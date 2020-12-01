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

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
