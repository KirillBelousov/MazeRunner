#pragma once
#include "BlindRunner.h"
class SightedRunner :
	public BlindRunner
{
public:
	SightedRunner(Maze *);
	void displayWelcomeMessage(); // display message after choosing game mode
	void displayMoveResult();
};

