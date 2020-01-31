# CSC 412 – Operating Systems

## Final Project, Fall 2019

```
Friday, December 13th 2019
```
Due date:December 20th, 10:00pm (re-submissions accepted until Dec. 23rd, 11:55pm).

# 1 What this Assignment is About

## 1.1 Objectives

In this final project, you will combine together several things that we saw this semester:

- pthreads,
- mutex locks,
- a bit of deadlock management,
- interprocess communication (pipelines).

## 1.2 Handouts

The handout for this assignment is the archive of a C project made up of 2 source files and 1 header
file.

## 1.3 Solo or group project

This project was designed to be done by a group of two students. You may do it as a group of three
but some of the “extra credit” components will be mandatory (more on this later).

# 2 Part I: The Basic Problem

## 2.1 Box pushing

For this problem, you are going to simulate robots that must push boxes on a grid. The robots
will be implemented as threads in your program. The boxes will be... whatever you decide to
implement them as (or not).
In order to push a box, a robot must be standing on a grid square adjacent to the box, and push
the box in the opposite direction. There are therefore only four possible directions for a robot to
push a box. We will call these displacement directionsW, E, N, S (for West, East, North, and


## Robot pushes "W"

## Robot pushes "E"

## Robot pushes "S"

## Robot pushes "N"

```
Figure 1: Directions in which a robot can push a box.
```
South respectively). In Figure 1, the boxes are represented as brown squares while the robots are
represented as light green squares.
In order for a box pushing command to be valid, the square that the robot is pushing the box to
must be free (occupied neither by a robot nor a box). Figure 2 shows example of invalid push
attempts.

## Invalid "N" push:

## Destination square occupied by a robot

## Invalid "E" push:

## Destination square occupied by a box

```
Figure 2: Invalid pushing commands.
```

### 2.2 Task assignment

At the beginning of the simulation, there will be one box assigned to each robot (in other words,
to have to create as many robot threads as there will be boxes in the simulation). In addition,
each robot has a destination “door” for its box. In Figure 3, each robot is shown with a color that
matches that of the door it got assigned. The objective of each robot is to proceed to its box, and
then push the box to its assigned door. When the box has reached the door, it disappears and so
does the robot (the thread terminates).

```
Door 0
```
```
Door 2
```
```
Box 0 Door 1
```
```
Robot 0
```
```
Box 2 Robot 2
```
```
Robot 1
```
```
Box 1
```
```
Box 3
```
```
Robot 3
Door 0
```
```
Door 2
```
```
Box 0 Door 1
```
```
Robot 0
```
```
Box 2 Robot 2
```
```
Robot 1
```
```
Box 1
```
```
Box 3
```
```
Robot 3
```
```
Figure 3: Robots, boxes, and doors assigned.
```
### 2.3 Programming language for the robots

The robots only know three commands:

- movedirection(wheredirectionisW,E,N, orS), to move to an unoccupied adjacent square
    in the chosen direction;
- pushdirection(wheredirectionisW,E,N, orS), to push an adjacent box into the unoccu-
    pied square in the chosen direction;
- endwhen the robot has pushed its box to its destination and terminates.

Admittedly, we could have done it all with simply themoveandendcommands (thepushcould
be seen as simply a particular case ofmove), but I prefer to make the box pushing more explicit.
To simplify the path planning problem somewhat, we will allow robots and boxes to move
over doors other than their designated destination (so you don’t have to write an obstacle-avoiding
path). The path shown for the box in Figure 4 is therefore acceptable. This assignment is not about
designing algorithms for path planning, but about synchronization and deadlocks.


#### Door 0

#### Door 2

#### Box 3

#### Robot 3

Figure 4: A box or a robot may move over the square of a door other than their assigned destination.

### 2.4 Input parameters of the program

Your programrobotsV<version number>should take as parameters from the command line:

- the width and heightNof the grid,
- the numbernof boxes (and therefore of robots),
- the numberpof doors (with 1 ≤p≤ 3 ),

and then randomly generate:

- a random location for each door;
- an initial position of each box on the grid (be careful that boxes should not be placed on the
    edges of the grid, that two boxes should not occupy the same position, and that a box should
    not be created at the same location as a door),
- an initial position for each on the grid (be careful that a robot should not occupy the same
    position as a door, a box, or as another robot);
- randomly assign a door as destination for a robot-box pair.

So, the grid and robot task assignment shown in Figure 3 may have been produced by launching
on the command line

```
robotsV1 16 16 4 3
```
Things are going to look prettier on screen with a square grid but I highly recommend that you
do your testing with different sizes of non-square grids. It’s so easy to confuse a row index and a
column index in code; having a rectangular grid almost guarantees that a row-column accidental
swap will result in an “index out of bounds” error that will expose your bug.


### 2.5 Output of the program

The desired output of the program is a text filerobotSimulOut.txt. This file will contain:

- First line: the input parameters of the simulation (the sizeNof the grid, the numbernof
    boxes, the numberpof doors);
- A blank line;
- On a separate line, for each door, its initial coordinates on the grid;
- A blank line;
- On a separate line, for each box, its initial coordinates on the grid;
- A blank line;
- On a separate line, for each robot, its initial coordinates on the grid and the number of its
    destination door;
- A blank line;
- A list of the “program” executed by each robot to complete its task. For example, a segment
    of this part of the output file might look like this:

```
robot 1 move N
robot 1 move N
robot 3 push W
robot 2 move E
robot 0 push S
...
```
As you can imagine, there are synchronization problems to solve, and risks of deadlock. We will
come to that step by step.

### 2.6 Basic path planning: a sketch of the algorithm

I will not try to pretend that this path planning algorithm is particularly clever or efficient, but it
gets the job done. Feel free to suggest and implement a better algorithm if you believe that this
will improve the deadlock problem.
Looking at Figure 5 we see that the simplest path for the box consists of a series of horizontal
pushes, to bring the box align vertically with the destination door, and then of a series of vertical
pushes, to bring the box to coincide with the door. In the case of the illustration, we see that the
difference ofxcoordinates between the destination door and the box (in its initial location) is+9,
while the difference ofycoordinates is− 6. The box will therefore be brought to the door by
applying in sequence 9 timespush E, then 6 timespush N^1

(^1) You may prefer to accomplish the vertical displacement before the horizontal one.


```
Door 0
```
```
Box 3
```
```
Robot 3
```
```
Δ xb = +
```
```
Δ yb = - 6
```
```
Figure 5: Moving the box to the door.
```
In order to be able to execute thepushcommands, the robot must position itself on the proper
side of the box. Figure 6 summarizes the path followed by the robot to complete the box pushing
path^2. Since the robot is going to performpush Ecommands, it must first position itself on the
left side of the box. After it has completed its series of 9push Ecommands, it must position
itself below the box in order to be able to executepush Ncommands.

#### Door 0

#### Box 3

#### Robot 3

```
Figure 6: Path followed by the robot.
```
(^2) Here again, I had the robot move to a destination by traveling first along thexdirection then along theydirection.
A zigzagging path going directly to the destination point would be aesthetically more pleasing, but also more difficult
to implement.


The complete “program” for Robot 3 to write to the output file in this example would be:

```
robot 3 move W
robot 3 move W
robot 3 move W
robot 3 move S
robot 3 move S
robot 3 move S
robot 3 move S
robot 3 move S
robot 3 push E
robot 3 push E
robot 3 push E
robot 3 push E
robot 3 push E
robot 3 push E
robot 3 push E
robot 3 push E
robot 3 push E
robot 3 move S
robot 3 move W
robot 3 push N
robot 3 push N
robot 3 push N
robot 3 push N
robot 3 push N
robot 3 push N
robot 3 end
```
# 3 Implementation, Part I: Building and Running the Handout

## 3.1 The code handout

The code handout consists of 2 source files and 1 header file. Normally, you shouldn’t have to
make any changes to the header and source file dealing with the “front end.” Even within the
main.csource file, there are clearly identified parts that you should probably don’t mess with.
Modify any of the “don’t touch” sections at your own peril.

## 3.2 Install MESA and FreeGlut

You should have installed these for the last assignment, but in case you didn’t, here is a reminder
of what you need to do:

- sudo apt-get install freeglut3-dev
- sudo apt-get install mesa-common-dev


## 3.3 Build and run the handout

After you have installed MESA and FreeGlut, you should be able to build the handout:
g++ -Wall main.cpp glfrontEnd.cpp -lm -lGL -lglut -o robots
Of course, feel free to change the name of the executable. What you cannot change, though, is the
order that has you list the libraries to loadafteryou have listed the source files to compile. This
doesn’t make much sense, but this is the way you must do it if you want to avoid linker errors.

## 3.4 The GUI

When the program runs, you should see a window with two panes. The one on the left displays a
grid showing a grid and the location of robots, boxes, and doors. I hard-coded a number of doors,
robot, and boxes locations so that you don’t stare at an empty grid. Also, the code of the handout
shows you how to display things.
I remind you that a robot is paired with the door of the same index. A robot-door pair has
been assigned a door they must reach. Robots and boxes are rendered with a colored contour that
indicates their target door.
There are a few keyboard commands enabled:

- Hitting the ‘escape’ key terminates the program;
- Keys ‘,’ and ‘.’ are used to slow down or speed up the simulation by changing the sleeping
    time of the robot threads between consecutive moves..
Note that currently the size of the text and some aspects of the drawing of the robots, boxes,
and doors are hard-coded. If you select a high dimension for your grid, the grid display may look
off.

# 4 Implementation, Part I: No Mutual Exclusion

## 4.1 First implementation: robotsV

I don’t want you to rush trying to implement directly the full problem, because too many of you
will end up with a mess of nonworking code. Therefore, I am going toimposetwo intermediate
versions, to make sure that you have the basic part of the program working before you add in
multiple threads and mutex locks.
In this version, you are going to solve the planning problem for each robot while ignoring
other robots and boxes. Furthermore, you will not yet implement the robots as separate threads.
Just call what will become your thread function, so that your robots will run and terminate their
task one after the other. Just make sure to implement the path planning for a robot (and output
of the computed robot program to the common output file) into a separate function. Since this
function is going to turn into the thread start function in the following version, make sure to define
a struct type to pass all the information that a robot (and soon, thread) thread will require to do its
calculations.
The ”robot program” section of the output file will therefore give the complete program for
each robot, listed one after the other (since in this single-threaded version each robot terminates its
box pushing task before the next robot undertakes its own task).


## 4.2 Second implementation, multithreaded: robotsV

In this version, you are going to implement each robot as a separate thread, but you are still going
to solve the planning problem for each robot while ignoring other robots and boxes.
As was the case in Assignment 6, if you want to join a robot thread in the main thread, for
whatever reason, you can only do that after theglutMainLoop()call at the end of the main
function. The reason for this is that in a GLUT-driven program, the handling of all events and
interrupts is left over to GLUT. If you insert apthreadjoincall (basically, a blocking call)
anywhere, you basically block the graphic front end. Keyboard events won’t be handled anymore
and no more rendering will occur. There is no way around that as GLUTmustrun on the main
thread. It is a the price to pay for it being such a light-weight, portable, and easy to use library.
This version doesn’t implement mutual exclusion for grid squares, but there is already a syn-
chronization problem: You must use a mutex lock to protect access to the output file.

# 5 Implementation, Part II: Mutual Exclusion

## 5.1 Complete implementation: robotsV

In this version, your robot threads must now verify that the grid square they—or the box they are
pushing—move to is free before they perform the move. You cannot use busy wait for this task: It
must be done with a mutex lock. It is up to you to decide how many mutex locks should be used
for this problem.

# 6 What to Hand in

## 6.1 Source code

The source code should be properly commented, with consistent indentation, proper and consistent
identifiers for your variables, functions, and data types, etc.

## 6.2 Note on the report

In the report, you will have to document and justify your design and implementation decisions, list
current limitations of your program, and detail whatever difficulties you may have run into.

6.2.1 Limitations of the program

It is important to be able to identify things that your program cannot do, or cannot do well, and it’s
crucial that you identify situations that can make it crash. Even if you lack the time to solve these
issues, it is important to identify problems. Think of it as a sort of “todo” list for future revisions.

6.2.2 Difficulties you ran into

By this I don’t mean “my car was for repair this week.” Rather, I mean coding problems that forced
you to think of some clever solution.


# 7 Grading

- Version 1 completed and performs as required: 10%
- Version 2 completed and performs as required: 20%
- Version 3 completed and performs as required: 20%
- good code design: 10%
- comments: 15%
- readability of the code: 10%
- report: 15%


