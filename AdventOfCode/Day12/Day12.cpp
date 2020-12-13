#include <cassert>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <tuple>

int main()
{
	enum class Instruction
	{
		North, South, East, West, Front, Left, Right
	};

	std::vector<std::tuple< Instruction, int>> input;

	std::ifstream infile("input.txt");

	std::string line;
	if (infile.is_open())
	{
		while (std::getline(infile, line))
		{
			Instruction i = Instruction::North;
			int units = 0;

			switch (line[0])
			{
			case 'N':
				i = Instruction::North;
				break;
			case 'S':
				i = Instruction::South;
				break;
			case 'E':
				i = Instruction::East;
				break;
			case 'W':
				i = Instruction::West;
				break;

			case 'F':
				i = Instruction::Front;
				break;
			case 'L':
				i = Instruction::Left;
				break;
			case 'R':
				i = Instruction::Right;
				break;

			default:
				assert(false);
				break;
			}

			units = std::stoi(line.substr(1));

			if (i == Instruction::Left || i == Instruction::Right)
				assert(units % 90 == 0);

			input.emplace_back(i, units);
		}
	}

	{
		Instruction direction = Instruction::East;
		int x = 0, y = 0;

		auto moveAbs = [&x, &y](Instruction direction, int units)
		{
			switch (direction)
			{
			case Instruction::North:
				y += units;
				break;
			case Instruction::South:
				y -= units;
				break;
			case Instruction::East:
				x += units;
				break;
			case Instruction::West:
				x -= units;
				break;
			default:
				assert(false);
				break;
			}
		};

		auto turnLeft = [&direction]()
		{
			switch (direction)
			{
			case Instruction::North:
				direction = Instruction::West;
				break;
			case Instruction::South:
				direction = Instruction::East;
				break;
			case Instruction::East:
				direction = Instruction::North;
				break;
			case Instruction::West:
				direction = Instruction::South;
				break;
			default:
				assert(false);
				break;
			}
		};

		auto turnRight = [&direction]()
		{
			switch (direction)
			{
			case Instruction::North:
				direction = Instruction::East;
				break;
			case Instruction::South:
				direction = Instruction::West;
				break;
			case Instruction::East:
				direction = Instruction::South;
				break;
			case Instruction::West:
				direction = Instruction::North;
				break;
			default:
				assert(false);
				break;
			}
		};

		auto turnReverse = [&direction]()
		{
			switch (direction)
			{
			case Instruction::North:
				direction = Instruction::South;
				break;
			case Instruction::South:
				direction = Instruction::North;
				break;
			case Instruction::East:
				direction = Instruction::West;
				break;
			case Instruction::West:
				direction = Instruction::East;
				break;
			default:
				assert(false);
				break;
			}
		};

		for (auto step : input)
		{
			Instruction instruction = std::get<0>(step);
			int units = std::get<1>(step);

			switch (instruction)
			{
			case Instruction::North:
			case Instruction::South:
			case Instruction::East:
			case Instruction::West:
				moveAbs(instruction, units);
				break;
			case Instruction::Front:
				moveAbs(direction, units);
				break;
			case Instruction::Left:
				switch (units % 360)
				{
				case 0:
					break;
				case 90:
					turnLeft();
					break;
				case 180:
					turnReverse();
					break;
				case 270:
					turnRight();
					break;
				default:
					break;
				}
				break;
			case Instruction::Right:
				switch (units % 360)
				{
				case 0:
					break;
				case 90:
					turnRight();
					break;
				case 180:
					turnReverse();
					break;
				case 270:
					turnLeft();
					break;
				default:
					break;
				}
				break;
			default:
				assert(false);
				break;
			}
		}

		std::cout << "Final position: " << x << ", " << y << std::endl;
		std::cout << "Manhatan distance: " << std::abs(x) + std::abs(y) << std::endl;
	}

	// problem 2
	{
		int x = 0, y = 0;
		int wx = 10, wy = 1;



		auto turnLeft = [&wx, &wy]()
		{
			std::swap(wx, wy);
			wx = -wx;
		};

		auto turnRight = [&wx, &wy]()
		{
			std::swap(wx, wy);
			wy = -wy;
		};

		auto turnReverse = [&wx, &wy]()
		{
			wx = -wx;
			wy = -wy;
		};

		for (auto step : input)
		{
			Instruction instruction = std::get<0>(step);
			int units = std::get<1>(step);

			switch (instruction)
			{
			case Instruction::North:
				wy += units;
				break;
			case Instruction::South:
				wy -= units;
				break;
			case Instruction::East:
				wx += units;
				break;
			case Instruction::West:
				wx -= units;
				break;
			case Instruction::Front:
				x += wx * units;
				y += wy * units;
				break;
			case Instruction::Left:
				switch (units % 360)
				{
				case 0:
					break;
				case 90:
					turnLeft();
					break;
				case 180:
					turnReverse();
					break;
				case 270:
					turnRight();
					break;
				default:
					break;
				}
				break;
			case Instruction::Right:
				switch (units % 360)
				{
				case 0:
					break;
				case 90:
					turnRight();
					break;
				case 180:
					turnReverse();
					break;
				case 270:
					turnLeft();
					break;
				default:
					break;
				}
				break;
			default:
				assert(false);
				break;
			}
		}

		std::cout << "Final position 2: " << x << ", " << y << std::endl;
		std::cout << "Manhatan distance 2: " << std::abs(x) + std::abs(y) << std::endl;
	}
}
