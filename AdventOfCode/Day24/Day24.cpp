#include <cassert>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

struct Coord
{
	int x = 0, y = 0;

	bool operator==(Coord const& c) const
	{
		return x == c.x && y == c.y;
	}
};

namespace std
{
	template<>
	class hash<Coord>
	{
	public:
		size_t operator()(Coord const& c) const
		{
			return (size_t)(1783050739ULL + c.x * 2025299389ULL + c.y * 4180580797ULL);
		}
	};
}

int main()
{
	Coord min, max;
	std::unordered_set<Coord> blackTiles;
	{
		std::ifstream infile("input.txt");

		std::string line;
		if (infile.is_open())
		{
			while (std::getline(infile, line))
			{
				Coord coord;
				for (int i = 0; i < (int)line.size(); ++i)
				{
					switch (line[i])
					{
					case 'e':
						++coord.x;
						break;
					case 'w':
						--coord.x;
						break;
					case 'n':
						++coord.y;
						if (line[i + 1] == 'e')
						{
							++coord.x;
						}
						else
						{
							assert(line[i + 1] == 'w');
						}
						++i;
						break;
					case 's':
						--coord.y;
						if (line[i + 1] == 'w')
						{
							--coord.x;
						}
						else
						{
							assert(line[i + 1] == 'e');
						}
						++i;
						break;
					default:
						assert(false);
						break;
					}
				}

				if (coord.x < min.x)
					min.x = coord.x;
				else if (coord.x > max.x)
					max.x = coord.x;
				if (coord.y < min.y)
					min.y = coord.y;
				else if (coord.y > max.y)
					max.y = coord.y;

				auto it = blackTiles.find(coord);
				if (it == blackTiles.end())
				{
					blackTiles.insert(coord);
				}
				else
				{
					blackTiles.erase(it);
				}

			}
		}
	}

	std::cout << "Initial Black Tiles: " << blackTiles.size() << std::endl;


	for (int d = 0; d < 100; ++d)
	{
		std::unordered_set<Coord> next;
		for (int x = min.x - 1; x <= max.x + 1; ++x)
		{
			for (int y = min.y - 1; y <= max.y + 1; ++y)
			{
				bool isBlack = (blackTiles.find(Coord{ x, y }) != blackTiles.end());

				int blackNeighbours = 0;

				if (blackTiles.find(Coord{ x - 1, y }) != blackTiles.end())
					++blackNeighbours;
				if (blackTiles.find(Coord{ x + 1, y }) != blackTiles.end())
					++blackNeighbours;

				if (blackTiles.find(Coord{ x, y + 1 }) != blackTiles.end())
					++blackNeighbours;
				if (blackTiles.find(Coord{ x + 1, y + 1 }) != blackTiles.end())
					++blackNeighbours;

				if (blackTiles.find(Coord{ x, y - 1 }) != blackTiles.end())
					++blackNeighbours;
				if (blackTiles.find(Coord{ x - 1, y - 1 }) != blackTiles.end())
					++blackNeighbours;

				if (isBlack && (blackNeighbours == 1 || blackNeighbours == 2))
				{
					next.insert(Coord{ x, y });
				}
				else if (!isBlack && blackNeighbours == 2)
				{
					next.insert(Coord{ x, y });

					if (x < min.x)
						min.x = x;
					else if (x > max.x)
						max.x = x;
					if (y < min.y)
						min.y = y;
					else if (y > max.y)
						max.y = y;
				}
			}
		}
		blackTiles = std::move(next);


		std::cout << "Black Tiles day " << d + 1 << ": " << blackTiles.size() << std::endl;
	}



}
