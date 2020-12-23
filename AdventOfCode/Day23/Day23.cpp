#include <cassert>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Cup
{
	int value;

	Cup* nextInCircle, * prevInCircle;
	Cup* nextInValue, * prevInValue;
};

int main()
{
	//constexpr int rounds = 100;
	constexpr int rounds = 10000000;

	//std::vector<int> cupsInput = { 3, 8, 9, 1, 2, 5, 4, 6, 7 };
	std::vector<int> cupsInput = { 6, 2, 4, 3, 9, 7, 1, 5, 8 };


	// don't search for deletes, there are not
	Cup* selectedCup = new Cup{ cupsInput[0] };
	Cup* one = nullptr;


	{
		Cup* lastCup = selectedCup;
		for (int i = 1; i < (int)cupsInput.size(); ++i)
		{
			lastCup = lastCup->nextInCircle = new Cup{ cupsInput[i], selectedCup, lastCup };
		}
		selectedCup->prevInCircle = lastCup;
	}
	{
		Cup* cup = selectedCup;
		Cup* nine = nullptr;
		for (int i : cupsInput)
		{
			if (cup->value == 1)
				one = cup;
			else if (cup->value == 9)
				nine = cup;
			Cup* candidate = selectedCup;
			for (int j : cupsInput)
			{
				if (candidate->value == cup->value - 1)
				{
					cup->prevInValue = candidate;
				}
				else if (candidate->value == cup->value + 1)
				{
					cup->nextInValue = candidate;
				}
				candidate = candidate->nextInCircle;
			}
			cup = cup->nextInCircle;
		}

		assert(one != nullptr);
		assert(nine != nullptr);

		one->prevInValue = nine;
		nine->nextInValue = one;

		{
			Cup* lastCup = selectedCup->prevInCircle;

			lastCup = lastCup->nextInCircle = new Cup{ 10, selectedCup, lastCup, nullptr, nine };
			nine->nextInValue = lastCup;

			for (int c = 11; c <= 1000000; ++c)
			{
				lastCup = lastCup->nextInCircle = lastCup->nextInValue = new Cup{ c, selectedCup, lastCup, one, lastCup };
			}
			selectedCup->prevInCircle = one->prevInValue = lastCup;
		}
	}


	for (int r = 0; r < rounds; ++r)
	{
		// The crab picks up the three cups that are immediately clockwise
		// of the current cup.They are removed from the circle; cup spacing
		// is adjusted as necessary to maintain the circle.
		Cup* firstRemoved = selectedCup->nextInCircle;
		Cup* lastRemoved = firstRemoved->nextInCircle->nextInCircle;

		selectedCup->nextInCircle = lastRemoved->nextInCircle;
		lastRemoved->nextInCircle->prevInCircle = selectedCup;
		
		firstRemoved->prevInCircle = nullptr;
		lastRemoved->nextInCircle = nullptr;

		// The crab selects a destination cup : the cup with a label equal
		// to the current cup's label minus one. If this would select one
		// of the cups that was just picked up, the crab will keep
		// subtracting one until it finds a cup that wasn't just picked up.
		// If at any point in this process the value goes below the lowest
		// value on any cup's label, it wraps around to the highest value
		// on any cup's label instead.
		Cup* destinationCup = selectedCup->prevInValue;
		while (destinationCup == firstRemoved ||
			destinationCup == firstRemoved->nextInCircle ||
			destinationCup == firstRemoved->nextInCircle->nextInCircle)
		{
			destinationCup = destinationCup->prevInValue;
		}
		

		// The crab places the cups it just picked up so that they are
		// immediately clockwise of the destination cup. They keep the same
		// order as when they were picked up.
		firstRemoved->prevInCircle = destinationCup;
		lastRemoved->nextInCircle = destinationCup->nextInCircle;

		destinationCup->nextInCircle->prevInCircle = lastRemoved;
		destinationCup->nextInCircle = firstRemoved;


		//	// The crab selects a new current cup: the cup which is immediately
		//	// clockwise of the current cup.
		selectedCup = selectedCup->nextInCircle;
	}

	/*
	for (Cup* cup = one->nextInCircle; cup != one; cup = cup->nextInCircle)
	{
		std::cout << cup->value;
	}
	std::cout << std::endl;
	/*/
	std::cout << (unsigned long long)one->nextInCircle->value * one->nextInCircle->nextInCircle->value << std::endl;
	//*/
}
