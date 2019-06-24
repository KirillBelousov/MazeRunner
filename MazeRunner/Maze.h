#pragma once
#include "MazeGenerator.h"

class Maze
{
public:
	Maze(MazeGenerator &);
	
	void initialization();
	
	int getEntranceX();
	void setEntranceX(int);
	
	int getEntranceY();
	void setEntranceY(int);
	
	int getExitX();
	void setExitX(int);

	int getExitY();
	void setExitY(int);

	char getCellType(int X, int Y) const; // return the type of a maze cell at coordinates XY: '.' for a free space, '#' for a wall
	// bool isTheCellAWall (int X, int Y) const
	void setCellToWall(int X, int Y);
	void setCellToFreeSpace(int X, int Y, bool demo = 0);

	bool isXwithinMaze(int X);
	bool isYwithinMaze(int Y);
	bool isCellWithinMaze(int X, int Y);

	int getHight();
	int getWidth();
	
	
	void display(int positionX = -1, int positionY = -1) const;
	void displayVisibleArea(int positionX, int positionY, int direction);
	
private:
	static const int mazeHight = 12;
	static const int mazeWidth = 12;
	char map[mazeHight][mazeWidth];
	int entranceX, entranceY;	// entrance coordinates
	int exitX, exitY; // exit coordinates
};

