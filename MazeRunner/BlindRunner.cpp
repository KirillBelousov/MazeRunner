#include "pch.h"
#include "BlindRunner.h"
#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#include <exception>

BlindRunner::BlindRunner(Maze *p_inputMaze)
{
	p_maze = p_inputMaze;
	runnerX = initialRunnerPositionX();
	runnerY = initialRunnerPositionY();
	moveDirection = initialMoveDirection();
}

int BlindRunner::getFinishStatus()
{
	return STATUS_EXIT;
}

int BlindRunner::getAbortStatus()
{
	return STATUS_ABORT;
}

int BlindRunner::getInitialStatus()
{
	return STATUS_INITIAL;
}

void BlindRunner::displayWelcomeMessage()
{
	std::cout << "You will have to go by touch :(. Let's start\n" << std::endl;
}

void BlindRunner::move(int & moveStatus)
{
	std::cout << "Press '" << static_cast <char> (CHOICE_MOVE_FORWARD) << "' to move forward, '"
						   << static_cast <char> (CHOICE_TURN_LEFT) <<"' to turn left, '"
						   << static_cast <char> (CHOICE_TURN_RIGHT) <<"' to turn right, '"
						   << static_cast <char> (CHOICE_TO_END) << "' to end the game" 
						   << std::endl;
	char choice = _getwch();
	if (!DEBUG) 
		system("cls");
	else std::cout << "Your choice: " << choice << std::endl;
	switch (choice)
	{
		case CHOICE_TURN_LEFT:
			moveStatus = turnLeft();
			break;
		case CHOICE_TURN_RIGHT:
			moveStatus = turnRight();
			break;
		case CHOICE_MOVE_FORWARD:
			moveStatus = goForward();
			break;
		case CHOICE_TO_END:
			moveStatus = STATUS_ABORT;
			break;
		default:
			moveStatus = STATUS_MISPRESS;
			break;
	}

	if (!DEBUG) 
		system("cls");
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
			return DIRECTION_RIGHT;
		else                      // up boundary 
			return DIRECTION_DOWN;
	else                          // down or right boundary
		if (p_maze->getEntranceX() == p_maze->getWidth() - 1) // right boundary
			return DIRECTION_LEFT;
		else                            // down boundary
			return DIRECTION_UP;
}

BlindRunner::Status BlindRunner::turnLeft()
{
	int hold = moveDirection;
	hold++;
	if (hold == 4) // if out of enumeration range
		hold = 0;
	moveDirection = static_cast<Direction>(hold);
	return STATUS_TURNED_LEFT;
}

BlindRunner::Status BlindRunner::turnRight()
{
	int hold = moveDirection;
	hold--;
	if (hold == -1) // if out of enumeration range
		hold = 3;
	moveDirection = static_cast<Direction>(hold);
	return STATUS_TURNED_RIGHT;
}

BlindRunner::Status BlindRunner::goForward()
{
	int holdX = runnerX;
	int holdY = runnerY;
	
	switch (moveDirection) // change holders
	{
	case DIRECTION_UP:
		holdY--;
		break;
	case DIRECTION_LEFT:
		holdX--;
		break;
	case DIRECTION_DOWN:
		holdY++;
		break;
	case DIRECTION_RIGHT:
		holdX++;
		break;
	default:
		std::cout << "Enumiration for move direction is out of range" << std::endl;
		return STATUS_ERROR;
		break;
	}
	
	// if a new position is within maze coordinates
	if (holdX >= 0 && holdY >= 0 && holdX < p_maze->getWidth() && holdY < p_maze->getHight()) 
		if (p_maze->getCellType(holdX, holdY) == '#') // if a new position is at a wall
		{
			return STATUS_WALL; // runner's coordinates remain unchanged
		}
		else
		{
			if (p_maze->getCellType(holdX, holdY) == '.') // if a new position is at a free space
			{
				runnerX = holdX;
				runnerY = holdY; // change runner's coordinates
				// and if the new position is enrance
				if (runnerX == p_maze->getEntranceX() && runnerY == p_maze->getEntranceY()) 
					return STATUS_ENTRANCE;
				else  // if the new position is exit
					if (runnerX == p_maze->getExitX() && runnerY == p_maze->getExitY())
						return STATUS_EXIT;
					else // it is just a one more step
						return STATUS_STEP;
			}
			else // if a maze cell neither '#' nor '.'
			{
				std::cout << "Undefined type of the maze cell: " 
					      << p_maze->getCellType(holdX, holdY) 
					      << std::endl;
				return STATUS_ERROR;
			}
		}
	else // if a new position is out of maze coordinates runner's coordinates remain unchanged
		// and if an old position is enrance
		if (runnerX == p_maze->getEntranceX() && runnerY == p_maze->getEntranceY()) 
			return STATUS_ENTRANCE;
		else // let a move status be WALL
			return STATUS_WALL;
}

void BlindRunner::displayMoveResult(int const & moveResult)
{
	switch (moveResult)
	{
	case STATUS_TURNED_LEFT:
		std::cout << "You turn left\n" << std::endl;
		break;
	case STATUS_TURNED_RIGHT:
		std::cout << "You turn right\n" << std::endl;
		break;
	case STATUS_STEP:
		std::cout << "You take a step forward\n" << std::endl;
		break;
	case STATUS_WALL:
		std::cout << "There is a wall\n" << std::endl;
		break;
	case STATUS_ENTRANCE:
		std::cout << "You are again at the entrance\n" << std::endl;
		break;
	case STATUS_EXIT:
		std::cout << "CONGRATULATIONS!!! You reach the exit!\n" << std::endl;
		p_maze->display(runnerX, runnerY);
		break;
	case STATUS_ABORT:
		std::cout << "You give up. It is so sad\n" << std::endl;
		p_maze->display(runnerX, runnerY);
		break;
	case STATUS_MISPRESS:
		std::cout << "You've pressed a wrong key\n" << std::endl;
		break;
	case STATUS_ERROR:
		std::cout << "The error occurred\n" << std::endl;
		break;
	default:
		std::cout << "Enumeration of move results is out of range\n" << std::endl;
		break;
	}
}

void BlindRunner::autorun()
{
	int moveStatus = getInitialStatus();
	while (moveStatus != STATUS_ENTRANCE && moveStatus != STATUS_EXIT && moveStatus != STATUS_ERROR)
	{
		moveStatus = turnLeft();
		moveStatus = goForward();
		while (moveStatus != STATUS_ENTRANCE && moveStatus != STATUS_EXIT && moveStatus != STATUS_ERROR && moveStatus != STATUS_STEP)
		{
			moveStatus = turnRight();
			moveStatus = goForward();
		}
		if (DEBUG)
		{
			system("cls");
			std::cout << "You are run by autorunner\n" << std::endl;
			p_maze->display(runnerX, runnerY);
			Sleep(500);
		}
	}
	if (moveStatus == STATUS_ENTRANCE)
	{
		throw std::runtime_error("Error! Autorunner isn't able to reach exit. It has returned to entrance.");
	}
	if (moveStatus == STATUS_ERROR)
	{
		throw std::runtime_error("Error! Autorunner isn't able to reach exit. A move has got error status.");
	}
}

std::string BlindRunner::getStatusName(Status inputStatus)
{
	switch (inputStatus)
	{
	case STATUS_TURNED_LEFT:
		return "TURNED_LEFT";
	case STATUS_TURNED_RIGHT:
		return "TURNED_RIGHT";
	case STATUS_STEP:
		return "STEP";
	case STATUS_WALL:
		return "WALL";
	case STATUS_ENTRANCE:
		return "ENTRANCE";
	case STATUS_EXIT:
		return "EXIT";
	case STATUS_ABORT:
		return "ABORT";
	case STATUS_MISPRESS:
		return "MISPRESS";
	case STATUS_ERROR:
		return "ERROR";
	default:
		return "Unknown status";
	}
}

std::string BlindRunner::getMoveDirectionName(Direction inputDirection)
{
	switch (inputDirection)
	{
	case DIRECTION_UP:
		return "UP";
	case DIRECTION_LEFT:
		return "LEFT";
	case DIRECTION_DOWN:
		return "DOWN";
	case DIRECTION_RIGHT:
		return "RIGHT";
	default:
		return "Unknown direction";
	}
}
