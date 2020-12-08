#include <cassert>

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

enum class Instruction
{
	nop, acc, jmp
};

std::tuple<int, bool> execute(std::vector<std::tuple<Instruction, int>> const &input)
{
	int programCounter = 0;
	int accumulator = 0;
	std::unordered_set<int> instructionsExecuted;
	for (; programCounter < (int)input.size();)
	{
		if (instructionsExecuted.find(programCounter) == instructionsExecuted.end())
		{
			instructionsExecuted.insert(programCounter);

			auto const& [ins, arg] = input[programCounter];

			switch (ins)
			{
			case Instruction::nop:
				++programCounter;
				break;
			case Instruction::acc:
				accumulator += arg;
				++programCounter;
				break;
			case Instruction::jmp:
				programCounter += arg;
				break;
			default:
				assert(false);
				break;
			}
		}
		else
		{
			break;
		}
	}

	return std::make_tuple(accumulator, programCounter == (int)input.size());
}

int main()
{

	std::vector<std::tuple<Instruction, int>> input;

	//std::ifstream infile("input0.txt");
	//std::ifstream infile("input1.txt");
	std::ifstream infile("input.txt");

	std::string line;
	if (infile.is_open())
	{
		while (std::getline(infile, line))
		{
			int space = line.find(' ');

			std::string instruction = line.substr(0, space);
			std::string arg = line.substr(space + 1);

			int iarg = std::stoi(arg);

			Instruction i = Instruction::nop;

			if (instruction == "nop")
			{
				i = Instruction::nop;
			}
			else if (instruction == "acc")
			{
				i = Instruction::acc;
			}
			else if (instruction == "jmp")
			{
				i = Instruction::jmp;
			}
			else
			{
				assert(false);
			}

			input.emplace_back(i, iarg);
		}
	}

	{
		auto const& [accumulator, correctTermination] = execute(input);

		std::cout << "Accumulator: " << accumulator << std::endl;
		std::cout << "Correct Termination: " << correctTermination << std::endl;
	}

	for (int i = 0; i < (int)input.size(); ++i)
	{
		Instruction ins = std::get<0>(input[i]);
		if (ins != Instruction::acc)
		{
			// fck it
			std::vector<std::tuple<Instruction, int>> inputCopy = input;
			
			if (ins == Instruction::nop)
			{
				std::get<0>(inputCopy[i]) = Instruction::jmp;
			}
			else 
			{
				assert(ins == Instruction::jmp);
				std::get<0>(inputCopy[i]) = Instruction::nop;
			}

			auto const& [accumulator, correctTermination] = execute(inputCopy);

			if (correctTermination)
			{
				std::cout << "Accumulator: " << accumulator << std::endl;
				std::cout << "Correct Termination: " << correctTermination << std::endl;
				std::cout << "Instruction fixed: " << i << std::endl;
			}
		}
	}
}
