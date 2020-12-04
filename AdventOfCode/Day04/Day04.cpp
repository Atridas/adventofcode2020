#include <cassert>

#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

int main()
{
	std::vector<std::unordered_map<std::string, std::string>> input;

	std::ifstream infile("input.txt");

	std::string line;
	if (infile.is_open())
	{
		std::unordered_map<std::string, std::string> passport;
		while (std::getline(infile, line))
		{
			if (line.empty())
			{
				input.push_back(std::move(passport));
				passport.clear();
			}
			else
			{
				while (!line.empty())
				{
					int space = line.find(' ');
					std::string field;
					if (space > 0)
					{
						field = line.substr(0, space);
						line = line.substr(space + 1);
					}
					else
					{
						field = std::move(line);
						line.clear();
					}

					int colon = field.find(':');
					assert(colon > 0);
					std::string key = field.substr(0, colon);
					std::string value = field.substr(colon + 1);
					passport[key] = value;
				}
			}
		}
		input.push_back(std::move(passport));
	}

	int validPassports = 0;

	auto onlyDigits = [](std::string_view str)
	{
		for (char c : str)
		{
			if (c > '9' || c < '0')
				return false;
		}
		return true;
	};

	for (std::unordered_map<std::string, std::string> const& passport : input)
	{
		auto byr = passport.find("byr");
		auto iyr = passport.find("iyr");
		auto eyr = passport.find("eyr");
		auto hgt = passport.find("hgt");
		auto hcl = passport.find("hcl");
		auto ecl = passport.find("ecl");
		auto pid = passport.find("pid");
		auto cid = passport.find("cid");

		
		if (byr != passport.end())
		{
			if (byr->second.size() != 4 || !onlyDigits(byr->second))
			{
				continue; // valid = false;
			}
			int year = std::stoi(byr->second);
			if (year < 1920 || year > 2002)
			{
				continue; // valid = false;
			}
		}
		else
		{
			continue; // valid = false;
		}
		
		if (iyr != passport.end())
		{
			if (iyr->second.size() != 4 || !onlyDigits(iyr->second))
			{
				continue; // valid = false;
			}
			int year = std::stoi(iyr->second);
			if (year < 2010 || year > 2020)
			{
				continue; // valid = false;
			}
		}
		else
		{
			continue; // valid = false;
		}
		
		if (eyr != passport.end())
		{
			if (eyr->second.size() != 4 || !onlyDigits(eyr->second))
			{
				continue; // valid = false;
			}
			int year = std::stoi(eyr->second);
			if (year < 2020 || year > 2030)
			{
				continue; // valid = false;
			}
		}
		else
		{
			continue; // valid = false;
		}
		
		if (hgt != passport.end())
		{
			std::string height = hgt->second.substr(0, hgt->second.size() - 2);
			std::string units = hgt->second.substr(hgt->second.size() - 2, 2);

			if (!onlyDigits(height) || height.empty())
			{
				continue; // valid = false;
			}

			int h = std::stoi(height);

			if (units == "cm")
			{
				if (h < 150 || h > 193)
				{
					continue; // valid = false;
				}
			}
			else if (units == "in")
			{
				if (h < 59 || h > 76)
				{
					continue; // valid = false;
				}
			}
			else
			{
				continue; // valid = false;
			}
		}
		else
		{
			continue; // valid = false;
		}
		
		if (hcl != passport.end())
		{
			std::string const & hairColor = hcl->second;

			if (hairColor.size() != 7 || hairColor[0] != '#')
			{
				continue; // valid = false;
			}

			bool valid = true;
			for (int i = 1; i < 7; ++i)
			{
				if ((hairColor[i] >= '0' && hairColor[i] <= '9') || (hairColor[i] >= 'a' && hairColor[i] <= 'f'))
				{
					// --
				}
				else
				{
					valid = false;
					break;
				}
			}

			if (!valid)
			{
				continue; // valid = false;
			}
		}
		else
		{
			continue; // valid = false;
		}
		
		if (ecl != passport.end())
		{
			std::string const & eyeColor = ecl->second;

			if (eyeColor == "amb" || eyeColor == "blu" || eyeColor == "brn" || eyeColor == "gry" || eyeColor == "grn" || eyeColor == "hzl" || eyeColor == "oth")
			{
				// --
			}
			else
			{
				continue; // valid = false;
			}
		}
		else
		{
			continue; // valid = false;
		}

		if (pid != passport.end())
		{
			if (pid->second.size() != 9 || !onlyDigits(pid->second))
			{
				continue; // valid = false;
			}
		}
		else
		{
			continue; // valid = false;
		}


		//if (byr && iyr && eyr && hgt && hcl && ecl && pid /*&& cid*/)
		{
			++validPassports;
		}
	}

	std::cout << "valid passports:" << validPassports << std::endl;
}
