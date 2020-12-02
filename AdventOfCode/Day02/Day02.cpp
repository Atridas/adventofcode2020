#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main()
{
	struct Dataline
	{
		int min, max;
		char letter;
		std::string password;
	};

	std::vector<Dataline> input;
	{
		std::ifstream infile("input.txt");

		std::string line;
		if (infile.is_open())
		{
			while (std::getline(infile, line))
			{
				Dataline dataline;

				int dash = line.find('-');
				int space = line.find(' ');
				int colon = line.find(':');

				dataline.min = std::stoi(line.substr(0, dash));
				dataline.max = std::stoi(line.substr(dash + 1, space - dash - 1));
				dataline.letter = line[space + 1];

				dataline.password = line.substr(colon + 2);

				input.push_back(dataline);
			}
			infile.close();
		}
	}

	int correctPasswordsPolicy1 = 0;
	int incorrectPasswordsPolicy1 = 0;
	int correctPasswordsPolicy2 = 0;
	int incorrectPasswordsPolicy2 = 0;

	for (const Dataline& dataline : input)
	{
		// part 1
		{
			int count = 0;
			for (char c : dataline.password)
			{
				if (c == dataline.letter)
				{
					++count;
				}
			}

			if (count >= dataline.min && count <= dataline.max)
			{
				++correctPasswordsPolicy1;
			}
			else
			{
				++incorrectPasswordsPolicy1;
			}
		}

		// part 2
		{
			char position1Value = dataline.min <= dataline.password.length() ? dataline.password[dataline.min - 1] : '\0';
			char position2Value = dataline.max <= dataline.password.length() ? dataline.password[dataline.max - 1] : '\0';

			int count = 0;
			if (position1Value == dataline.letter)
			{
				++count;
			}
			if (position2Value == dataline.letter)
			{
				++count;
			}

			if (count == 1)
			{
				++correctPasswordsPolicy2;
			}
			else
			{
				++incorrectPasswordsPolicy2;
			}
		}
	}

	std::cout << "Correct passwords policy 1: " << correctPasswordsPolicy1 << std::endl;
	std::cout << "Incorrect passwords policy 1: " << incorrectPasswordsPolicy1 << std::endl;

	std::cout << "Correct passwords policy 2: " << correctPasswordsPolicy2 << std::endl;
	std::cout << "Incorrect passwords policy 2: " << incorrectPasswordsPolicy2 << std::endl;
}
