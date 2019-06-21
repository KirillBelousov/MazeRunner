#include "pch.h"
#include "SightedRunner.h"
#include <iostream>


SightedRunner::SightedRunner(Maze &inputMaze) : BlindRunner(inputMaze)
{
}

void SightedRunner::displayWelcomeMessage()
{
	std::cout << "The light picks out a small circle around you. Let's start\n" << std::endl;
	p_maze->displayVisibleArea(runnerX, runnerY, moveDirection);
}

void SightedRunner::displayMoveResult(int const & moveResult)
{
	BlindRunner::displayMoveResult(moveResult);

	if (moveResult != EXIT && moveResult!=ABORT) 
		p_maze->displayVisibleArea(runnerX, runnerY, moveDirection);
}
