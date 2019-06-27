#include "pch.h"
#include "Maze.h"
#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <exception>

Maze::Maze(MazeGenerator &generator)
{
	generator.run(*this);
}

void Maze::initialization()
{
	for (int i = 0; i < mazeHight; i++)
	{
		for (int j = 0; j < mazeWidth; j++)
		{
			map[i][j] = '#';
		}
	}
}

int Maze::getEntranceX()
{
	return entranceX;
}

void Maze::setEntranceX(int coordinate)
{
	if (isXwithinMaze(coordinate))
		entranceX = coordinate;
	else
	{
		throw std::runtime_error("Error! Entrance X coordinate is out of maze during generation.");
	}
}

int Maze::getEntranceY()
{
	return entranceY;
}

void Maze::setEntranceY(int coordinate)
{
	if (isYwithinMaze(coordinate))
		entranceY = coordinate;
	else
	{
		throw std::runtime_error("Error! Entrance Y coordinate is out of maze during generation.");
	}
}

int Maze::getExitX()
{
	return exitX;
}

void Maze::setExitX(int coordinate)
{
	if (isXwithinMaze(coordinate))
		exitX = coordinate;
	else
	{
		throw std::runtime_error("Error! Exit X coordinate is out of maze during generation.");
	}
}

int Maze::getExitY()
{
	return exitY;
}

void Maze::setExitY(int coordinate)
{
	if (isYwithinMaze(coordinate))
		exitY = coordinate;
	else
	{
		throw std::runtime_error("Error! Exit Y coordinate is out of maze during generation");
	}
}

bool Maze::isXwithinMaze(int X)
{
	
	return (X >= 0 && X <= mazeWidth - 1);
}

bool Maze::isYwithinMaze(int Y)
{
	return (Y >= 0 && Y <= mazeHight - 1);
}

bool Maze::isCellWithinMaze(int X, int Y)
{
	return (isXwithinMaze(X) && isYwithinMaze(Y));
}

int Maze::getHight()
{
	return mazeHight;
}

int Maze::getWidth()
{
	return mazeWidth;
}

char Maze::getCellType(int X, int Y) const
{
	return map[Y][X];
}

void Maze::setCellToWall(int X, int Y)
{
	if (isCellWithinMaze(X, Y))
		map[Y][X] = '#';
	else
		std::cout << "The cell coordinate is out of range!" << std::endl;
}

void Maze::setCellToFreeSpace(int X, int Y, bool demo)
{
	if (isCellWithinMaze(X, Y))
		map[Y][X] = '.';
	else
		std::cout << "The cell coordinate is out of range!" << std::endl;
	if (demo)
	{
		system("cls");
		std::cout << "You are gnawing passages within maze\n" << std::endl;
		display(X, Y);
		Sleep(500);
	}
}

void Maze::display(int positionX, int positionY) const
{
	for (int i = 0; i < mazeHight; i++)
	{
		for (int j = 0; j < mazeWidth; j++)
		{
			if (i != positionY || j != positionX)
				std::cout << ' ' << map[i][j] ;
			else
				std::cout << ' ' << 'X';
		}
		std::cout << "\n" ;
	}
	if (positionX >=0) std::cout << "\nX - you are here" << std::endl;
}

void Maze::displayVisibleArea(int positionX, int positionY, int directionOfMove)
{
	const int areaSideLength = 3; // the length of a square visible area side
	const int upLeftCornerX = positionX - 1;
	const int upLeftCornerY = positionY - 1; // the place of an up left corner of a visible area in a maze coordinates
	const int centerOfView = 1; // the place of a view center in a view coordinates
	const int COS90 = 0; // cosine of 90 degrees
	const int SIN90 = 1; // sine of 90 degrees
	char view[areaSideLength][areaSideLength]; // the view of a visible area
	char viewHold[areaSideLength][areaSideLength];

	for (int i = 0; i < areaSideLength; i++) // copy the visible area of a maze to the view
		for (int j = 0; j < areaSideLength; j++)
		{
			if ((upLeftCornerX + j) >= 0 && (upLeftCornerY + i) >= 0 && (upLeftCornerX + j) < mazeWidth && (upLeftCornerY + i) < mazeHight)
				view[i][j] = map[upLeftCornerY + i][upLeftCornerX + j]; // if the part of a visible area within maze: copy maze cell
			else view[i][j] = ' '; // else: set the space
		}

	for (int k = 0; k < directionOfMove; k++) // rotate the view 90 degrees clockwise so much times as move direction enumeration value is. So a runner is fixed and a visible area rotates
	{
		for (int i = 0; i < areaSideLength; i++) // duplicate the view
			for (int j = 0; j < areaSideLength; j++)
			{
				viewHold[i][j] = view[i][j]; 
			}
		
		for (int i = -1; i <= 1; i++) // rotate the view
			for (int j = -1; j <= 1; j++)
			{
				view[centerOfView + i][centerOfView + j] = viewHold[centerOfView + i*COS90 - j*SIN90][centerOfView + i*SIN90 + j*COS90];
			}
	}
	view[centerOfView][centerOfView] = '^'; // put the runner in the center
	for (int i = 0; i < areaSideLength; i++) // display the view
	{
		for (int j = 0; j < areaSideLength; j++)
		{
			std::cout << ' ' << view[i][j];
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
}