#include "pch.h"
#include "SightedRunner.h"
#include <iostream>


SightedRunner::SightedRunner(Maze *inputMaze) : BlindRunner(inputMaze)
{
}

void SightedRunner::displayWelcomeMessage()
{
	if (!DEBUG)
		system("cls");
	std::cout << "The light picks out a small circle around you. Let's start\n" << std::endl;
	p_maze->displayVisibleArea(runnerX, runnerY, moveDirection);
}

void SightedRunner::displayMoveResult()
{
	BlindRunner::displayMoveResult();

	if (isNotEnd()) 
		p_maze->displayVisibleArea(runnerX, runnerY, moveDirection);
}
