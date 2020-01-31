#ifndef ROBOTS_H
#define ROBOTS_H

#include <vector>
#include <pthread.h>

extern int numLiveThreads;

/** To handle the directional choices */

typedef enum Direction
{
  NORTH = 0,
  WEST,
  SOUTH,
  EAST,
  NS,
  SN,
  STOP,
} Direction;

/**  A Struct used to create the doors. */
struct Door
{

  /** Variables needed for each door */
  int door_id;
  int door_Col;
  int door_Row;
};

/** A Struct used to create the Robots and Boxes. */
struct RobotAndBox
{

  /** Variables needed for the Rob/box */
  bool isLive;
  int robot_id;
  int robot_Col;
  int robot_Row;
  int box_Col;
  int box_Row;
  bool rotation;

  /** Data for the direction the robot is going in and how far it is from the box */
  Direction robotDir;
  int rowDistanceFromBox;
  int colDistanceFromBox;

  /** Likewise for the door but direction is handled in push */
  Door *assigned_door;
  int rowDistanceFromDoor;
  int colDistanceFromDoor;
};

/** This struct holds all the information about what is currently happening in the grid along with a list of robots and doors: */
struct RobotScene
{

  /** Vector lists for Rob/Box and Doors */
  std::vector<RobotAndBox *> robot_list;
  std::vector<Door *> door_list;

  /** Mutext to lock various parts of the progam, they're here because scene is shared everywhere */
  pthread_mutex_t vector_mutex;
  pthread_mutex_t **grid_mutex;
  pthread_mutex_t output_mutex;
  pthread_mutex_t state_mutex;

  /** Variables to create the grid from */
  int **grid;
  int numRows;
  int numCols;
  int numDoors;
  int numBoxes;
};
/** This struct is the thread struct so it points to a robot and the scene. */
struct RobotThread
{

  /** Thread Indexes */
  pthread_t id;
  int thread_index;

  /** Pointers to the scene so data is shared about the surroundings */
  RobotScene *robot_scene;

  /** A Pointer to the Rob/Box combo this thread is controlling */
  RobotAndBox *owner;
};

//==================================================================================
//	Function prototypes
//==================================================================================
/** A function to encapsulate and manipulate the Rob/Box thread
  *  @param data the struct to be worked with.
  *  @return  N/A
  */
void *robot_thread(void *data);
/** A function that sets the path to the box, with the direction it
  * can call move for robot to move to the box and then when 
  * there it calls push
  *  @param rt the thread that is being manipulated
  *  @return  N/A
  */
void path(RobotThread *rt);
/** A function to move the robot, in V3 it checks the mutex on the grid
  * if it is open then it moves to the space.
  *  @param rt the thread that is being manipulated
  *  @return  N/A
  */
void move(RobotThread *rt);
/** A function to push the box, in V3 it checks the mutex on the grid
  * if it is open then it moves the box to the space then the robot.
  *  @param rt the thread that is being manipulated
  *  @return  N/A
  */
void push(RobotThread *rt);
/** A function that terminiates once the box is in the door.
  *  @param rt the thread that is being manipulated
  *  @return  N/A
  */
void end(RobotThread *rt);

#endif
