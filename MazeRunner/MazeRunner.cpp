// MazeRunner.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include "Runner.h"
#include "BlindRunner.h"
#include "SightedRunner.h"
#include <iostream>
#include <conio.h>
#include <ctime>
#include <exception>

int main()
{
	srand(time(0));
	MazeGenerator generator;
	Maze *maze = NULL;
	try
	{
		maze = new Maze(generator);
	}
	catch (std::exception const& exception)
	{
		std::cout << exception.what() << "\nThe programm execution was terminated. Press any key to exit." << std::endl;
		_getch();
		return -1;
	}
	
	Runner *p_player = NULL;
	char yourChoice = 0;

	std::cout << "You are infront of the entrance in a dark maze. I hope, you have a light... (Y/N)" << std::endl;

	while (p_player == NULL)
	{
		std::cin >> yourChoice;
		switch (yourChoice)
		{
		case 'n':
		case 'N':
			p_player = new BlindRunner(maze);
			break;
		case 'y':
		case 'Y':
			p_player = new SightedRunner(maze);
			break;
		default:
			std::cout << "I don't understand. Please, press Y or N" << std::endl;
		}
	}

	system("cls");

	p_player->displayWelcomeMessage();
	
	int finishStatus = p_player->getFinishStatus();
	int abortStatus = p_player->getAbortStatus();
	int moveResult = p_player->getInitialStatus();

	while ((moveResult != finishStatus) && (moveResult != abortStatus)) //TODO: predicate functions
	{
		p_player->move(moveResult);
		p_player->displayMoveResult(moveResult);
	}

	std::cout << "\nPress any key to exit" << std::endl;
	_getch();
}

/* TODO: 1. improve mazegenerator
		 2. predicate functions
		 3. add special class for coordinates
		 4. MagicMaze
		 */