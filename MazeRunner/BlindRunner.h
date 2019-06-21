#pragma once
#include "Maze.h"
#include "Runner.h"
#include <string>

class BlindRunner :
	public Runner
{
public:
	BlindRunner(Maze &);
	int getFinishStatus(); // return the status value of a successfully finished game
	int getAbortStatus(); // return the status value of a game aborted by player
	int getInitialStatus(); // return the initial status of a move
	void displayWelcomeMessage(); // display message after choosing game mode
	void move(int &moveStatus); // make a move and change status
	void displayMoveResult(int const &moveResult);
protected:
	Maze *p_maze;
	enum Status { TURNED_LEFT, TURNED_RIGHT, STEP, WALL, ENTRANCE, EXIT, ABORT, MISPRESS, ERROR };

	std::string getStatusName(Status);

	enum Direction { UP, LEFT, DOWN, RIGHT };
	Direction moveDirection;

	std::string getMoveDirectionName(Direction);

	enum Choice {MOVE_FORWARD = 'w', TURN_LEFT = 'a', TURN_RIGHT = 'd', TO_END = 'o'};
	Choice playerChoice;

	int initialRunnerPositionX(); // return X coordinate
	int initialRunnerPositionY(); // return Y coordinate
	Direction initialMoveDirection(); // define an initial move direction based on the coordinates of a maze entrance
	Status turnLeft(); // turn left and return move status
	Status turnRight(); // turn right and return move status
	Status goForward(); // try to make one step forward and return move status

	static const bool DEBUG = false;
};