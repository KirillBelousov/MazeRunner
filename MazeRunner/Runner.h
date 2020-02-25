#pragma once
class Runner
{
public:
	Runner();
	virtual void move() = 0; // make a move and change status
	virtual void displayWelcomeMessage() = 0; // display message after choosing game mode
	virtual void displayMoveResult() = 0;
	virtual bool isNotEnd() = 0;
protected:
	int runnerX;
	int runnerY; // runner's coordinates
};

