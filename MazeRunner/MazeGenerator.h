#pragma once

class Maze;

class MazeGenerator
{
public:
	MazeGenerator();
	void run(Maze &maze); // generate a maze
private:
	Maze* p_maze;
	void gnaw(int initialX, int initialY, int passageType, int mainDirection = 0); // help generator to gnaw passage through a maze
	int defineLength(); // help gnaw() to choose the length of a passage unit
	int defineType(int); // help gnaw() to choose the type of a passage unit
	//void fill(int density);

	static const bool DEMO = false;
};

