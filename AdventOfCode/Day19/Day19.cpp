#include <cassert>

#include <charconv>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_set>
#include <variant>
#include <vector>

struct RuleChar
{
	char c;
};

struct RuleAnd
{
	std::vector<int> subrules;
};

struct RuleOr
{
	std::vector<RuleAnd> subrules;
	std::vector<std::unordered_set<std::string>> firstCharacters;
};


RuleAnd parseRuleAnd(std::string_view in)
{
	RuleAnd result;

	int prevSpace = 0;
	int space = in.find(' ');
	while (space > 0)
	{
		int i;
		std::from_chars(in.data() + prevSpace, in.data() + space, i);
		result.subrules.push_back(i);
		prevSpace = space + 1;
		space = in.find(' ', prevSpace);
	}

	int i;
	std::from_chars(in.data() + prevSpace, in.data() + in.size(), i);
	result.subrules.push_back(i);

	return result;
}

RuleOr parseRuleOr(std::string_view in)
{
	RuleOr result;

	int prevBar = 0;
	int bar = in.find('|');
	while (bar > 0)
	{
		result.subrules.push_back(parseRuleAnd(in.substr(prevBar, bar - prevBar - 1)));

		prevBar = bar + 2;
		bar = in.find('|', prevBar);
	}

	result.subrules.push_back(parseRuleAnd(in.substr(prevBar)));

	return result;
}

std::unordered_set<std::string> printAll(std::vector<std::variant<std::monostate, RuleChar, RuleAnd, RuleOr>> const& rules, std::variant<std::monostate, RuleChar, RuleAnd, RuleOr> const& rule, int depth)
{
	if (depth <= 0)
	{
		return {};
	}

	if (std::holds_alternative<RuleChar>(rule))
	{
		std::string result;
		result = std::get<RuleChar>(rule).c;
		return { std::move(result) };
	}
	else if (std::holds_alternative<RuleAnd>(rule))
	{
		std::unordered_set<std::string> result;

		std::vector<int> subrules = std::get<RuleAnd>(rule).subrules;

		result = printAll(rules, rules[subrules[0]], depth);

		for (int i = 1; i < (int)subrules.size(); ++i)
		{
			int minLen = depth;
			for (std::string const& s : result)
			{
				minLen = std::min(minLen, (int)s.size());
			}

			std::unordered_set<std::string> step = printAll(rules, rules[subrules[i]], depth - minLen);

			if (step.size() > 0)
			{
				std::unordered_set<std::string> nextResult;
				for (std::string const& res : result)
				{
					for (std::string const& s : step)
					{
						nextResult.insert(res + s);
					}
				}
				result = std::move(nextResult);
			}
		}
		return result;
	}
	else if (std::holds_alternative<RuleOr>(rule))
	{
		std::unordered_set<std::string> result;

		for (RuleAnd const& subrule : std::get<RuleOr>(rule).subrules)
		{
			result.merge(printAll(rules, subrule, depth));
		}

		return result;
	}

	return {};
}

struct RuleResult
{
	bool correct = false;
	std::string_view residu;
	std::string_view match;
};

RuleResult canApplyRule(std::vector<std::variant<std::monostate, RuleChar, RuleAnd, RuleOr>> const& rules, std::variant<std::monostate, RuleChar, RuleAnd, RuleOr> const& rule, std::string_view message)
{
	RuleResult result;
	if (std::holds_alternative<RuleChar>(rule))
	{
		char c = std::get<RuleChar>(rule).c;
		result.correct = message.size() > 0 && (c == message[0]);
		if (result.correct)
		{
			result.residu = message.substr(1);
			result.match = message.substr(0, 1);
		}
	}
	else if (std::holds_alternative<RuleAnd>(rule))
	{
		result.residu = message;
		for (int r : std::get<RuleAnd>(rule).subrules)
		{
			result = canApplyRule(rules, rules[r], result.residu);
			if (!result.correct)
				break;
		}
		if (result.correct)
		{
			result.match = message.substr(0, message.size() - result.residu.size());
		}
	}
	else if (std::holds_alternative<RuleOr>(rule))
	{
		RuleOr const& r = std::get<RuleOr>(rule);
		assert(r.subrules.size() == r.firstCharacters.size());
		for (int i = 0; i < (int)r.subrules.size(); ++i)
		{
			for (std::string const& s : r.firstCharacters[i])
			{
				if (message.find(s) == 0)
				{
					return canApplyRule(rules, r.subrules[i], message);
				}
			}
		}
	}
	return result;
}

int main()
{
	std::vector<std::variant<std::monostate, RuleChar, RuleAnd, RuleOr>> rules;
	std::vector<std::string> messages;

	std::ifstream infile("input.txt");

	std::string line;
	if (infile.is_open())
	{
		while (std::getline(infile, line))
		{
			if (line == "")
				break;

			int colon = line.find(':');
			int ruleNumber = std::stoi(line.substr(0, colon));


			if ((int)rules.size() <= ruleNumber)
			{
				rules.resize(ruleNumber + 1);
			}

			int cometes = line.find('"');
			int barra = line.find('|');

			if (cometes >= 0)
			{
				char c = line[line.size() - 2];


				rules[ruleNumber] = RuleChar{ c };
			}
			else if (barra >= 0)
			{
				rules[ruleNumber] = parseRuleOr(line.substr(colon + 2));
			}
			else
			{
				rules[ruleNumber] = parseRuleAnd(line.substr(colon + 2));
			}

		}

		while (std::getline(infile, line))
		{
			messages.push_back(std::move(line));
		}
	}

	/*for (auto const& r : rules)
	{
		auto s = printAll(rules, r, 10);

		for (std::string const& str : s)
		{
			std::cout << str << ',';
		}

		std::cout << std::endl;
	}*/

	for (int ri = 0; ri < (int)rules.size(); ++ri)
	{
		auto& r = rules[ri];

		if (std::holds_alternative<RuleOr>(r))
		{
			int llk = 1;
			RuleOr& rule = std::get<RuleOr>(r);

		COMPUTE_FIRST_CHARACTERS:

			rule.firstCharacters.clear();
			for (RuleAnd const& subrule : std::get<RuleOr>(r).subrules)
			{
				rule.firstCharacters.push_back(printAll(rules, subrule, llk));
			}


			/*for (int i = 0; i < (int)rule.firstCharacters.size(); ++i)
			{
				for (auto it = rule.firstCharacters[i].begin(); it != rule.firstCharacters[i].end();)
				{
					if ((int)it->size() != llk)
					{
						it = rule.firstCharacters[i].erase(it);
					}
					else
					{
						++it;
					}
				}
			}*/

			for (int i = 0; i < (int)rule.firstCharacters.size(); ++i)
			{
				for (int j = i + 1; j < (int)rule.firstCharacters.size(); ++j)
				{
					assert(rule.firstCharacters[j].size() > 0);
					for (std::string const& f : rule.firstCharacters[j])
					{
						if (rule.firstCharacters[i].find(f) != rule.firstCharacters[i].end())
						{
							++llk;
							goto COMPUTE_FIRST_CHARACTERS;
						}
					}
				}
			}


			std::cout << ri << ':' << std::endl;
			for (int i = 0; i < (int)rule.firstCharacters.size(); ++i)
			{
				bool first = true;
				for (std::string const& f : rule.firstCharacters[i])
				{
					char const* b = ", ";
					if (first)
					{
						b = "\t";
						first = false;
					}
					std::cout << b << f;
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}
	}

	int count1 = 0;
	int count2 = 0;
	for (std::string const& message : messages)
	{
		std::cout << message << std::endl;

		auto result = canApplyRule(rules, rules[0], message);
		if (result.correct && result.residu.size() == 0)
			++count1;


		{
			RuleResult result42 = canApplyRule(rules, rules[42], message);


			if (!result42.correct)
			{
				continue;
			}
			std::cout << "\t42: " << result42.match << " " << result42.residu << std::endl;

			RuleResult result31;

			int rules42 = 0;
			do
			{
				++rules42;
				result31 = canApplyRule(rules, rules[31], result42.residu);
				result42 = canApplyRule(rules, rules[42], result42.residu);

				if (result42.correct)
				{
					std::cout << "\t42: " << result42.match << " " << result42.residu << std::endl;
					assert(!result31.correct);
				}
				if (result31.correct)
				{
					std::cout << "\t31: " << result31.match << " " << result31.residu << std::endl;
					assert(!result42.correct);
				}
			} while (result42.correct);

			int rules31 = 1;
			while (result31.correct && result31.residu.size() > 0)
			{
				++rules31;
				result31 = canApplyRule(rules, rules[31], result31.residu);
				if (result31.correct)
				{
					std::cout << "\t31: " << result31.match << " " << result31.residu << std::endl;
				}
			}

			if (result31.correct && result31.residu.size() == 0 && rules42 > rules31)
			{
				++count2;
				std::cout << "\tYES (42:" << rules42 << ", 31:" << rules31 << ')' << std::endl;
			}
		}
	}


	std::cout << "puzzle 1: " << count1 << std::endl;
	std::cout << "puzzle 2: " << count2 << std::endl;
}
