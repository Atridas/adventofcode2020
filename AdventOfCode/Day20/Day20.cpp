#include <cassert>

#include <fstream>
#include <iostream>
#include <string>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

//*
static constexpr char const* inputFile = "input.txt";
static constexpr int imageSize = 12;
/*/
static constexpr char const* inputFile = "input0.txt";
static constexpr int imageSize = 3;
//*/



static constexpr int seaMonsterWidth = 20;
static constexpr int seaMonsterHeight = 3;
static constexpr char const* seaMonster[] =
{
	"                  # ",
	"#    ##    ##    ###",
	" #  #  #  #  #  #   "
};

struct OrientationValues
{
	unsigned short top = 0, bottom = 0, left = 0, right = 0;
};

struct TilePermutation
{
	char img[10][10];

	OrientationValues borders;

	void computeBorders()
	{
		for (int i = 0; i < 10; ++i)
		{
			if (img[0][i] == '#')
			{
				borders.top |= 1 << (9 - i);
			}
			if (img[9][i] == '#')
			{
				borders.bottom |= 1 << (9 - i);
			}
			if (img[i][0] == '#')
			{
				borders.left |= 1 << (9 - i);
			}
			if (img[i][9] == '#')
			{
				borders.right |= 1 << (9 - i);
			}
		}
	}

	void copy(char _img[10][10])
	{
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				img[i][j] = _img[i][j];
			}
		}

		computeBorders();
	}

	void copyRotatedClockwise(char _img[10][10])
	{
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				img[i][j] = _img[9 - j][i];
			}
		}

		computeBorders();
	}

	void copyMirror(char _img[10][10])
	{
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				img[i][j] = _img[i][9 - j];
			}
		}

		computeBorders();
	}
};

void copy(char out[8 * imageSize][8 * imageSize], char in[8 * imageSize][8 * imageSize])
{
	for (int i = 0; i < 8 * imageSize; ++i)
	{
		for (int j = 0; j < 8 * imageSize; ++j)
		{
			out[i][j] = in[i][j];
		}
	}
}

void copyRotatedClockwise(char out[8 * imageSize][8 * imageSize], char in[8 * imageSize][8 * imageSize])
{
	for (int i = 0; i < 8 * imageSize; ++i)
	{
		for (int j = 0; j < 8 * imageSize; ++j)
		{
			out[i][j] = in[8 * imageSize - 1 - j][i];
		}
	}
}

void copyMirror(char out[8 * imageSize][8 * imageSize], char in[8 * imageSize][8 * imageSize])
{
	for (int i = 0; i < 8 * imageSize; ++i)
	{
		for (int j = 0; j < 8 * imageSize; ++j)
		{
			out[i][j] = in[i][8 * imageSize - 1 - j];
		}
	}
}

struct Tile
{
	int id;
	char img[10][10];

	OrientationValues original, inverted;

	TilePermutation permutations[8];
};

enum class CopyOrientation
{
	Unmodified,
	RotateCounterClockwise,

	SideMirror,
	UpsideDown
};

void copy(char finalImage[8 * imageSize][8 * imageSize], Tile const& tile, int x, int y, CopyOrientation orientation)
{
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			switch (orientation)
			{
			case CopyOrientation::Unmodified:
				finalImage[y * 8 + i][x * 8 + j] = tile.img[i + 1][j + 1];
				break;

			case CopyOrientation::RotateCounterClockwise:
				finalImage[y * 8 + i][x * 8 + j] = tile.img[j + 1][8 - i];
				break;

			case CopyOrientation::SideMirror:
				finalImage[y * 8 + i][x * 8 + j] = tile.img[i + 1][8 - j];
				break;
			case CopyOrientation::UpsideDown:
				finalImage[y * 8 + i][x * 8 + j] = tile.img[8 - i][j + 1];
				break;
			default:
				assert(false);
			}
		}
	}
}



int main()
{
	std::vector<Tile> tiles;


	{
		std::ifstream infile(inputFile);

		std::string line;
		if (infile.is_open())
		{
			while (std::getline(infile, line))
			{
				assert(line.find("Tile ") == 0);
				assert(line.find(':') == line.size() - 1);

				Tile tile;
				tile.id = std::stoi(line.substr(5, line.size() - 6));

				for (int i = 0; i < 10; ++i)
				{
					bool hasLine = (bool)std::getline(infile, line);
					assert(hasLine);
					assert(line.size() == 10);
					for (int j = 0; j < 10; ++j)
					{
						assert(line[j] == '#' || line[j] == '.');
						tile.img[i][j] = line[j];
					}
				}

				tiles.push_back(tile);


				bool hasLine = (bool)std::getline(infile, line);
				assert(hasLine);
			}
		}
	}

	std::unordered_map<int, std::vector<int>> borderMap;

	for (Tile& tile : tiles)
	{
		for (int i = 0; i < 10; ++i)
		{
			if (tile.img[0][i] == '#')
			{
				tile.original.top |= 1 << (9 - i);
				tile.inverted.top |= 1 << i;
			}
			if (tile.img[9][i] == '#')
			{
				tile.original.bottom |= 1 << (9 - i);
				tile.inverted.bottom |= 1 << i;
			}
			if (tile.img[i][0] == '#')
			{
				tile.original.left |= 1 << (9 - i);
				tile.inverted.left |= 1 << i;
			}
			if (tile.img[i][9] == '#')
			{
				tile.original.right |= 1 << (9 - i);
				tile.inverted.right |= 1 << i;
			}
		}

		borderMap[tile.original.top].push_back(tile.id);
		borderMap[tile.inverted.top].push_back(tile.id);

		borderMap[tile.original.bottom].push_back(tile.id);
		borderMap[tile.inverted.bottom].push_back(tile.id);

		borderMap[tile.original.left].push_back(tile.id);
		borderMap[tile.inverted.left].push_back(tile.id);

		borderMap[tile.original.right].push_back(tile.id);
		borderMap[tile.inverted.right].push_back(tile.id);

		tile.permutations[0].copy(tile.img);

		tile.permutations[1].copyRotatedClockwise(tile.permutations[0].img);
		tile.permutations[4].copyMirror(tile.permutations[0].img);

		tile.permutations[2].copyRotatedClockwise(tile.permutations[1].img);
		tile.permutations[5].copyRotatedClockwise(tile.permutations[4].img);

		tile.permutations[3].copyRotatedClockwise(tile.permutations[2].img);
		tile.permutations[6].copyRotatedClockwise(tile.permutations[5].img);

		tile.permutations[7].copyRotatedClockwise(tile.permutations[6].img);
	}

	unsigned long long result1 = 1;

	for (Tile const& tile : tiles)
	{
		int totalCoincidences = 0;
		int coincidences;

		coincidences = (int)borderMap[tile.original.top].size();
		assert(coincidences == 1 || coincidences == 2);
		if (coincidences == 2)
			++totalCoincidences;

		coincidences = (int)borderMap[tile.inverted.top].size();
		assert(coincidences == 1 || coincidences == 2);
		if (coincidences == 2)
			++totalCoincidences;

		coincidences = (int)borderMap[tile.original.bottom].size();
		assert(coincidences == 1 || coincidences == 2);
		if (coincidences == 2)
			++totalCoincidences;

		coincidences = (int)borderMap[tile.inverted.bottom].size();
		assert(coincidences == 1 || coincidences == 2);
		if (coincidences == 2)
			++totalCoincidences;

		coincidences = (int)borderMap[tile.original.left].size();
		assert(coincidences == 1 || coincidences == 2);
		if (coincidences == 2)
			++totalCoincidences;

		coincidences = (int)borderMap[tile.inverted.left].size();
		assert(coincidences == 1 || coincidences == 2);
		if (coincidences == 2)
			++totalCoincidences;

		coincidences = (int)borderMap[tile.original.right].size();
		assert(coincidences == 1 || coincidences == 2);
		if (coincidences == 2)
			++totalCoincidences;

		coincidences = (int)borderMap[tile.inverted.right].size();
		assert(coincidences == 1 || coincidences == 2);
		if (coincidences == 2)
			++totalCoincidences;

		if (totalCoincidences == 4)
		{
			std::cout << "corner found: " << tile.id << std::endl;
			result1 *= tile.id;
		}
		else
		{
			assert(totalCoincidences == 6 || totalCoincidences == 8);
		}
	}

	int downHash[imageSize] = {};
	int sideHash = 0;

	char finalImage[8 * imageSize][8 * imageSize] = {};
	std::unordered_set<int> placedPuzzlePieces;

	for (int y = 0; y < imageSize; ++y)
	{
		for (int x = 0; x < imageSize; ++x)
		{
			TilePermutation permutationFound = {};
			for (Tile const& tile : tiles)
			{
				if (placedPuzzlePieces.find(tile.id) == placedPuzzlePieces.end())
				{
					for (TilePermutation const& permutation : tile.permutations)
					{
						if (x == 0 && y == 0)
						{
							if (borderMap[permutation.borders.top].size() == 1 &&
								borderMap[permutation.borders.right].size() == 2 &&
								borderMap[permutation.borders.bottom].size() == 2 &&
								borderMap[permutation.borders.left].size() == 1)
							{
								placedPuzzlePieces.insert(tile.id);
								permutationFound = permutation;
								goto PERMUTATION_FOUND;
							}
						}
						else if (x == 0)
						{
							if (permutation.borders.top == downHash[0])
							{
								placedPuzzlePieces.insert(tile.id);
								permutationFound = permutation;
								goto PERMUTATION_FOUND;
							}
						}
						else
						{
							if (permutation.borders.left == sideHash)
							{
								if (y != 0) assert(permutation.borders.top == downHash[x]);

								placedPuzzlePieces.insert(tile.id);
								permutationFound = permutation;
								goto PERMUTATION_FOUND;
							}
						}
					}
				}
			}

			assert(false);
		PERMUTATION_FOUND:

			// copy to image
			for (int i = 0; i < 8; ++i)
			{
				for (int j = 0; j < 8; ++j)
				{
					finalImage[y * 8 + i][x * 8 + j] = permutationFound.img[i + 1][j + 1];
				}
			}

			downHash[x] = permutationFound.borders.bottom;
			sideHash = permutationFound.borders.right;
		}
	}



	char finalPermutations[8][8 * imageSize][8 * imageSize] = {};

	copy(finalPermutations[0], finalImage);

	copyRotatedClockwise(finalPermutations[1], finalPermutations[0]);
	copyMirror(finalPermutations[4], finalPermutations[0]);

	copyRotatedClockwise(finalPermutations[2], finalPermutations[1]);
	copyRotatedClockwise(finalPermutations[5], finalPermutations[4]);

	copyRotatedClockwise(finalPermutations[3], finalPermutations[2]);
	copyRotatedClockwise(finalPermutations[6], finalPermutations[5]);

	copyRotatedClockwise(finalPermutations[7], finalPermutations[6]);

	for (int p = 0; p < 8; ++p)
	{
		bool hasSeaMonsters = false;
		for (int i = 0; i < 8 * imageSize - seaMonsterHeight; ++i)
		{
			for (int j = 0; j < 8 * imageSize - seaMonsterWidth; ++j)
			{
				for (int mi = 0; mi < seaMonsterHeight; ++mi)
				{
					for (int mj = 0; mj < seaMonsterWidth; ++mj)
					{
						if (seaMonster[mi][mj] == '#' && finalPermutations[p][i + mi][j + mj] != '#')
						{
							goto SEA_MONSTER_NOT_FOUND;
						}
					}
				}

				hasSeaMonsters = true;
				for (int mi = 0; mi < seaMonsterHeight; ++mi)
				{
					for (int mj = 0; mj < seaMonsterWidth; ++mj)
					{
						if (seaMonster[mi][mj] == '#')
						{
							assert(finalPermutations[p][i + mi][j + mj] == '#');
							finalPermutations[p][i + mi][j + mj] = 'O';
						}
					}
				}

			SEA_MONSTER_NOT_FOUND:
				continue;
			}
		}
		if (hasSeaMonsters)
		{
			int puzzle2 = 0;
			std::cout << std::endl;

			for (int i = 0; i < 8 * imageSize; ++i)
			{
				for (int j = 0; j < 8 * imageSize; ++j)
				{
					std::cout << finalPermutations[p][i][j];
					if (finalPermutations[p][i][j] == '#')
						++puzzle2;
				}
				std::cout << std::endl;
			}

			std::cout << "Puzzle 2: " << puzzle2 << std::endl;
			break;
		}
	}

	/*std::cout << std::endl;

	for (int i = 0; i < 8 * imageSize; ++i)
	{
		for (int j = 0; j < 8 * imageSize; ++j)
		{
			std::cout << finalImage[i][j];
		}
		std::cout << std::endl;
	}*/

	std::cout << "Puzzle 1: " << result1 << std::endl;
}
