#include "Heuristik.h"
#include <iostream>

Heuristik::Heuristik(Ownership first, Ownership second, Ownership third, Ownership fourth, Ownership self)
{
	tokens.push_back(first);
	tokens.push_back(second);
	tokens.push_back(third);
	tokens.push_back(fourth);
	target = self;
}

Heuristik::~Heuristik()
{
}

int Heuristik::getHeuristik()
{
	int score = 0;
	int ownCount = std::count(tokens.begin(), tokens.end(), target);
	int emptyCount = std::count(tokens.begin(), tokens.end(), NONE);

	DebugOutput();

	if (ownCount == 4) score += 1000000;
	if (ownCount == 3 && emptyCount == 1) score += 10000;
	if (ownCount == 2 && emptyCount == 2) score += 1000;

	//Konter von Gegner 3er
	if (ownCount == 0 && emptyCount == 0) score += 900000;

	if (ownCount == 0 && emptyCount == 2) score += -1000;
	if (ownCount == 0 && emptyCount == 1) score += -10000;
	if (ownCount == 0 && emptyCount == 0) score += -1000000;

	return score;
}

void Heuristik::DebugOutput()
{
	for(auto i : tokens)
	{
		std::cout << i;
	}
	std::cout << std::endl;
}