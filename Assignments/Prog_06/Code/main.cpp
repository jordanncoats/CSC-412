//
//  main.cpp
//  GL threads
//
//  Created by Jean-Yves Hervé on 2017-04-24, revised 2019-11-19
//

/*-------------------------------------------------------------------------+
 |	A graphic front end for a grid+state simulation.						|
 |																			|
 |	This application simply creates a glut window with a pane to display	|
 |	a colored grid and the other to display some state information.			|
 |	Sets up callback functions to handle menu, mouse and keyboard events.	|
 |	Normally, you shouldn't have to touch anything in this code, unless		|
 |	you want to change some of the things displayed, add menus, etc.		|
 |	Only mess with this after everything else works and making a backup		|
 |	copy of your project.  OpenGL & glut are tricky and it's really easy	|
 |	to break everything with a single line of code.							|
 |																			|
 |	Current GUI:															|
 |		- 'ESC' --> exit the application									|
 |		- 'r' --> add red ink												|
 |		- 'g' --> add green ink												|
 |		- 'b' --> add blue ink												|
 +-------------------------------------------------------------------------*/

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <string>
#include <vector>
//
#include "gl_frontEnd.h"

#if 0
#pragma mark -
#pragma mark Function prototypes
#endif

//==================================================================================
//	Function prototypes
//==================================================================================
/** These functions were created by Prof Herve
 */
void displayGridPane(void);
void displayStatePane(void);
void initializeApplication(void);

/** Function for each thread to call to start the travelers
  *  movement.
  *  @param  data A pointer to the data from a Traveler.
  *  @return  void.
  */
void *moveTraveler(void *data);

#if 0
#pragma mark -
#pragma mark Global variables
#endif

//==================================================================================
//	Application-level global variables
//==================================================================================

//	Don't touch
extern int GRID_PANE, STATE_PANE;
extern int gMainWindow, gSubwindow[2];

//	The state grid and its dimensions
int **grid;
const int NUM_ROWS = 20, NUM_COLS = 20;

//	the number of live threads (that haven't terminated yet)
int MAX_NUM_TRAVELER_THREADS = 8;
int numLiveThreads = 0;

//	the ink levels
int MAX_LEVEL = 50;
int MAX_ADD_INK = 10;
int redLevel = 20, greenLevel = 10, blueLevel = 40;

//	ink producer sleep time (in microseconds)
const int MIN_SLEEP_TIME = 1000;
int producerSleepTime = 100000;

//	Enable this declaration if you want to do the traveler information
//	maintaining extra credit section
TravelerInfo *travelList;

typedef struct ThreadInfo
{
	pthread_t threadID;
	unsigned int index;
} ThreadInfo;

ThreadInfo *threads;

//==================================================================================
//	These are the functions that tie the simulation with the rendering.
//	Some parts are "don't touch."  Other parts need your intervention
//	to make sure that access to critical section is properly synchronized
//==================================================================================

void displayGridPane(void)
{
	//	This is OpenGL/glut magic.  Don't touch
	glutSetWindow(gSubwindow[GRID_PANE]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//---------------------------------------------------------
	//	This is the call that makes OpenGL render the grid.
	//
	//	You *must* synchronize this call.
	//
	//---------------------------------------------------------
	pthread_mutex_lock(&scene->state_mutex);
	drawGrid(grid, NUM_ROWS, NUM_COLS);
	pthread_mutex_unlock(&scene->state_mutex);
	//
	//	Use this drawing call instead if you do the extra credits for
	//	maintaining traveler information
	drawGridAndTravelers(grid, NUM_ROWS, NUM_COLS, travelList);

	//	This is OpenGL/glut magic.  Don't touch
	glutSwapBuffers();

	glutSetWindow(gMainWindow);
}

void displayStatePane(void)
{
	//	This is OpenGL/glut magic.  Don't touch
	glutSetWindow(gSubwindow[STATE_PANE]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//---------------------------------------------------------
	//	This is the call that makes OpenGL render information
	//	about the state of the simulation.
	//
	//	You *must* synchronize this call.
	//
	//---------------------------------------------------------
	drawState(numLiveThreads, redLevel, greenLevel, blueLevel);

	//	This is OpenGL/glut magic.  Don't touch
	glutSwapBuffers();

	glutSetWindow(gMainWindow);
}

//------------------------------------------------------------------------
//	These are the functions that would be called by a traveler thread in
//	order to acquire red/green/blue ink to trace its trail.
//	You *must* synchronized access to the ink levels
//------------------------------------------------------------------------
//
int acquireRedInk(int theRed)
{
	int ok = 0;
	if (redLevel >= theRed)
	{
		redLevel -= theRed;
		ok = 1;
	}
	return ok;
}

int acquireGreenInk(int theGreen)
{
	int ok = 0;
	if (greenLevel >= theGreen)
	{
		greenLevel -= theGreen;
		ok = 1;
	}
	return ok;
}

int acquireBlueInk(int theBlue)
{
	int ok = 0;
	if (blueLevel >= theBlue)
	{
		blueLevel -= theBlue;
		ok = 1;
	}
	return ok;
}

//------------------------------------------------------------------------
//	These are the functions that would be called by a producer thread in
//	order to refill the red/green/blue ink tanks.
//	You *must* synchronized access to the ink levels
//------------------------------------------------------------------------
//
int refillRedInk(int theRed)
{
	int ok = 0;
	if (redLevel + theRed <= MAX_LEVEL)
	{
		redLevel += theRed;
		ok = 1;
	}
	return ok;
}

int refillGreenInk(int theGreen)
{
	int ok = 0;
	if (greenLevel + theGreen <= MAX_LEVEL)
	{
		greenLevel += theGreen;
		ok = 1;
	}
	return ok;
}

int refillBlueInk(int theBlue)
{
	int ok = 0;
	if (blueLevel + theBlue <= MAX_LEVEL)
	{
		blueLevel += theBlue;
		ok = 1;
	}
	return ok;
}

//------------------------------------------------------------------------
//	You shouldn't have to touch this one.  Definitely if you don't
//	add the "producer" threads, and probably not even if you do.
//------------------------------------------------------------------------
void speedupProducers(void)
{
	//	decrease sleep time by 20%, but don't get too small
	int newSleepTime = (8 * producerSleepTime) / 10;

	if (newSleepTime > MIN_SLEEP_TIME)
	{
		producerSleepTime = newSleepTime;
	}
}

void slowdownProducers(void)
{
	//	increase sleep time by 20%
	producerSleepTime = (12 * producerSleepTime) / 10;
}

#if 0
#pragma mark -
#pragma mark Function implementations
#endif

//--------------------------------------------------------------
//  Main function
//
//--------------------------------------------------------------
int main(int argc, char **argv)
{
	initializeFrontEnd(argc, argv, displayGridPane, displayStatePane);

	int error_code = pthread_mutex_init(&scene->state_mutex, nullptr);
	if (error_code < 0)
	{
		printf("Error failed to create mutex.\n");
	}

	//	Now we can do application-level
	initializeApplication();

	//	Now we enter the main loop of the program and to a large extend
	//	"lose control" over its execution.  The callback functions that
	//	we set up earlier will be called when the corresponding event
	//	occurs
	glutMainLoop();

	//	Free allocated resource before leaving (not absolutely needed, but
	//	just nicer.  Also, if you crash there, you know something is wrong
	//	in your code.
	for (int i = 0; i < NUM_ROWS; i++)
		free(grid[i]);
	free(grid);
	//
	free(travelList);

	//	This will never be executed (the exit point will be in one of the
	//	call back functions).
	return 0;
}

// ---------------------------------------------------------------------
//  Function : initializeApplication
//  Description :
//
//  A utility function to create the travelers that will be displayed
//  on screen and passed to the moveTravelers function via multiple
//  threads.
//
//----------------------------------------------------------------------

void initializeApplication(void)
{
	//	Allocate the grid
	grid = (int **)malloc(NUM_ROWS * sizeof(int *));
	for (int i = 0; i < NUM_ROWS; i++)
		grid[i] = (int *)malloc(NUM_COLS * sizeof(int));

	//---------------------------------------------------------------
	//	All the code below to be replaced/removed
	//	I initialize the grid's pixels to have something to look at
	//---------------------------------------------------------------
	//	Yes, I am using the C random generator after ranting in class that the C random
	//	generator was junk.  Here I am not using it to produce "serious" data (as in a
	//	simulation), only some color, in meant-to-be-thrown-away code

	//	seed the pseudo-random generator
	//srand((unsigned int)time(NULL));
	//const unsigned char minVal = (unsigned char)40;
	// const unsigned char range = (unsigned char)(255 - minVal);

	//	create RGB values (and alpha  = 255) for each pixel
	for (int i = 0; i < NUM_ROWS; i++)
	{
		for (int j = 0; j < NUM_COLS; j++)
		{
			//	temp code to get some color initially
			// unsigned char red = (unsigned char)(rand() % range + minVal);
			// unsigned char green = (unsigned char)(rand() % range + minVal);
			// unsigned char blue = (unsigned char)(rand() % range + minVal);
			// grid[i][j] = 0xFF000000 | (blue << 16) | (green << 8) | red;

			//	the intialization you should use
			grid[i][j] = 0xFF000000;
		}
	}

	travelList = (TravelerInfo *)malloc(MAX_NUM_TRAVELER_THREADS * sizeof(TravelerInfo));

	for (int k = 0; k < MAX_NUM_TRAVELER_THREADS; k++)
	{

		travelList[k].type = (TravelerType)(rand() % NUM_TRAV_TYPES);
		travelList[k].row = rand() % NUM_ROWS;
		travelList[k].col = rand() % NUM_COLS;
		travelList[k].dir = (TravelDirection)(rand() % NUM_TRAVEL_DIRECTIONS);
		travelList[k].isLive = 1;
		pthread_create(&travelList[k].threadID, NULL, moveTraveler, &travelList[k]);
		numLiveThreads++;
	}
}

// ---------------------------------------------------------------------
//  Function : moveTravler
//  Description :
//
//  This function takes data of a traveler, determines what color it should
//  create a trail of then randomly decides which direction to move out of
//  the two perpendicular choices.
//
//----------------------------------------------------------------------

void *moveTraveler(void *data)
{
	TravelerInfo *t = (TravelerInfo *)data;
	while (t->isLive)
	{
		//The three color possbilities are avalible:
		if (t->type == RED_TRAV)
		{
			if (redLevel > 0)
			{
				//Mask for the color
				int red = grid[t->row][t->col] & 0x0000ff;
				//If the color is full then it doesn't use ink needlessly
				if (red < 255)
				{
					//Increases by 32 to show a noticeable difference
					red += 32;
					//Keeps the color from over flowing
					if (red > 255)
					{
						red = 255;
					}
					//Turns the grid to the color
					grid[t->row][t->col] |= red;
					//Decreases the color level
					redLevel--;
				}
			}
		}
		//As written in the red, each color setting after works in a similar way, I will note differences.
		if (t->type == BLUE_TRAV)
		{
			if (blueLevel > 0)
			{
				int blue = grid[t->row][t->col] & 0xff0000;
				//Shifts it by 16 to only change the blue bits
				blue >>= 16;
				if (blue < 255)
				{
					blue += 32;
					if (blue > 255)
					{
						blue = 255;
					}
					//Shift back, so it won't accidentially effect another color
					blue <<= 16;
					grid[t->row][t->col] |= blue;
					blueLevel--;
				}
			}
		}
		if (t->type == GREEN_TRAV)
		{
			if (greenLevel > 0)
			{
				int green = grid[t->row][t->col] & 0x00ff00;
				//Similar to blue this shifts by 8 to only change the green bits.
				green >>= 8;
				if (green < 255)
				{
					green += 32;
					if (green > 255)
					{
						green = 255;
					}
					//Same as above, shifts back to ensure we don't modifiy other colors
					green <<= 8;
					grid[t->row][t->col] |= green;
					greenLevel--;
				}
			}
		}
		//A vector that will be loaded with the possible directions
		std::vector<TravelDirection> validDirection;
		//To make sure the traveler only moves perpendicular when it needs a new direction
		//Each choice is dependant on which was it already is facing
		if (t->col + 1 <= NUM_COLS - 1)
		{
			if (t->dir != WEST || t->dir != EAST)
			{
				validDirection.push_back(EAST);
			}
		}
		if (t->col - 1 >= 0)
		{
			if (t->dir != WEST || t->dir != EAST)
			{
				validDirection.push_back(WEST);
			}
		}
		if (t->row + 1 <= NUM_ROWS - 1)
		{
			if (t->dir != NORTH || t->dir != SOUTH)
			{
				validDirection.push_back(NORTH);
			}
		}
		if (t->row - 1 >= 0)
		{
			if (t->dir != NORTH || t->dir != SOUTH)
			{
				validDirection.push_back(SOUTH);
			}
		}
		//A random choice from the two avalible directions
		int randomIndex = rand() % validDirection.size();
		t->dir = validDirection[randomIndex];
		//clearing the vector for future use
		validDirection.clear();
		//north
		if (t->dir == NORTH)
		{
			t->row++;
		}
		//west
		if (t->dir == WEST)
		{
			t->col--;
		}
		//south
		if (t->dir == SOUTH)
		{
			t->row--;
		}
		//east
		if (t->dir == EAST)
		{
			t->col++;
		}
		//Terminating when it reaches a corner
		if ((t->col == 0 && t->row == 0) || (t->col == 0 && t->row == NUM_ROWS - 1) || (t->col == NUM_COLS - 1 && t->row == 0) || (t->col == NUM_COLS - 1 && t->row == NUM_ROWS - 1))
		{
			t->isLive = 0;
			numLiveThreads--;
		}
		usleep(62500);
	}
	return NULL;
}
