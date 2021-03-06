#pragma once
#include "Maze.h"
#include "Runner.h"
#include <string>

class BlindRunner :
	public Runner
{
public:
	BlindRunner(Maze *);
	void displayWelcomeMessage(); // display message after choosing game mode
	void move(); // make a move and change status
	void automove();
	void displayMoveResult();
	bool isNotEnd();
protected:
	Maze *p_maze;
	
	enum Status { STATUS_INITIAL, STATUS_TURNED_LEFT, STATUS_TURNED_RIGHT, STATUS_STEP, 
		STATUS_WALL, STATUS_ENTRANCE, STATUS_EXIT, STATUS_ABORT, STATUS_MISPRESS, STATUS_ERROR };
	Status moveStatus;
	std::string getStatusName(Status);

	enum Direction { DIRECTION_UP, DIRECTION_LEFT, DIRECTION_DOWN, DIRECTION_RIGHT };
	Direction moveDirection;
	std::string getMoveDirectionName(Direction);

	enum Choice {CHOICE_MOVE_FORWARD = 'w', CHOICE_TURN_LEFT = 'a', CHOICE_TURN_RIGHT = 'd', 
		CHOICE_TO_END = 'o'};
	Choice playerChoice;

	int initialRunnerPositionX(); // return X coordinate
	int initialRunnerPositionY(); // return Y coordinate
	Direction initialMoveDirection(); // define an initial move direction based on 
									  // the coordinates of a maze entrance
	Status initialStatus(); // return the initial status of a move
	Status turnLeft(); // turn left and return move status
	Status turnRight(); // turn right and return move status
	Status goForward(); // try to make one step forward and return move status

	static const bool DEBUG = false;
};