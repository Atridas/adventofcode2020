#include <cassert>

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

struct Coord
{
	Coord() = default;
	Coord(int x, int y, int z) : x(x), y(y), z(z), w(0) {}
	Coord(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}

	int x, y, z, w;
};

Coord operator+(Coord const& a, Coord const& b)
{
	return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

bool operator==(Coord const& a, Coord const& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

bool operator!=(Coord const& a, Coord const& b)
{
	return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
}

namespace std
{
	template<>
	class hash<Coord>
	{
	public:
		size_t operator()(Coord const& v) const
		{
			return (size_t)(1783050739ULL + v.x * 2025299389ULL + v.y * 4180580797ULL + v.z * 564635177ULL + v.w * 2840788999ULL);
		}
	};
}

void printState(std::unordered_set<Coord> const& conwayCubes, int minX, int minY, int minZ, int maxX, int maxY, int maxZ)
{
	for (int z = minZ; z <= maxZ; ++z)
	{
		std::cout << "z=" << z << std::endl;
		for (int y = minY; y <= maxY; ++y)
		{
			std::cout << '\t';
			for (int x = minX; x <= maxX; ++x)
			{
				if (conwayCubes.find(Coord{ x, y, z }) != conwayCubes.end())
				{
					std::cout << '#';
				}
				else
				{
					std::cout << '.';
				}
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}

void printState(std::unordered_set<Coord> const& conwayCubes, int minX, int minY, int minZ, int minW, int maxX, int maxY, int maxZ, int maxW)
{
	for (int w = minW; w <= maxW; ++w)
	{
		for (int z = minZ; z <= maxZ; ++z)
		{
			std::cout << "z=" << z << " w=" << w << std::endl;
			for (int y = minY; y <= maxY; ++y)
			{
				std::cout << '\t';
				for (int x = minX; x <= maxX; ++x)
				{
					if (conwayCubes.find(Coord{ x, y, z }) != conwayCubes.end())
					{
						std::cout << '#';
					}
					else
					{
						std::cout << '.';
					}
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}
	}
}

int problem1(std::unordered_set<Coord> conwayCubes, int minX, int minY, int minZ, int maxX, int maxY, int maxZ)
{
	Coord neighbourDiff[] = {
		{-1, -1, -1},
		{-1, -1,  0},
		{-1, -1,  1},
		{-1,  0, -1},
		{-1,  0,  0},
		{-1,  0,  1},
		{-1,  1, -1},
		{-1,  1,  0},
		{-1,  1,  1},
		{ 0, -1, -1},
		{ 0, -1,  0},
		{ 0, -1,  1},
		{ 0,  0, -1},
		//{ 0,  0,  0},
		{ 0,  0,  1},
		{ 0,  1, -1},
		{ 0,  1,  0},
		{ 0,  1,  1},
		{ 1, -1, -1},
		{ 1, -1,  0},
		{ 1, -1,  1},
		{ 1,  0, -1},
		{ 1,  0,  0},
		{ 1,  0,  1},
		{ 1,  1, -1},
		{ 1,  1,  0},
		{ 1,  1,  1}
	};

	for (int i = 0; i < 6; ++i)
	{
		std::unordered_set<Coord> nextConwayCubes;

		for (int x = minX - 1; x <= maxX + 1; ++x)
			for (int y = minY - 1; y <= maxY + 1; ++y)
				for (int z = minZ - 1; z <= maxZ + 1; ++z)
				{
					int neighbours = 0;
					Coord coord{ x, y, z };

					for (Coord n : neighbourDiff)
					{
						if (conwayCubes.find(n + coord) != conwayCubes.end())
						{
							++neighbours;
						}
					}

					if (neighbours == 3 || (neighbours == 2 && conwayCubes.find(coord) != conwayCubes.end()))
					{
						nextConwayCubes.emplace(x, y, z);
						minX = std::min(x, minX);
						minY = std::min(y, minY);
						minZ = std::min(z, minZ);
						maxX = std::max(x, maxX);
						maxY = std::max(y, maxY);
						maxZ = std::max(z, maxZ);
					}
				}

		conwayCubes = std::move(nextConwayCubes);
		std::cout << "iteration = " << (i + 1) << std::endl;
		std::cout << "total cubes = " << conwayCubes.size() << std::endl;
		printState(conwayCubes, minX, minY, minZ, maxX, maxY, maxZ);
	}

	return (int)conwayCubes.size();
}

int problem2(std::unordered_set<Coord> conwayCubes, int minX, int minY, int minZ, int minW, int maxX, int maxY, int maxZ, int maxW)
{
	std::vector<Coord> neighbourDiff;
	for (int w = -1; w <= 1; ++w)
		for (int z = -1; z <= 1; ++z)
			for (int y = -1; y <= 1; ++y)
				for (int x = -1; x <= 1; ++x)
				{
					Coord coord{ x, y, z, w };
					if (coord != Coord{ 0, 0, 0, 0 })
					{
						neighbourDiff.push_back(coord);
					}
				}


	for (int i = 0; i < 6; ++i)
	{
		std::unordered_set<Coord> nextConwayCubes;

		for (int x = minX - 1; x <= maxX + 1; ++x)
			for (int y = minY - 1; y <= maxY + 1; ++y)
				for (int z = minZ - 1; z <= maxZ + 1; ++z)
					for (int w = minW - 1; w <= maxW + 1; ++w)
					{
						int neighbours = 0;
						Coord coord{ x, y, z, w };

						for (Coord n : neighbourDiff)
						{
							if (conwayCubes.find(n + coord) != conwayCubes.end())
							{
								++neighbours;
							}
						}

						if (neighbours == 3 || (neighbours == 2 && conwayCubes.find(coord) != conwayCubes.end()))
						{
							nextConwayCubes.emplace(x, y, z, w);
							minX = std::min(x, minX);
							minY = std::min(y, minY);
							minZ = std::min(z, minZ);
							minW = std::min(w, minW);
							maxX = std::max(x, maxX);
							maxY = std::max(y, maxY);
							maxZ = std::max(z, maxZ);
							maxW = std::max(w, maxW);
						}
					}

		conwayCubes = std::move(nextConwayCubes);
		std::cout << "iteration = " << (i + 1) << std::endl;
		std::cout << "total cubes = " << conwayCubes.size() << std::endl;
		printState(conwayCubes, minX, minY, minZ, minW, maxX, maxY, maxZ, maxW);
	}

	return (int)conwayCubes.size();
}

int main()
{
	int minX = 0;
	int minY = 0;
	int minZ = 0;
	int minW = 0;
	int maxX = 0;
	int maxY = 0;
	int maxZ = 0;
	int maxW = 0;

	std::unordered_set<Coord> conwayCubes;

	std::ifstream infile("input.txt");

	std::string line;
	if (infile.is_open())
	{
		int y = 0;
		while (std::getline(infile, line))
		{
			for (int x = 0; x < (int)line.size(); ++x)
			{
				char c = line[x];
				if (c == '#')
				{
					conwayCubes.emplace(x, y, 0);
					minX = std::min(x, minX);
					minY = std::min(y, minY);
					maxX = std::max(x, maxX);
					maxY = std::max(y, maxY);
				}
				else
				{
					assert(c == '.');
				}
			}
			++y;
		}
	}

	printState(conwayCubes, minX, minY, minZ, minW, maxX, maxY, maxZ, maxW);

	int result1 = problem1(conwayCubes, minX, minY, minZ, maxX, maxY, maxZ);
	int result2 = problem2(conwayCubes, minX, minY, minZ, minW, maxX, maxY, maxZ, maxW);

	std::cout << "solution 1: " << result1 << std::endl;
	std::cout << "solution 2: " << result2 << std::endl;
}
