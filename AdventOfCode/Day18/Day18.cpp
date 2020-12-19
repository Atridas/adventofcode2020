#include "AoC18.h"

#include <cassert>

#include <charconv>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Parser : public aoc18::Parser
{
public:

	unsigned long long operate(aoc18::Scanner &scanner)
	{
		advancedMode = false;
		operations.resize(1);
		accum.resize(1);

		operations[0] = Operation::Add;
		accum[0] = 0;

		ParseOperation(scanner);

		return accum[0];
	}

	unsigned long long operateAdvanced(aoc18::Scanner &scanner)
	{
		advancedMode = true;
		operations.resize(1);
		accum.resize(1);

		operations[0] = Operation::Add;
		accum[0] = 0;

		ParseAdvancedOperation(scanner);

		return accum[0];
	}

protected:
	//virtual bool BeginOperation() { return true; }
	//virtual bool EndOperation() { return true; }

	virtual bool BeginParenthesis()
	{
		std::cout << '(';

		operations.push_back(Operation::Add);
		accum.push_back(0);

		return true;
	}
	virtual bool EndParenthesis()
	{
		std::cout << ')';

		unsigned long long n = accum[accum.size() - 1];

		operations.pop_back();
		accum.pop_back();

		switch (operations.back())
		{
		case Operation::Add:
			accum[accum.size() - 1] += n;
			break;
		case Operation::Mul:
			accum[accum.size() - 1] *= n;
			break;
		default:
			break;
		}

		return true;
	}

	virtual bool ParseTerminal(aoc18::TerminalData const& terminalData)
	{
		if (advancedMode)
		{
			if (terminalData.type == aoc18::Terminal::NUMBER)
			{
				unsigned long long n = 0;
				for (int i = 0; i < terminalData.textSize; ++i)
				{
					assert(terminalData.textBeginning[i] >= '0' && terminalData.textBeginning[i] <= '9');
					n *= 10;
					n += terminalData.textBeginning[i] - '0';
				}
				
				switch (operations.back())
				{
				case Operation::Add:
					accum[accum.size() - 1] += n;
					break;
				case Operation::Mul:
					accum[accum.size() - 1] *= n;
					break;
				default:
					break;
				}
			}
		}
		else
		{
			switch (terminalData.type)
			{
			case aoc18::Terminal::NUMBER:
			{
				unsigned long long n = 0;
				for (int i = 0; i < terminalData.textSize; ++i)
				{
					assert(terminalData.textBeginning[i] >= '0' && terminalData.textBeginning[i] <= '9');
					n *= 10;
					n += terminalData.textBeginning[i] - '0';
				}

				std::cout << n;

				switch (operations.back())
				{
				case Operation::Add:
					accum[accum.size() - 1] += n;
					break;
				case Operation::Mul:
					accum[accum.size() - 1] *= n;
					break;
				default:
					break;
				}

				break;
			}
			case aoc18::Terminal::PLUS:
				std::cout << '+';
				operations[operations.size() - 1] = Operation::Add;
				break;
			case aoc18::Terminal::MULT:
				std::cout << 'x';
				operations[operations.size() - 1] = Operation::Mul;
				break;
			default:
				break;
			}

		}
		return true;
	}


	virtual bool BeginAdvancedAddition()
	{
		operations.push_back(Operation::Add);
		accum.push_back(0);

		return true;
	}
	virtual bool EndAdvancedAddition() {
		unsigned long long n = accum[accum.size() - 1];

		operations.pop_back();
		accum.pop_back();

		switch (operations.back())
		{
		case Operation::Add:
			accum[accum.size() - 1] += n;
			break;
		case Operation::Mul:
			accum[accum.size() - 1] *= n;
			break;
		default:
			break;
		}
		return true;
	}

	virtual bool BeginAdvancedMultiplication()
	{
		operations.push_back(Operation::Mul);
		accum.push_back(1);
		return true;
	}
	virtual bool EndAdvancedMultiplication()
	{
		unsigned long long n = accum[accum.size() - 1];

		operations.pop_back();
		accum.pop_back();

		switch (operations.back())
		{
		case Operation::Add:
			accum[accum.size() - 1] += n;
			break;
		case Operation::Mul:
			accum[accum.size() - 1] *= n;
			break;
		default:
			break;
		}
		return true;
	}


	virtual void ErrorUnexpectedTerminal(aoc18::TerminalData const& terminalData, aoc18::Terminal expectedTerminal)
	{
		std::cout << "ERROR!!" << std::endl;
	}

	virtual void ErrorUnexpectedTerminal(aoc18::TerminalData const& terminalData, aoc18::Terminal const* expectedTerminals, size_t expectedTerminalCount)
	{
		std::cout << "ERROR!!" << std::endl;
	}

private:
	enum class Operation {
		Add, Mul
	};

	bool advancedMode = false;

	std::vector<Operation> operations;
	std::vector<unsigned long long> accum;
};

std::u32string ToUTF32(const std::string s32)
{
	std::u32string result;
	for (char32_t c : s32)
	{
		if (c < 0x80)
		{
			result += (char)c;
		}
		else
			assert(false); // TODO
	}
	return result;
}

int main()
{
	Parser p;

	std::ifstream infile("input.txt");

	unsigned long long accum1 = 0;
	unsigned long long accum2 = 0;

	std::string line;
	if (infile.is_open())
	{
		while (std::getline(infile, line))
		{
			auto line32 = ToUTF32(line);
			{
				aoc18::Scanner scanner(line32.c_str());
				unsigned long long result1 = p.operate(scanner);
				accum1 += result1;
				std::cout << " = " << result1 << std::endl;
			}
			{
				aoc18::Scanner scanner(line32.c_str());
				unsigned long long result2 = p.operateAdvanced(scanner);
				accum2 += result2;
				std::cout << " = " << result2 << std::endl;
			}
		}
	}

	std::cout << "result 1: " << accum1 << std::endl;
	std::cout << "result 2: " << accum2 << std::endl;
}
