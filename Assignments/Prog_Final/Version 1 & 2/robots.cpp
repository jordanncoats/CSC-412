#include "robots.h"
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
FILE *output;

// ---------------------------------------------------------------------
//  Function : robot_thread
//  Description :
//
//  This function handles the robot thread,
//  it opens a file and while it is active it calls
//  the path function then sleeps
//  at the end it will close the file it has open.
//
//----------------------------------------------------------------------

void *robot_thread(void *data)
{
    /** Open the text file to write to */
    output = fopen("robotSimulOut.txt", "a");
    if (output == NULL)
    {
        printf("Failed to write to robotSimulOut.txt.\n");
    }
    /** Load the data */
    RobotThread *rt = (RobotThread *)data;
    /** While the thread is alive it will keep moving */
    while (rt->owner->isLive)
    {
        path(rt);
        usleep(625000);
    }

    return nullptr;
}

// ---------------------------------------------------------------------
//  Function : path
//  Description :
//
//  Path is my attempt at robot logic to move around the grid.
//  In theory it just takes the distance and direction and calls
//  move until something the distance is 0.
//
//  It always goes N/S then W/E.
//
//----------------------------------------------------------------------

void path(RobotThread *rt)
{

    if (rt->owner->rowDistanceFromBox != 0)
    {
        /** Through out this function it uses a basic 
         * direction of the box to determine the
         *  movement do before calling move.
         * */
        if (rt->owner->rowDistanceFromBox < 0)
        {
            rt->owner->robotDir = NORTH;
            move(rt);
            rt->owner->rowDistanceFromBox++;
        }
        if (rt->owner->rowDistanceFromBox > 0)
        {
            rt->owner->robotDir = SOUTH;
            move(rt);
            rt->owner->rowDistanceFromBox--;
        }
    }

    else if (rt->owner->colDistanceFromBox != 0)
    {
        if (rt->owner->colDistanceFromBox < 0)
        {
            rt->owner->robotDir = WEST;
            move(rt);
            rt->owner->colDistanceFromBox++;
        }
        if (rt->owner->colDistanceFromBox > 0)
        {
            rt->owner->robotDir = EAST;
            move(rt);
            rt->owner->colDistanceFromBox--;
        }
    }
    /** When it reaches the box it then calls push which will handle everything else */
    else if (rt->owner->rowDistanceFromBox == 0)
    {
        rt->owner->robotDir = STOP;

        push(rt);
    }
}
// ---------------------------------------------------------------------
//  Function : move
//  Description :
//
//  This is a simple function that just checks if the block is unlocked
//  then moves itself into that block.
//
//----------------------------------------------------------------------
void move(RobotThread *rt)
{
    switch (rt->owner->robotDir)
    {
    case NORTH:

        rt->owner->robot_Row--;
        pthread_mutex_lock(&rt->robot_scene->output_mutex);
        fprintf(output, "Robot %d moves North.\n", rt->owner->robot_id);
        pthread_mutex_unlock(&rt->robot_scene->output_mutex);
        usleep(100000);
        break;

    case SOUTH:

        rt->owner->robot_Row++;
        pthread_mutex_lock(&rt->robot_scene->output_mutex);
        fprintf(output, "Robot %d moves South.\n", rt->owner->robot_id);
        pthread_mutex_unlock(&rt->robot_scene->output_mutex);
        usleep(100000);
        break;
    case WEST:

        rt->owner->robot_Col--;
        pthread_mutex_lock(&rt->robot_scene->output_mutex);
        fprintf(output, "Robot %d moves West.\n", rt->owner->robot_id);
        pthread_mutex_unlock(&rt->robot_scene->output_mutex);
        usleep(100000);
        break;
    case EAST:
        rt->owner->robot_Col++;
        pthread_mutex_lock(&rt->robot_scene->output_mutex);
        fprintf(output, "Robot %d moves East.\n", rt->owner->robot_id);
        pthread_mutex_unlock(&rt->robot_scene->output_mutex);
        usleep(100000);
        break;
    case NS:
        /** North to South */
        rt->owner->robotDir = WEST;
        move(rt);

        rt->owner->robotDir = SOUTH;
        move(rt);

        rt->owner->robotDir = SOUTH;
        move(rt);

        rt->owner->robotDir = EAST;
        move(rt);
        break;
    case SN:
        /** South to North */
        rt->owner->robotDir = WEST;
        move(rt);

        rt->owner->robotDir = NORTH;
        move(rt);

        rt->owner->robotDir = NORTH;
        move(rt);

        rt->owner->robotDir = EAST;
        move(rt);
        break;
    case STOP:
        break;
    default:
        std::cout << "Movement Error" << std::endl;
        break;
    }
}
// ---------------------------------------------------------------------
//  Function : push
//  Description :
//
//  Push is almost a combination of move and path.
//  The path part comes from when it needs to rotate itself around a box.
//  The actual pushing is just the move function with the added box movement.
//
//----------------------------------------------------------------------
void push(RobotThread *rt)
{
    /** This handles when they're in the same col and need to go around */
    if (rt->owner->rotation && rt->owner->robot_Col == rt->owner->assigned_door->door_Col)
    {
        if (rt->owner->box_Row < rt->owner->robot_Row && rt->owner->box_Row < rt->owner->assigned_door->door_Row)
        {
            rt->owner->robotDir = SN;
            rt->owner->rotation = false;
            move(rt);
        }
        if (rt->owner->box_Row > rt->owner->robot_Row && rt->owner->box_Row > rt->owner->assigned_door->door_Row)
        {
            rt->owner->robotDir = NS;
            rt->owner->rotation = false;
            move(rt);
        }
    }
    if (rt->owner->rowDistanceFromDoor != 0)
    {
        /** I'm only commenting the first instance of code as these blocks are all the same except for the location */
        if (rt->owner->rowDistanceFromDoor < 0)
        {
            /** North */

            /** Move Box */

            rt->owner->box_Row--;
            /** Move Robot */
            rt->owner->robot_Row--;
            /** Write to File */
            pthread_mutex_lock(&rt->robot_scene->output_mutex);
            fprintf(output, "Robot %d pushes North.\n", rt->owner->robot_id);
            pthread_mutex_unlock(&rt->robot_scene->output_mutex);
            /** Increment the distance closer to zero */
            rt->owner->rowDistanceFromDoor++;
        }
        if (rt->owner->rowDistanceFromDoor > 0)
        {
            /** South */

            rt->owner->box_Row++;
            rt->owner->robot_Row++;

            pthread_mutex_lock(&rt->robot_scene->output_mutex);
            fprintf(output, "Robot %d pushes South.\n", rt->owner->robot_id);
            pthread_mutex_unlock(&rt->robot_scene->output_mutex);

            rt->owner->rowDistanceFromDoor--;
        }
    }

    if (rt->owner->rowDistanceFromDoor == 0 && rt->owner->colDistanceFromDoor != 0)
    {
        /** This is the rotation, it only works from W/E because the movement handles N/S */
        if (rt->owner->colDistanceFromDoor > 0 && rt->owner->box_Row < rt->owner->robot_Row)
        {
            /** East, so West & South/North
             * Box above Robot door is East */
            rt->owner->robotDir = WEST;
            move(rt);

            rt->owner->robotDir = NORTH;
            move(rt);
        }
        if (rt->owner->colDistanceFromDoor > 0 && rt->owner->box_Row > rt->owner->robot_Row)
        {
            /** East, so West & South/North 
             * Box below Robot door is East */

            rt->owner->robotDir = WEST;
            move(rt);

            rt->owner->robotDir = SOUTH;
            move(rt);
        }

        if (rt->owner->colDistanceFromDoor < 0 && rt->owner->box_Row < rt->owner->robot_Row)
        {
            /** Westward, so East & South/North 
             * Box above Robot door is West */
            rt->owner->robotDir = EAST;
            move(rt);

            rt->owner->robotDir = NORTH;
            move(rt);
        }
        if (rt->owner->colDistanceFromDoor < 0 && rt->owner->box_Row > rt->owner->robot_Row)
        {
            /** Westward, so East & South/North 
             * Box below Robot door is West */
            rt->owner->robotDir = EAST;
            move(rt);

            rt->owner->robotDir = SOUTH;
            move(rt);
        }

        else if (rt->owner->colDistanceFromDoor > 0)
        {
            /** East */

            rt->owner->box_Col++;

            rt->owner->robot_Col++;

            pthread_mutex_lock(&rt->robot_scene->output_mutex);
            fprintf(output, "Robot %d pushes East.\n", rt->owner->robot_id);
            pthread_mutex_unlock(&rt->robot_scene->output_mutex);

            rt->owner->colDistanceFromDoor--;
        }
        else if (rt->owner->colDistanceFromDoor < 0)
        {
            /** West */

            rt->owner->box_Col--;

            rt->owner->robot_Col--;

            pthread_mutex_lock(&rt->robot_scene->output_mutex);
            fprintf(output, "Robot %d pushes West.\n", rt->owner->robot_id);
            pthread_mutex_unlock(&rt->robot_scene->output_mutex);

            rt->owner->colDistanceFromDoor++;
        }
    }
    if ((rt->owner->box_Col == rt->owner->assigned_door->door_Col) && (rt->owner->box_Row == rt->owner->assigned_door->door_Row))
    {
        /** Once the box is in the door calls end */
        end(rt);
    }
}
// ---------------------------------------------------------------------
//  Function : end
//  Description :
//
//  When the box is in the same spot as the door it will terminiate
//  the thread and release the locks on that grid space.
//
//----------------------------------------------------------------------
void end(RobotThread *rt)
{

    /** Write to File */
    pthread_mutex_lock(&rt->robot_scene->output_mutex);
    fprintf(output, "Robot %d terminates.\n", rt->owner->robot_id);
    pthread_mutex_unlock(&rt->robot_scene->output_mutex);
    /** Decreases the message */
    pthread_mutex_lock(&rt->robot_scene->state_mutex);
    numLiveThreads--;
    pthread_mutex_unlock(&rt->robot_scene->state_mutex);
    /**Terminate */
    rt->owner->isLive = 0;
}
