#include <cassert>

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

struct Dish
{
	std::unordered_set<std::string> ingredients;
	std::unordered_set<std::string> alergens;
};

int main()
{
	std::vector<Dish> dishes;
	std::unordered_set<std::string> ingredients;
	std::unordered_set<std::string> alergens;

	std::ifstream infile("input.txt");

	std::string line;
	if (infile.is_open())
	{
		while (std::getline(infile, line))
		{
			Dish dish;
			int space = line.find(' ');

			std::string ingredient = line.substr(0, space);

			while (ingredient.find('(') != 0)
			{
				ingredients.insert(ingredient);
				dish.ingredients.insert(std::move(ingredient));
				int nextSpace = line.find(' ', space + 1);
				ingredient = line.substr(space + 1, nextSpace - space - 1);
				space = nextSpace;
			}

			assert(ingredient == "(contains");

			++space;
			int comma = line.find(',', space);

			while (comma >= 0)
			{
				std::string alergen = line.substr(space, comma - space);
				dish.alergens.insert(alergen);
				alergens.insert(std::move(alergen));
				space = comma + 2;
				comma = line.find(',', space);
			}

			std::string alergen = line.substr(space, line.size() - space - 1);
			dish.alergens.insert(alergen);
			alergens.insert(std::move(alergen));

			dishes.push_back(std::move(dish));
		}
	}

	std::unordered_map<std::string, std::unordered_set<std::string>> possiblesIngredientsPerAlergen;
	std::map<std::string, std::string> alergenToIngredient;
	std::unordered_map<std::string, std::string> ingredientToAlergen;


	for (Dish const &dish : dishes)
	{
		for (std::string const& alergen : dish.alergens)
		{
			auto possiblesIngredients = possiblesIngredientsPerAlergen.find(alergen);
			if (possiblesIngredients == possiblesIngredientsPerAlergen.end())
			{
				possiblesIngredientsPerAlergen[alergen] = dish.ingredients;
			}
			else
			{
				for (auto it = possiblesIngredients->second.begin(); it != possiblesIngredients->second.end();)
				{
					if (dish.ingredients.find(*it) == dish.ingredients.end())
					{
						it = possiblesIngredients->second.erase(it);
					}
					else
					{
						++it;
					}
				}
			}

			assert(possiblesIngredientsPerAlergen[alergen].size() > 0);

			if (possiblesIngredientsPerAlergen[alergen].size() == 1)
			{
				alergenToIngredient[alergen] = *possiblesIngredientsPerAlergen[alergen].begin();
				ingredientToAlergen[*possiblesIngredientsPerAlergen[alergen].begin()] = alergen;
			}
		}
	}

	while (alergenToIngredient.size() < possiblesIngredientsPerAlergen.size())
	{
		for (auto &possibleIngredients : possiblesIngredientsPerAlergen)
		{
			if (possibleIngredients.second.size() > 1)
			{
				assert(alergenToIngredient.find(possibleIngredients.first) == alergenToIngredient.end());
				
				for (auto it = possibleIngredients.second.begin(); it != possibleIngredients.second.end();)
				{
					if (ingredientToAlergen.find(*it) != ingredientToAlergen.end())
					{
						it = possibleIngredients.second.erase(it);
					}
					else
					{
						++it;
					}
				}
				assert(possibleIngredients.second.size() > 0);

				if (possibleIngredients.second.size() == 1)
				{
					alergenToIngredient[possibleIngredients.first] = *possibleIngredients.second.begin();
					ingredientToAlergen[*possibleIngredients.second.begin()] = possibleIngredients.first ;
				}
			}
			else
			{
				assert(alergenToIngredient.find(possibleIngredients.first) != alergenToIngredient.end());
			}
		}
	}

	std::unordered_set<std::string> nonAlergenicIngredients;

	for (std::string const& ingredient : ingredients)
	{
		if (ingredientToAlergen.find(ingredient) == ingredientToAlergen.end())
		{
			nonAlergenicIngredients.insert(ingredient);
		}
	}


	int puzzle1 = 0;
	for (Dish const& dish : dishes)
	{
		for (std::string const& ingredient : dish.ingredients)
		{
			if (nonAlergenicIngredients.find(ingredient) != nonAlergenicIngredients.end())
			{
				++puzzle1;
			}
		}
	}

	std::cout << "Puzzle1: " << puzzle1 << std::endl;
	std::cout << "Puzzle2: ";

	for (auto const &it : alergenToIngredient)
	{
		std::cout << it.second << ',';
	}
	std::cout << std::endl;
}
