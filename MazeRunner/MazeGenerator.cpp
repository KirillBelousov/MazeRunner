#include "pch.h"
#include "MazeGenerator.h"
#include "Maze.h"
#include <iostream>
#include <cmath>

MazeGenerator::MazeGenerator()
{
}

void MazeGenerator::run(Maze &inputMaze)
{
	p_maze = &inputMaze;
	p_maze->initialization();
	int startWall = rand() % 4; // choose the wall where the entrance is
	switch (startWall)
	{
	case 0:
		p_maze->setEntranceX(0);  // left wall
		p_maze->setEntranceY(1 + rand() % (p_maze->getHight() - 2)); // a random Y position on the wall
		break;
	case 1:
		p_maze->setEntranceX(1 + rand() % (p_maze->getWidth() - 2)); // a random X position on the wall
		p_maze->setEntranceY(0); // up wall
		break;
	case 2:
		p_maze->setEntranceX(p_maze->getWidth() - 1); // right wall
		p_maze->setEntranceY(1 + rand() % (p_maze->getHight() - 2)); // a random Y position on the wall
		break;
	case 3:
		p_maze->setEntranceX(1 + rand() % (p_maze->getWidth() - 2)); // a random X position on the wall
		p_maze->setEntranceY(p_maze->getHight() - 1); // down wall
		break;
	default:
		break;
	}
	int space = 3; //magic number
	for (int i = 2, j = 2; i < (p_maze->getWidth() - 1) || j < (p_maze->getHight() - 1); i = i + space, j = j + space)
	{
		gnaw(i, 1, -1);
		gnaw(1, (p_maze->getHight() - 1) - j, 1);
		gnaw((p_maze->getWidth() - 1) - i, p_maze->getHight() - 2, -1);
		gnaw(p_maze->getWidth() - 2, j, 1);
	}
	gnaw(p_maze->getEntranceX(), p_maze->getEntranceY(), 0);
}

/*Create one passage within maze changing wall cells into free cells. The start position of the passage is taken as
input arguments 'initialX' and 'initialY'. The passage consist of vertical and horizontal units and can have additional
side branches. There are two types of passages: ordinary ones that can't end on outermost maze coordinates and
entrance-exit passage (EEPassage) that must start and end on the maze boundaries creating entrance and exit of a maze.
The passage has main direction in which the first unit and parallel to it is built. Input argument 'passage Type'
determines if main direction of a passage is horizontal (1), vertical(-1), or if the passage is a EEPassage (0).
Input argument 'main Direction' (by default it's 0) determines if main direction is positive (if even) or negative (if odd).
The direction of an EEPassage is determined during function execution.*/
void MazeGenerator::gnaw(int initialX, int initialY, int passageType, int mainDirection)  // passageType = 1 for horizontal, -1 for vertical, 0 for EEPassage
{
	bool hitAnotherPassage = false; // control if during the gnawing the passage meets another already existing passage
	int currentX = initialX;
	int currentY = initialY;
	int unitEndX = initialX;
	int unitEndY = initialY;
	int unitLength;
	int unitType; // determines if unit is codirectional to main direction (0) or perpendicular to it (1 or -1)
	int deltaX; // how much X coordinate will change 
	int deltaY; // how much X coordinate will change 
	int branchType; //horizontal or vertical
	int unitNumber = 0; // the first unit has 0 (even) number
	p_maze->setCellToFreeSpace(currentX, currentY, DEMO); // set free space at start coordinates 
	// if a start wall is vertical (for an EEPassage) or horizontal direction has been set directly
	if (initialX == 0 || initialX == (p_maze->getWidth() - 1) || passageType == 1)
	{
		// stop if the unit ends on the border of the maze (for an EEPassage) or in front of it (for others) 
		//or hit another existing passage. But continue in any case if it is the first unit
		while ((unitEndX != (0 + abs(passageType)) && unitEndX != (p_maze->getWidth() - 1 - abs(passageType))
			&& unitEndY != (0 + abs(passageType)) && unitEndY != (p_maze->getHight() - 1 - abs(passageType)) && !hitAnotherPassage)
			|| unitEndX == initialX)
		{
			unitLength = defineLength(); // define the length of the unit

	// define the type of the unit: parallel (0) to the first unit (if unitNumber is even) or perpendicular (if unitNumber is odd)
	//+ define randomly direction of perpendicular unit (1 or -1, positive or negative direction) 
			unitType = defineType(unitNumber);

			if (unitType == 0) // if the unit is parallel to the first unit, i.e. horizontal in this case
			{
				branchType = -1; // a possible branch will be vertical
			}
			else branchType = 1; //else the unit is vertical and a possible branch will be horizontal
			deltaX = unitLength * (1 - abs(unitType)) * ((((initialX != 0) + (initialX != (p_maze->getWidth() - 1))) / 2 + (initialX == 0) - (initialX == (p_maze->getWidth() - 1)))) * ((1 - mainDirection % 2) - mainDirection % 2);
			//if unitType 0 then ghaw unit      // if start at right maze boundary then -1 and coordinate decreases                                          // if mainDirection is even then 1 and coordinate increases
			//along X axis                      // if at left then 1 and increases                                                                           // if odd then -1 and decreases
												// if not at a boundary then also 1
			deltaY = unitLength * unitType; // if the unit is perpendicular to main direction (unitType is 1 or -1) ghaw along Y axis
			unitEndX = currentX + deltaX; // calculate the end of the unit X coordinate
			unitEndY = currentY + deltaY; // calculate the end of the unit Y coordinate

			// if the end is within the maze (for an EEPassage) or before outermost coordinates (for others)
			//and not at the begin of the passage
			if ((unitEndX >= (0 + abs(passageType)) && unitEndX <= (p_maze->getWidth() - 1 - abs(passageType))
				&& unitEndY >= (0 + abs(passageType)) && unitEndY <= (p_maze->getHight() - 1 - abs(passageType)))
				&& unitEndX != initialX)
			{
				// until all the length is gone and the unit doesn't hit another passage   
				for (int i = 1; i <= unitLength && !hitAnotherPassage; i++)
				{
					//change coordinates one by one
					currentY += unitType;
					currentX += (1 - abs(unitType)) * ((((initialX != 0) + (initialX != (p_maze->getWidth() - 1))) / 2 + (initialX == 0) - (initialX == (p_maze->getWidth() - 1)))) * ((1 - mainDirection % 2) - mainDirection % 2);
					p_maze->setCellToFreeSpace(currentX, currentY, DEMO); // set free space
					//display();
					//check if the unit meets the free space of another passage, but if the unit belongs to EEPassage then don't care about it
					if ((p_maze->getCellType(currentX + unitType, currentY + (1 - abs(unitType))) == '.'
						|| p_maze->getCellType(currentX - unitType, currentY - (1 - abs(unitType))) == '.')
						&& abs(passageType))
					{
						hitAnotherPassage = true; // note that the unit hit another passage
					}
					else //if there is no another passage or an EEPassage is built
					{
						// if it's not the end of the unit randomly decide to create additional branch 
						if (rand() % 2 == 0 && currentX != unitEndX && currentY != unitEndY)
						{
							int branchDirection = rand() % 2;
							gnaw(currentX, currentY, branchType, branchDirection);
						}
					}
				}
				unitNumber++; // increase unit number
			}
			else // gnaw in opposite direction
			{
				unitEndX -= unitLength * (1 - abs(unitType)) * ((((initialX != 0) + (initialX != (p_maze->getWidth() - 1))) / 2 + (initialX == 0) - (initialX == (p_maze->getWidth() - 1)))) * ((1 - mainDirection % 2) - mainDirection % 2);
				unitEndY -= unitLength * unitType;
				// if the passage is not an EEPassage change main direction to the opposite one
				mainDirection = mainDirection + abs(passageType);
			}
		}
	}
	else
	{
		// similar stuff for horizontal start walls (for an EEPassage) and direct vertical direction
		if (initialY == 0 || initialY == (p_maze->getHight() - 1) || passageType == -1)
		{
			while ((unitEndX != (0 + abs(passageType)) && unitEndX != (p_maze->getWidth() - 1 - abs(passageType)) && unitEndY != (0 + abs(passageType)) && unitEndY != (p_maze->getHight() - 1 - abs(passageType)))
				&& !hitAnotherPassage || unitEndY == initialY)
			{
				unitLength = defineLength();
				unitType = defineType(unitNumber);
				if (unitType == 0)
				{
					branchType = 1;
				}
				else branchType = -1;
				unitEndY = currentY + unitLength * (1 - abs(unitType)) * ((((initialY != 0) + (initialY != (p_maze->getHight() - 1))) / 2 + (initialY == 0) - (initialY == (p_maze->getHight() - 1)))) * ((1 - mainDirection % 2) - mainDirection % 2);
				unitEndX = currentX + unitLength * unitType;
				if ((unitEndX >= (0 + abs(passageType)) && unitEndX <= (p_maze->getWidth() - 1 - abs(passageType)) && unitEndY >= (0 + abs(passageType)) && unitEndY <= (p_maze->getHight() - 1 - abs(passageType))) && unitEndY != initialY)
				{
					for (int i = 1; i <= unitLength && !hitAnotherPassage; i++)
					{
						currentY += (1 - abs(unitType)) * ((((initialY != 0) + (initialY != (p_maze->getHight() - 1))) / 2 + (initialY == 0) - (initialY == (p_maze->getHight() - 1)))) * ((1 - mainDirection % 2) - mainDirection % 2);
						currentX += unitType;
						p_maze->setCellToFreeSpace(currentX, currentY, DEMO);
						if (abs(passageType) && (p_maze->getCellType(currentX + (1 - abs(unitType)), currentY + unitType) == '.' || p_maze->getCellType(currentX - (1 - abs(unitType)), currentY - unitType) == '.'))
						{
							hitAnotherPassage = true;
						}
						else
						{
							if (rand() % 2 == 0 && currentX != unitEndX && currentY != unitEndY)
							{
								int branchDirection = rand() % 2;
								gnaw(currentX, currentY, branchType, branchDirection);
							}
						}
					}
					unitNumber++;
				}
				else
				{
					unitEndY -= unitLength * (1 - abs(unitType)) *((((initialY != 0) + (initialY != (p_maze->getHight() - 1))) / 2 + (initialY == 0) - (initialY == (p_maze->getHight() - 1)))) * ((1 - mainDirection % 2) - mainDirection % 2);
					unitEndX -= unitLength * unitType;
					mainDirection = mainDirection + abs(passageType);
				}
			}
		}
	}
	if (passageType == 0) // if it's an EEPassage save the end coordinates of the last unit as an exit coordinates
	{
		p_maze->setExitX(unitEndX);
		p_maze->setExitY(unitEndY);
	}
}

int MazeGenerator::defineLength()
{
	int dice = rand() % 12;
	switch (dice)
	{
	case 0:
		return 1;
	case 1:
	case 2:
		return 2;
	case 3:
	case 4:
	case 5:
		return 3;
	case 6:
	case 7:
	case 8:
		return 4;
	case 9:
	case 10:
		return 5;
	case 11:
		return 6;
	default:
		std::cout << "Random value is out of expected range during defineLength() execution. Default value 3 is returned" << std::endl;
		return 3;
	}
	// average 4.5
}

int MazeGenerator::defineType(int counter)
{
	if (counter % 2) // if odd, the unit is perpendicular to the first unit
	{
		int dice = rand() % 2;
		switch (dice)
		{
		case 0:
			return -1; // and has negative direction
		case 1:
			return 1; // and has positive direction
		default:
			std::cout << "Random value is out of expected range during defineLength() execution. Default value 1 is returned" << std::endl;
			return 1;
		}
	}
	else return 0; // if even, the unit is parallel to the first unit
}