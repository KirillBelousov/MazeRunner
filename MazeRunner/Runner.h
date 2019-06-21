#pragma once
class Runner
{
public:
	Runner();
	virtual void move(int &moveResult) = 0; // make a move and change status 
	virtual int getFinishStatus() = 0; // return the status value of a successfully finished game
	virtual int getAbortStatus() = 0; // return the status value of a game aborted by player
	virtual int getInitialStatus() = 0; // return the initial status of a move
	virtual void displayWelcomeMessage() = 0; // display message after choosing game mode
	virtual void displayMoveResult(int const &moveResult) = 0;
protected:
	int runnerX;
	int runnerY; // runner's coordinates
};

