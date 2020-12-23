#include <cassert>

#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


struct GameResult
{
	int winner;
	std::queue<int> deck;

	bool operator==(GameResult const& gr) const
	{
		return winner == gr.winner && deck == gr.deck;
	}
};

struct RoundHistory
{
	std::queue<int> player1, player2;

	bool operator==(RoundHistory const& rh) const
	{
		return player1 == rh.player1 && player2 == rh.player2;
	}

	bool operator!=(RoundHistory const& rh) const
	{
		return !operator==(rh);
	}
};

namespace std
{
	template<>
	class hash<RoundHistory>
	{
	public:
		size_t operator()(RoundHistory const& rh) const
		{
			return (size_t)(1783050739ULL + rh.player1.size() * 2025299389ULL + rh.player2.size() * 4180580797ULL + rh.player1.front() * 564635177ULL + rh.player2.front() * 2840788999ULL);
		}
	};
}

GameResult recursiveCombat(std::unordered_map<RoundHistory, GameResult>& resultHistory, std::queue<int> _player1, std::queue<int> _player2, int _size1, int _size2)
{
	std::queue<int> player1, player2;

	// preparations
	while ((int)player1.size() < _size1)
	{
		player1.push(_player1.front());
		_player1.pop();
	}
	while ((int)player2.size() < _size2)
	{
		player2.push(_player2.front());
		_player2.pop();
	}

	RoundHistory thisGame = { player1, player2 };

	/*{
		auto it = resultHistory.find(thisGame);
		if (it != resultHistory.end())
		{
			return it->second;
		}
	}*/


	std::unordered_set<RoundHistory> roundHistory;


	// main event
	while (player1.size() > 0 && player2.size() > 0)
	{
		RoundHistory thisRound = { player1, player2 };



		//  Before either player deals a card, if there was a previous round in 
		// this game that had exactly the same cards in the same order in the same
		// players' decks, the game instantly ends in a win for player 1. Previous
		// rounds from other games are not considered. (This prevents infinite
		// games of Recursive Combat, which everyone agrees is a bad idea.)
		if (roundHistory.find(thisRound) != roundHistory.end())
		{
			GameResult result = { 0, std::move(player1) };
			//resultHistory[thisGame] = result;
			return result;
		}
		else
		{
			roundHistory.insert(std::move(thisRound));

			int p1 = player1.front();
			int p2 = player2.front();

			player1.pop();
			player2.pop();

			// If both players have at least as many cards remaining in their deck
			// as the value of the card they just drew, the winner of the round is
			// determined by playing a new game of Recursive Combat (see below).
			if (p1 <= (int)player1.size() && p2 <= (int)player2.size())
			{
				GameResult subgame = recursiveCombat(resultHistory, player1, player2, p1, p2);
				if (subgame.winner == 0)
				{
					player1.push(p1);
					player1.push(p2);
				}
				else
				{
					assert(subgame.winner == 1);
					player2.push(p2);
					player2.push(p1);
				}
			}
			// Otherwise, at least one player must not have enough cards left in
			// their deck to recurse; the winner of the round is the player with
			// the higher-value card
			else if (p1 > p2)
			{
				player1.push(p1);
				player1.push(p2);
			}
			else
			{
				assert(p2 > p1);

				player2.push(p2);
				player2.push(p1);
			}
		}

	}

	GameResult result;

	if (player1.size() > 0)
	{
		assert(player2.size() == 0);
		result = GameResult{ 0, std::move(player1) };
	}
	else
	{
		assert(player2.size() > 0);
		result = GameResult{ 1, std::move(player2) };
	}

	/*{
		auto it = resultHistory.find(thisGame);
		if (it != resultHistory.end())
		{
			assert(it->second == result);
		}
	}*/

	//resultHistory[thisGame] = result;

	return std::move(result);
}

GameResult recursiveCombat(std::queue<int> _player1, std::queue<int> _player2)
{
	std::unordered_map<RoundHistory, GameResult> resultHistory;
	return recursiveCombat(resultHistory, _player1, _player2, (int)_player1.size(), (int)_player2.size());
}

int main()
{
	std::queue<int> player1, player2;

	{
		std::unordered_set<int> repeated;
		std::ifstream infile("input.txt");

		std::string line;
		if (infile.is_open())
		{
			std::getline(infile, line);
			assert(line == "Player 1:");

			while (std::getline(infile, line) && line != "")
			{
				player1.push(std::stoi(line));

				if (repeated.find(player1.back()) == repeated.end())
				{
					repeated.insert(player1.back());
				}
				else
				{
					assert(false);
				}
			}


			std::getline(infile, line);
			assert(line == "Player 2:");

			while (std::getline(infile, line))
			{
				player2.push(std::stoi(line));

				if (repeated.find(player2.back()) == repeated.end())
				{
					repeated.insert(player2.back());
				}
				else
				{
					assert(false);
				}
			}
		}
	}

	GameResult game2 = recursiveCombat(player1, player2);

	while (player1.size() > 0 && player2.size() > 0)
	{
		int p1 = player1.front();
		int p2 = player2.front();

		player1.pop();
		player2.pop();

		if (p1 > p2)
		{
			player1.push(p1);
			player1.push(p2);
		}
		else
		{
			assert(p2 > p1);

			player2.push(p2);
			player2.push(p1);
		}
	}

	assert(player1.size() == 0 || player2.size() == 0);

	unsigned long long points = 0;
	{
		unsigned long long multiplier = player1.size();
		while (!player1.empty())
		{
			points += multiplier * player1.front();
			--multiplier;
			player1.pop();
		}
	}
	{
		unsigned long long multiplier = player2.size();
		while (!player2.empty())
		{
			points += multiplier * player2.front();
			--multiplier;
			player2.pop();
		}
	}

	unsigned long long points2 = 0;
	{
		std::cout << "Winner 2: " << game2.winner << std::endl;
		unsigned long long multiplier = game2.deck.size();
		while (!game2.deck.empty())
		{
			std::cout << ", " << game2.deck.front();
			points2 += multiplier * game2.deck.front();
			--multiplier;
			game2.deck.pop();
		}
		std::cout << std::endl;
	}

	std::cout << "Puzzle 1: " << points << std::endl;
	std::cout << "Puzzle 2: " << points2 << std::endl;
}
