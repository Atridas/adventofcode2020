#include <cassert>

#include <charconv>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <unordered_set>


struct Rule
{
	std::string name;

	int min1, max1;
	int min2, max2;
};


struct Ticket
{
	std::vector<int> values;
};

Ticket parseTicket(std::string_view line)
{
	Ticket result;
	int lastComma = 0;
	int comma;
	for (;;)
	{
		comma = line.find(',', lastComma);

		std::string_view value = comma > 0 ? line.substr(lastComma, comma - lastComma) : line.substr(lastComma);

		int i;
		std::from_chars(value.data(), value.data() + value.size(), i);

		result.values.push_back(i);

		if (comma > 0)
			lastComma = comma + 1;
		else
			break;
	}

	return result;
}

void updateInvalidMappings(std::unordered_map<int, std::unordered_set<std::string_view>> &invalidMappings, std::vector<Rule> const &rules, Ticket const &ticket)
{
	for (Rule const& r : rules)
	{
		for (int i = 0; i < (int)ticket.values.size(); ++i)
		{
			int value = ticket.values[i];
			if (!(value >= r.min1 && value <= r.max1) && !(value >= r.min2 && value <= r.max2))
			{
				invalidMappings[i].insert(r.name);
			}
		}
	}
}

int main()
{

	std::vector<Rule> rules;
	Ticket myTicket;
	std::vector<Ticket> nearbyTickets;

	{
		std::ifstream infile("input.txt");

		std::string line;
		if (infile.is_open())
		{
			while (std::getline(infile, line) && line != "")
			{
				int colon = line.find(':');
				Rule rule;
				rule.name = line.substr(0, colon);

				int dash1 = line.find('-', colon + 1);
				int separator = line.find(" or ", dash1 + 1);
				int dash2 = line.find('-', separator + 4);

				rule.min1 = std::stoi(line.substr(colon + 2, dash1 - colon - 2));
				rule.max1 = std::stoi(line.substr(dash1 + 1, separator - dash1 - 1));

				rule.min2 = std::stoi(line.substr(separator + 4, dash2 - separator - 4));
				rule.max2 = std::stoi(line.substr(dash2 + 1));

				rules.push_back(std::move(rule));
			}

			std::getline(infile, line);
			assert(line == "your ticket:");

			std::getline(infile, line);
			myTicket = parseTicket(line);

			std::getline(infile, line);
			assert(line == "");
			std::getline(infile, line);
			assert(line == "nearby tickets:");

			while (std::getline(infile, line))
			{
				nearbyTickets.push_back(parseTicket(line));
			}
		}
	}

	int ticketScanningErrorRate = 0;

	std::unordered_map<int, std::unordered_set<std::string_view>> invalidMappings;

	for (int i = 0; i < (int)myTicket.values.size(); ++i)
	{
		invalidMappings[i]; // force the creation of all
	}

	updateInvalidMappings(invalidMappings, rules, myTicket);

	for (Ticket const& t : nearbyTickets)
	{
		bool isValidTicked = true;
		for (int value : t.values)
		{
			bool isValidForAnyField = false;
			for (Rule const& r : rules)
			{
				if ((value >= r.min1 && value <= r.max1) || (value >= r.min2 && value <= r.max2))
				{
					isValidForAnyField = true;
					break;
				}
			}

			if (!isValidForAnyField)
			{
				ticketScanningErrorRate += value;
				isValidTicked = false;
			}
		}

		if(isValidTicked)
		{
			updateInvalidMappings(invalidMappings, rules, t);
		}
	}

	std::cout << "ticketScanningErrorRate: " << ticketScanningErrorRate << std::endl;
	
	std::cout << std::endl;
	std::cout << " -------------------- " << std::endl;
	std::cout << std::endl;


	bool modified;
	do
	{
		modified = false;
		for (Rule const& r : rules)
		{
			std::optional<int> ruleIndex;
			for (auto& it : invalidMappings)
			{
				if (it.second.find(r.name) == it.second.end())
				{
					if (!ruleIndex.has_value())
						ruleIndex = it.first;
					else
					{
						ruleIndex.reset();
						break;
					}
				}
			}

			if (ruleIndex.has_value())
			{
				// mark all other rules invalid for this index
				for (Rule const& other : rules)
				{
					if (other.name != r.name && invalidMappings[*ruleIndex].find(other.name) == invalidMappings[*ruleIndex].end())
					{
						invalidMappings[*ruleIndex].insert(other.name);
						modified = true; // we need to start again to
					}
				}
			}
		}
	} while (modified);

	unsigned long long solution2 = 1;

	for (Rule const& r : rules)
	{
		std::cout << "Rule: " << r.name << std::endl;
		for (auto& it : invalidMappings)
		{
			if (it.second.find(r.name) == it.second.end())
			{
				std::cout << "\tvalid index: " << it.first << std::endl;

				if (r.name.find("departure") == 0)
				{
					std::cout << "\t\tmy ticket value: " << myTicket.values[it.first] << std::endl;
					solution2 *= myTicket.values[it.first];
				}

			}
		}
	}

	std::cout << "solution 2: " << solution2 << std::endl;
}
