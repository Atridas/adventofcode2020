#include <cassert>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main()
{
	std::ifstream infile("input.txt");

	std::string line;
	if (infile.is_open())
	{
		while (std::getline(infile, line))
		{
		}
	}

    std::cout << "Hello World!" << std::endl;
}
