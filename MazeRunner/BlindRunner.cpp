#include "pch.h"
#include "BlindRunner.h"
#include <iostream>
#include <cstdlib>
#include <conio.h>


BlindRunner::BlindRunner(Maze *p_inputMaze)
{
	p_maze = p_inputMaze;
	runnerX = initialRunnerPositionX();
	runnerY = initialRunnerPositionY();
	moveDirection = initialMoveDirection();
}

int BlindRunner::getFinishStatus()
{
	return EXIT;
}

int BlindRunner::getAbortStatus()
{
	return ABORT;
}

int BlindRunner::getInitialStatus()
{
	return ENTRANCE;
}

void BlindRunner::displayWelcomeMessage()
{
	std::cout << "You will have to go by touch :(. Let's start\n" << std::endl;
}

void BlindRunner::move(int & moveStatus)
{
	std::cout << "Press '" << static_cast <char> (MOVE_FORWARD) << "' to move forward, '"
						   << static_cast <char> (TURN_LEFT) <<"' to turn left, '"
						   << static_cast <char> (TURN_RIGHT) <<"' to turn right, '"
						   << static_cast <char> (TO_END) << "' to end the game" << std::endl;
	char choice = _getwch();
	if (!DEBUG) system("cls");
	else std::cout << "Your choice: " << choice << std::endl;
	switch (choice)
	{
		case TURN_LEFT:
			moveStatus = turnLeft();
			break;
		case TURN_RIGHT:
			moveStatus = turnRight();
			break;
		case MOVE_FORWARD:
			moveStatus = goForward();
			break;
		case TO_END:
			moveStatus = ABORT;
			break;
		default:
			moveStatus = MISPRESS;
			break;
	}

	if (!DEBUG) system("cls");
	else
	{
		std::cout << "The move direction: " << getMoveDirectionName(moveDirection) << std::endl;
		std::cout << "The move status: " << getStatusName(static_cast<Status>(moveStatus)) << std::endl;
	}
}

int BlindRunner::initialRunnerPositionX()
{
	return p_maze->getEntranceX();
}

int BlindRunner::initialRunnerPositionY()
{
	return p_maze->getEntranceY();
}

BlindRunner::Direction BlindRunner::initialMoveDirection()
{
	if (p_maze->getEntranceX() * p_maze->getEntranceY() == 0) // left or up boundary
		if (p_maze->getEntranceX() == 0)       // left boundary
			return RIGHT;
		else                      // up boundary 
			return DOWN;
	else                          // down or right boundary
		if (p_maze->getEntranceX() == p_maze->getWidth() - 1) // right boundary
			return LEFT;
		else                            // down boundary
			return UP;
}

BlindRunner::Status BlindRunner::turnLeft()
{
	int hold = moveDirection;
	hold++;
	if (hold == 4) // if out of enumeration range
		hold = 0;
	moveDirection = static_cast<Direction>(hold);
	return TURNED_LEFT;
}

BlindRunner::Status BlindRunner::turnRight()
{
	int hold = moveDirection;
	hold--;
	if (hold == -1) // if out of enumeration range
		hold = 3;
	moveDirection = static_cast<Direction>(hold);
	return TURNED_RIGHT;
}

BlindRunner::Status BlindRunner::goForward()
{
	int holdX = runnerX;
	int holdY = runnerY;
	
	switch (moveDirection) // change holders
	{
	case UP:
		holdY--;
		break;
	case LEFT:
		holdX--;
		break;
	case DOWN:
		holdY++;
		break;
	case RIGHT:
		holdX++;
		break;
	default:
		std::cout << "Enumiration for move direction is out of range" << std::endl;
		return ERROR;
		break;
	}

	if (holdX >= 0 && holdY >= 0 && holdX < p_maze->getWidth() && holdY < p_maze->getHight()) // if a new position is within maze coordinates
		if (p_maze->getCellType(holdX, holdY) == '#') // if a new position is at a wall
		{
			return WALL; // runner's coordinates remain unchanged
		}
		else
		{
			if (p_maze->getCellType(holdX, holdY) == '.') // if a new position is at a free space
			{
				runnerX = holdX;
				runnerY = holdY; // change runner's coordinates
				if (runnerX == p_maze->getEntranceX() && runnerY == p_maze->getEntranceY()) // and if the new position is enrance
					return ENTRANCE;
				else
					if (runnerX == p_maze->getExitX() && runnerY == p_maze->getExitY()) // else if the new position is exit
						return EXIT;
					else													  // else it is just a one more step
						return STEP;
			}
			else // if a maze cell neither '#' nor '.'
			{
				std::cout << "Undefined type of the maze cell: " << p_maze->getCellType(holdX, holdY) << std::endl;
				return ERROR;
			}
		}
	else
		return WALL;	// if a new position is out of maze coordinates runner's coordinates remain unchanged and let move status be WALL
}

void BlindRunner::displayMoveResult(int const & moveResult)
{
	switch (moveResult)
	{
	case TURNED_LEFT:
		std::cout << "You turn left\n" << std::endl;
		break;
	case TURNED_RIGHT:
		std::cout << "You turn right\n" << std::endl;
		break;
	case STEP:
		std::cout << "You take a step forward\n" << std::endl;
		break;
	case WALL:
		std::cout << "There is a wall\n" << std::endl;
		break;
	case ENTRANCE:
		std::cout << "You are again at the entrance\n" << std::endl;
		break;
	case EXIT:
		std::cout << "CONGRATULATIONS!!! You reach the exit!\n" << std::endl;
		p_maze->display(runnerX, runnerY);
		break;
	case ABORT:
		std::cout << "You give up. It is so sad\n" << std::endl;
		p_maze->display(runnerX, runnerY);
		break;
	case MISPRESS:
		std::cout << "You've pressed a wrong key\n" << std::endl;
		break;
	case ERROR:
		std::cout << "The error occurred\n" << std::endl;
		break;
	default:
		std::cout << "Enumeration of move results is out of range\n" << std::endl;
		break;
	}
}

std::string BlindRunner::getStatusName(Status inputStatus)
{
	switch (inputStatus)
	{
	case TURNED_LEFT:
		return "TURNED_LEFT";
	case TURNED_RIGHT:
		return "TURNED_RIGHT";
	case STEP:
		return "STEP";
	case WALL:
		return "WALL";
	case ENTRANCE:
		return "ENTRANCE";
	case EXIT:
		return "EXIT";
	case ABORT:
		return "ABORT";
	case MISPRESS:
		return "MISPRESS";
	case ERROR:
		return "ERROR";
	default:
		return "Unknown status";
	}
}

std::string BlindRunner::getMoveDirectionName(Direction inputDirection)
{
	switch (inputDirection)
	{
	case UP:
		return "UP";
	case LEFT:
		return "LEFT";
	case DOWN:
		return "DOWN";
	case RIGHT:
		return "RIGHT";
	default:
		return "Unknown direction";
	}
}
