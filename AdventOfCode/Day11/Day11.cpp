#include <cassert>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main()
{
	enum class Seat {
		Empty, Occupied, Floor
	};
	std::vector<std::vector<Seat>> input;

	{
		std::ifstream infile("input.txt");

		std::string line;
		if (infile.is_open())
		{
			while (std::getline(infile, line))
			{
				std::vector<Seat> row;
				for (char c : line)
				{
					switch (c)
					{
					case 'L':
						row.push_back(Seat::Empty);
						break;
					case '.':
						row.push_back(Seat::Floor);
						break;
					default:
						assert(false);
						break;
					}
				}
				input.push_back(std::move(row));
			}
		}
	}

	bool changed1;
	int occupied1;
	{
		std::vector<std::vector<Seat>> currentState = input;
		std::vector<std::vector<Seat>> nextState = input;
		do
		{
			changed1 = false;
			occupied1 = 0;


			const int columnLen = currentState.size();
				const int rowLen = currentState[0].size();
			for (int i = 0; i < columnLen; ++i)
			{
				assert(currentState[i].size() == rowLen);
				for (int j = 0; j < rowLen; ++j)
				{
					int neigboutSeats = 0;

					if (i > 0 && j > 0 && currentState[i - 1][j - 1] == Seat::Occupied)
						++neigboutSeats;
					if (i > 0 && currentState[i - 1][j] == Seat::Occupied)
						++neigboutSeats;
					if (i > 0 && j < rowLen - 1 && currentState[i - 1][j + 1] == Seat::Occupied)
						++neigboutSeats;

					if (j > 0 && currentState[i][j - 1] == Seat::Occupied)
						++neigboutSeats;
					if (j < rowLen - 1 && currentState[i][j + 1] == Seat::Occupied)
						++neigboutSeats;

					if (i < columnLen - 1 && j > 0 && currentState[i + 1][j - 1] == Seat::Occupied)
						++neigboutSeats;
					if (i < columnLen - 1 && currentState[i + 1][j] == Seat::Occupied)
						++neigboutSeats;
					if (i < columnLen - 1 && j < rowLen - 1 && currentState[i + 1][j + 1] == Seat::Occupied)
						++neigboutSeats;

					switch (currentState[i][j])
					{
					case Seat::Empty:
						if (neigboutSeats == 0)
						{
							nextState[i][j] = Seat::Occupied;
							changed1 = true;
							++occupied1;
						}
						else
							nextState[i][j] = Seat::Empty;
						break;
					case Seat::Occupied:
						if (neigboutSeats >= 4)
						{
							nextState[i][j] = Seat::Empty;
							changed1 = true;
						}
						else
						{
							nextState[i][j] = Seat::Occupied;
							++occupied1;
						}
						break;
					case Seat::Floor:
						nextState[i][j] = Seat::Floor;
						break;
					default:
						assert(false);
						break;
					}
				}
			}


			std::swap(nextState, currentState);

		} while (changed1);
	}

	bool changed2;
	int occupied2;
	{
		std::vector<std::vector<Seat>> currentState = input;
		std::vector<std::vector<Seat>> nextState = input;
		do
		{
			changed2 = false;
			occupied2 = 0;


			const int columnLen = currentState.size();
			const int rowLen = currentState[0].size();
			for (int i = 0; i < columnLen; ++i)
			{
				assert(currentState[i].size() == rowLen);
				for (int j = 0; j < rowLen; ++j)
				{
					int neigboutSeats = 0;

					// - -
					for (int d = 1; i - d >= 0 && j - d >= 0; ++d)
					{
						if (currentState[i - d][j - d] == Seat::Occupied)
						{
							++neigboutSeats;
							break;
						}
						else if (currentState[i - d][j - d] == Seat::Empty)
						{
							break;
						}
					}

					// - 0
					for (int d = 1; i - d >= 0; ++d)
					{
						if (currentState[i - d][j] == Seat::Occupied)
						{
							++neigboutSeats;
							break;
						}
						else if (currentState[i - d][j] == Seat::Empty)
						{
							break;
						}
					}

					// - +
					for (int d = 1; i - d >= 0 && j + d < rowLen; ++d)
					{
						if (currentState[i - d][j + d] == Seat::Occupied)
						{
							++neigboutSeats;
							break;
						}
						else if (currentState[i - d][j + d] == Seat::Empty)
						{
							break;
						}
					}

					// 0 -
					for (int d = 1; j - d >= 0; ++d)
					{
						if (currentState[i][j - d] == Seat::Occupied)
						{
							++neigboutSeats;
							break;
						}
						else if (currentState[i][j - d] == Seat::Empty)
						{
							break;
						}
					}

					// 0 +
					for (int d = 1; j + d < rowLen; ++d)
					{
						if (currentState[i][j + d] == Seat::Occupied)
						{
							++neigboutSeats;
							break;
						}
						else if (currentState[i][j + d] == Seat::Empty)
						{
							break;
						}
					}

					// + -
					for (int d = 1; i + d < columnLen && j - d >= 0; ++d)
					{
						if (currentState[i + d][j - d] == Seat::Occupied)
						{
							++neigboutSeats;
							break;
						}
						else if (currentState[i + d][j - d] == Seat::Empty)
						{
							break;
						}
					}

					// + 0
					for (int d = 1; i + d < columnLen; ++d)
					{
						if (currentState[i + d][j] == Seat::Occupied)
						{
							++neigboutSeats;
							break;
						}
						else if (currentState[i + d][j] == Seat::Empty)
						{
							break;
						}
					}

					// + +
					for (int d = 1; i + d < columnLen && j + d < rowLen; ++d)
					{
						if (currentState[i + d][j + d] == Seat::Occupied)
						{
							++neigboutSeats;
							break;
						}
						else if (currentState[i + d][j + d] == Seat::Empty)
						{
							break;
						}
					}

					switch (currentState[i][j])
					{
					case Seat::Empty:
						if (neigboutSeats == 0)
						{
							nextState[i][j] = Seat::Occupied;
							changed2 = true;
							++occupied2;
						}
						else
							nextState[i][j] = Seat::Empty;
						break;
					case Seat::Occupied:
						if (neigboutSeats >= 5)
						{
							nextState[i][j] = Seat::Empty;
							changed2 = true;
						}
						else
						{
							nextState[i][j] = Seat::Occupied;
							++occupied2;
						}
						break;
					case Seat::Floor:
						nextState[i][j] = Seat::Floor;
						break;
					default:
						assert(false);
						break;
					}
				}
			}


			std::swap(nextState, currentState);

		} while (changed2);
	}
	std::cout << "Occupied seats rules 1: " << occupied1 << std::endl;
	std::cout << "Occupied seats rules 2: " << occupied2 << std::endl;
}
