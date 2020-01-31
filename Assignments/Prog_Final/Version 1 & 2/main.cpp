/********************************************/ /**
 *  Jordan Coats
 *  main.cpp
 *  Final Project CSC 412
 * 
 *  Compile With: g++ -Wall main.cpp robots.cpp gl_frontEnd.cpp -lm -lGL -lglut -lpthread -o robots
 *  Input Example: ./robots 15 15 5 3
 * 
 *  This is version 1 and 2 together, the honest truth is I never made a version 1. My 
 *  first program was multithreaded since day one. I used this version to work out mostly
 *  the logic and writing to the output file. The robots.h file does make mention of grid mutex
 *  locks for Version 3 but that was because I was planning for the future.
 * 
 *  I recieved help from Chris throughout this version and as such there are some 
 *  modifications to gl_frontEnd that I would not normally had made. I do feel like these
 *  changes made some parts easier and others a big harder.
 * 
 ***********************************************/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <random>
//
#include "gl_frontEnd.h"
#include "robots.h"
using namespace std;

#if 0
#pragma mark -
#pragma mark Function prototypes
#endif
//==================================================================================
//	Function prototypes
//==================================================================================
/** A function that is mostly OpenGL/glut magic but with modifications done
  * to make it use my scene data and to display the robots/doors.
  *  @return  N/A
  */
void displayGridPane(void);
/** A function that is mostly OpenGL/glut magic but with modifications done
  * to sync with a mutex
  *  @return  N/A
  */
void displayStatePane(void);
/** A function to clean the grid
  *  @return  N/A
  */
void cleanupGridAndLists(void);
/** A function to initialize the application
  *  @param scene the scene is where it pulls data from.
  *  @return  N/A
  */
void initializeApplication(RobotScene *scene);
/** A function to randomly generate a number
  *  @param padding a way to move the doors away from the edges
  *  @param limit the max number for the random generation
  *  @return  A random number
  */
int randomNoGenerator(int padding, int limit);

#if 0
#pragma mark -
#pragma mark Global Variables
#endif

//==================================================================================
//	Application-level global variables
//==================================================================================

//	Don't touch
extern int GRID_PANE, STATE_PANE;
extern int GRID_PANE_WIDTH, GRID_PANE_HEIGHT;
extern int gMainWindow, gSubwindow[2];

//-------------------------------------
//// Don't rename any of these variables
//-------------------------------------

//	Removed most of the Globals as Chris suggested:

// int **grid;
// int numRows = -1;  //	height of the grid
// int numCols = -1;  //	width
int numBoxes = -1;      //	also the number of robots
int numDoors = -1;      //	The number of doors.
int numLiveThreads = 0; //	the number of live robot threads
RobotScene *scene = nullptr;

//	robot sleep time between moves (in microseconds)
const int MIN_SLEEP_TIME = 1000;
int robotSleepTime = 100000;

//	An array of C-string where you can store things you want displayed
//	in the state pane to display (for debugging purposes?)
//	Dont change the dimensions as this may break the front end
const int MAX_NUM_MESSAGES = 8;
const int MAX_LENGTH_MESSAGE = 32;
char **message;
time_t startTime;

FILE *outputFile;

#if 0
#pragma mark -
#pragma mark Function implementations
#endif

// ---------------------------------------------------------------------
//  Function : randomNoGenerator
//  Description :
//
//  GeeksforGeeks & Stackoverflow helped to create this.
//  Generates a random number.
//
//----------------------------------------------------------------------
int randomNoGenerator(int padding, int limit)
{
    /** I decided to try a random_engine after reading about Rand()'s pontential problems */
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(padding, limit);
    int rand = uniform_dist(e1);
    return rand;
}
// ---------------------------------------------------------------------
//  Function : displayGridPane
//  Description :
//
//  OpenGL/glut magic
//
//----------------------------------------------------------------------
void displayGridPane(void)
{
    //	This is OpenGL/glut magic.  Don't touch
    glutSetWindow(gSubwindow[GRID_PANE]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0, GRID_PANE_HEIGHT, 0);
    glScalef(1.f, -1.f, 1.f);

    //-----------------------------
    //	CHANGE THIS
    //-----------------------------

    int numBoxes = (int)scene->robot_list.size();
    for (int i = 0; i < numBoxes; i++)
    {

        const RobotAndBox *rb = scene->robot_list.at(i);
        /** Test if Robot is alive */
        if (scene->robot_list.at(i)->isLive)

            //						row				column			row			column
            drawRobotAndBox(i, rb->robot_Row, rb->robot_Col, rb->box_Row, rb->box_Col, rb->assigned_door->door_id, scene->numRows, scene->numCols);
    }
    int numDoors = (int)scene->door_list.size();
    for (int i = 0; i < numDoors; i++)
    {
        const Door *door = scene->door_list.at(i);
        drawDoor(door->door_id, door->door_Row, door->door_Col, scene->numRows, scene->numCols);
    }
    //	This call does nothing important. It only draws lines
    //	There is nothing to synchronize here

    drawGrid(scene->numRows, scene->numCols);

    //	This is OpenGL/glut magic.  Don't touch
    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}
// ---------------------------------------------------------------------
//  Function : displayStatePane
//  Description :
//
//  OpenGL/glut magic
//
//----------------------------------------------------------------------
void displayStatePane(void)
{
    //	This is OpenGL/glut magic.  Don't touch
    glutSetWindow(gSubwindow[STATE_PANE]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //	Here I hard-code a few messages that I want to see displayed
    //	in my state pane.  The number of live robot threads will
    //	always get displayed.  No need to pass a message about it.
    time_t currentTime = time(NULL);
    double deltaT = difftime(currentTime, startTime);

    int numMessages = 3;
    sprintf(message[0], "We have %d doors", (int)scene->door_list.size());
    sprintf(message[1], "I like cheese");
    sprintf(message[2], "Run time is %4.0f", deltaT);

    //---------------------------------------------------------
    //	This is the call that makes OpenGL render information
    //	about the state of the simulation.
    //
    //	You *must* synchronize this call.
    //
    //---------------------------------------------------------

    pthread_mutex_lock(&scene->state_mutex);
    drawState(numMessages, message);
    pthread_mutex_unlock(&scene->state_mutex);

    //	This is OpenGL/glut magic.  Don't touch
    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}

// ---------------------------------------------------------------------
//  Function : speedupRobots / slowdownRobots
//  Description :
//
//	You shouldn't have to touch this one.  Definitely if you don't
//	add the "producer" threads, and probably not even if you do.
//
//----------------------------------------------------------------------
void speedupRobots(void)
{
    //	decrease sleep time by 20%, but don't get too small
    int newSleepTime = (8 * robotSleepTime) / 10;

    if (newSleepTime > MIN_SLEEP_TIME)
    {
        robotSleepTime = newSleepTime;
    }
}

void slowdownRobots(void)
{
    //	increase sleep time by 20%
    robotSleepTime = (12 * robotSleepTime) / 10;
}

//--------------------------------------------------------------
//  Main function, expecting as arguments:
//                   row cols boxes doors
//  e.g.    ./robots 15 20 5 2
//          ./robots 10 10 2 3
//--------------------------------------------------------------
int main(int argc, char **argv)
{
    /** Creating teh scene for the whole program */
    scene = new RobotScene();

    scene->numRows = atoi(argv[1]);
    scene->numCols = atoi(argv[2]);
    scene->numBoxes = atoi(argv[3]);
    scene->numDoors = atoi(argv[4]);

    /** Output File: */
    outputFile = fopen("robotSimulOut.txt", "a");
    if (outputFile == NULL)
    {
        printf("ERROR: Failed to create robotSimulOut.txt.\n");
        return 1;
    }

    /** Error handling: */
    if (argc != 5)
    {
        printf("ERROR: Input \n \t Example: Rows, Cols, Robots&Boxes, Doors.\n");
        return 1;
    }
    if (atoi(argv[1]) <= 0 || atoi(argv[2]) <= 0 || atoi(argv[3]) <= 0 || atoi(argv[4]) <= 0)
    {
        printf("ERROR: Input must be positive.\n");
        return 1;
    }
    if (atoi(argv[4]) > 3)
    {
        printf("ERROR: Door must be between 1 - 3.\n");
        return 1;
    }
    if (atoi(argv[3]) > (atoi(argv[1]) * atoi(argv[2])))
    {
        printf("ERROR: Grid too small.\n");
        return 1;
    }
    int error_code = pthread_mutex_init(&scene->state_mutex, nullptr);
    if (error_code < 0)
    {
        printf("Error failed to create mutex.\n");
    }
    /** Writing the base information to the output */
    fprintf(outputFile, "Grid (%d x %d), Boxes: %d, Doors: %d\n", atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
    fclose(outputFile);

    //	Even though we extracted the relevant information from the argument
    //	list, I still need to pass argc and argv to the front-end init
    //	function because that function passes them to glutInit, the required call
    //	to the initialization of the glut library.
    initializeFrontEnd(argc, argv, displayGridPane, displayStatePane);

    /** Taken out of gl_frontEnd because of Externs - Suggested by Chris */
    createDoorColors(scene->numDoors);

    //	Now we can do application-level initialization
    /** Changed under Chris's suggestion to pass variables instead of Externs. */
    initializeApplication(scene);

    //	Now we enter the main loop of the program and to a large extend
    //	"lose control" over its execution.  The callback functions that
    //	we set up earlier will be called when the corresponding event
    //	occurs
    glutMainLoop();

    cleanupGridAndLists();
    return 0;
}
// ---------------------------------------------------------------------
//  Function : cleanupGridAndLists
//  Description :
//
//	Free allocated resource before leaving (not absolutely needed, but
//	just nicer.  Also, if you crash there, you know something is wrong
//	in your code.
//
//----------------------------------------------------------------------
void cleanupGridAndLists(void)
{
    for (int i = 0; i < scene->numRows; i++)
        free(scene->grid[i]);
    free(scene->grid);
    for (int k = 0; k < MAX_NUM_MESSAGES; k++)
        free(message[k]);
    free(message);
}

//==================================================================================
//
//  Function: initializeApplication
//
//  Description:
//  Based on Herve Code
//	Edits made by Jordan Coats with the help of Chris
//
//==================================================================================

void initializeApplication(RobotScene *scene)
{
    /** 	Allocate the grid: */
    scene->grid = (int **)malloc(scene->numRows * sizeof(int *));
    for (int i = 0; i < scene->numRows; i++)
        scene->grid[i] = (int *)malloc(scene->numCols * sizeof(int));
    /**  Zeroes are used to show the grid location is empty: */
    for (int i = 0; i < scene->numRows; i++)
        for (int j = 0; j < scene->numCols; j++)
            scene->grid[i][j] = 0;

    message = (char **)malloc(MAX_NUM_MESSAGES * sizeof(char *));
    for (int k = 0; k < MAX_NUM_MESSAGES; k++)
        message[k] = (char *)malloc((MAX_LENGTH_MESSAGE + 1) * sizeof(char));

    //	seed the pseudo-random generator
    startTime = time(NULL);
    srand((unsigned int)startTime);

    /**  Initilize the locations along with commented out test code... just in case */
    int robotLoc[scene->numBoxes][2]; // = {{7, 5}, {5, 7}};
    int boxLoc[scene->numBoxes][2];   // = {{2, 5}, {5, 2}};
    int doorAssign[scene->numBoxes];  // = {0, 1};
    int doorLoc[scene->numDoors][2];  // = {{8, 5}, {5, 8}};

    /** Here is the entire randomization process: 
     *  This would have been best if it was a function but time
     *  was used for other areas.
    */

    /** Randomly assigned doors */
    for (int i = 0; i < scene->numBoxes; i++)
    {
        doorAssign[i] = randomNoGenerator(0, scene->numDoors - 1);
    }
    /**  Randomly placed Robots*/
    for (int i = 0; i < scene->numBoxes; i++)
    {
        int match = 1;
        while (match)
        {
            robotLoc[i][1] = randomNoGenerator(1, scene->numCols - 2);
            robotLoc[i][0] = randomNoGenerator(1, scene->numRows - 2);
            /** By using the grid we know whether an area is taken or not: */
            if (scene->grid[robotLoc[i][1]][robotLoc[i][0]] == 0)
            {
                scene->grid[robotLoc[i][1]][robotLoc[i][0]] = 1;
                match = 0;
            }
        }
    }
    /** Randomly placed Boxes*/
    for (int i = 0; i < scene->numBoxes; i++)
    {
        int match = 1;
        while (match)
        {
            boxLoc[i][1] = randomNoGenerator(1, scene->numCols - 2);
            boxLoc[i][0] = randomNoGenerator(1, scene->numRows - 2);

            if (scene->grid[boxLoc[i][1]][boxLoc[i][0]] == 0)
            {
                scene->grid[boxLoc[i][1]][boxLoc[i][0]] = 1;
                match = 0;
            }
        }
    }
    /** Randomly place Doors*/
    for (int i = 0; i < scene->numDoors; i++)
    {

        int match = 1;
        while (match)
        {
            doorLoc[i][1] = randomNoGenerator(1, scene->numCols - 1);
            doorLoc[i][0] = randomNoGenerator(1, scene->numRows - 1);

            if (scene->grid[doorLoc[i][1]][doorLoc[i][0]] == 0)
            {
                scene->grid[doorLoc[i][1]][doorLoc[i][0]] = 1;
                match = 0;
            }
        }
    }
    outputFile = fopen("robotSimulOut.txt", "a");
    /** Here we take the random data and apply it to our structs:

     Create all the Doors*/
    for (int i = 0; i < scene->numDoors; i++)
    {
        Door *door = new Door();
        door->door_id = i;
        door->door_Col = doorLoc[i][1];
        door->door_Row = doorLoc[i][0];
        scene->door_list.push_back(door);
        fprintf(outputFile, "Door #%d, (%d, %d) \n", door->door_id, door->door_Col, door->door_Row);
    }
    /** Create all the Robots and Boxes for the Thread*/
    RobotAndBox *rb;
    for (int i = 0; i < scene->numBoxes; i++)
    {
        rb = new RobotAndBox();
        rb->robot_id = i;
        rb->robot_Col = robotLoc[i][1];
        rb->robot_Row = robotLoc[i][0];
        rb->box_Col = boxLoc[i][1];
        rb->box_Row = boxLoc[i][0];
        rb->assigned_door = scene->door_list[doorAssign[i]];
        scene->robot_list.push_back(rb);
        rb->rotation = true;
        rb->robotDir = STOP;
        rb->rowDistanceFromDoor = rb->assigned_door->door_Row - rb->box_Row;
        rb->colDistanceFromDoor = rb->assigned_door->door_Col - rb->box_Col;

        /** This handles direction for the path function and special doors where they're in the same line.*/
        if (rb->rowDistanceFromDoor < 0)
        {
            if (rb->box_Col == rb->robot_Col && rb->box_Row > rb->robot_Row)
            {
                /** North-South Door*/
                rb->rowDistanceFromBox = (rb->box_Row - (rb->robot_Row + 1));
                rb->colDistanceFromBox = rb->box_Col - rb->robot_Col;
            }
            else
            {
                /** North*/
                rb->rowDistanceFromBox = (rb->box_Row - (rb->robot_Row - 1));
                rb->colDistanceFromBox = rb->box_Col - rb->robot_Col;
            }
        }
        else
        {
            if (rb->box_Col == rb->robot_Col && rb->box_Row < rb->robot_Row)
            {
                /** South-North Door*/
                rb->rowDistanceFromBox = (rb->box_Row - (rb->robot_Row - 1));
                rb->colDistanceFromBox = rb->box_Col - rb->robot_Col;
            }
            else
            {
                /** South Door*/
                rb->rowDistanceFromBox = (rb->box_Row - (rb->robot_Row + 1));
                rb->colDistanceFromBox = rb->box_Col - rb->robot_Col;
            }
        }
    }

    for (int i = 0; i < (int)scene->robot_list.size(); i++)
    {
        fprintf(outputFile, "Box #%d, (%d, %d) \n", scene->robot_list.at(i)->robot_id, scene->robot_list.at(i)->box_Col, scene->robot_list.at(i)->box_Row);
    }

    for (int i = 0; i < (int)scene->robot_list.size(); i++)
    {
        fprintf(outputFile, "Robot #%d, (%d, %d) \n", scene->robot_list.at(i)->robot_id, scene->robot_list.at(i)->robot_Col, scene->robot_list.at(i)->robot_Row);
    }
    fclose(outputFile);
    /** create the mutex*/
    int error_output = pthread_mutex_init(&scene->output_mutex, nullptr);
    if (error_output < 0)
    {
        printf("Error failed to create mutex.\n");
    }
    /** start threads */
    RobotThread *rt;
    for (int i = 0; i < scene->numBoxes; i++)
    {
        rt = new RobotThread();
        rt->id = 0;
        rt->thread_index = i;
        numLiveThreads++;
        rt->owner = scene->robot_list.at(i);
        rt->robot_scene = scene;
        rt->owner->isLive = true;
        /** Starting the thread functions */
        int error_code = pthread_create(&rt->id, NULL, robot_thread, (void *)rt);
        if (error_code < 0)
        {
            printf("ERROR: Thread %d failed creation", i);
            rt->owner->isLive = false;
        }
    }
}
