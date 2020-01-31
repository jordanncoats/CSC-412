# CSC 412 – Operating Systems

## Programming Assignment 06, Fall 2019

```
Tuesday, November 19th, 2019
```
Due date:Sunday, Dec. 1st, 11:55pm.

## 0.1 Objectives

The objectives of this assignment are for you to

- Getting further experience with the pthread library;
- Experiment with the synchronization of pthreads using mutex locks;
- Learn how to use a third-party library in C.

This is an individual assignment.

## 0.2 Handout

The handout for this assignment is the archive of a C++ project made up of 2 source files and
1 header file.

## 0.3 The problem

In this assignment, you are going to implement “traveler” threads that move randomly (with some
constraints) on a rectangular grid. They roam randomly on the grid, but a traveler that reaches a
grid corner terminates. Travelers can be of three kinds, indicated by a color (red, green, or blue).
They will record a trail of their travel on the grid.
However, in this “nothing comes free” world, drawing a colorful trail requires acquiring ink
resources. A “red” traveler that plans to execute a 5-square long displacement on the grid needs
to acquired 5 units of red ink to draw its trail. There are limited-capacity tanks for each kind of
ink. When a tank is empty, the travelers of the corresponding color cannot move until the tank is
refilled again (maybe only partially). In the version of the code handed out, the tanks are refilled
when the user hits the appropriate key (‘r’, ‘g’, or ‘b’).

# 1 Part I: Building and Running the Handout

## 1.1 The code handout

The code handout consists of 2 source files and 1 header file. Normally, you shouldn’t have to
make any changes to the header and source file dealing with the “front end.” Even within the


main.cppsource file, there are clearly identified parts that you should probably don’t mess with.
Modify any of the “don’t touch” sections at your own peril.

## 1.2 Install MESA and FreeGlut

MESA is a open-source implementation of OpenGL, the only free option on Linux. FreeGLUT is a
more up-to-date version of GLUT (the GL Utility Toolkit), a library that provides binding between
OpenGL and the OS^1. MESA and FreeGLUT come installed by default on Ubuntu, so that you
can run pre-built OpenGL, but in order to build a MESA project, you need to install the developer
version of both libraries.
Make sure first that your Ubuntu install is up to date, and then execute:

- sudo apt-get install freeglut3-dev
- sudo apt-get install mesa-common-dev

## 1.3 Build and run the handout

After you have installed MESA and FreeGlut, you should be able to build the handout:
g++ -Wall main.cpp glfrontEnd.cpp -lm -lGL -lglut -o travel
Of course, feel free to change the name of the executable. What you cannot change, though, is the
order that has you list the libraries to loadafteryou have listed the source files to compile. This
doesn’t make much sense, but this is the way you must do it if you want to avoid linker errors.

## 1.4 The GUI

When the program runs, you should see a window with two panes. The one on the left displays a
colored grid. This grid is going to show the colored trails of multithreaded “travelers.” The right
pane shows the state of the simulation: The number of “live” traveler threads (i.e. of threads that
have not terminated after having reached a grid corner) and the current level of the three ink tanks.
There are a few keyboard commands enabled:

- Hitting the ‘escape’ key terminates the program;
- Hitting keys ‘r’, ‘g’, or ‘b’ partially refills the corresponding ink tank (the amount of the
    refill is defined by a variable defined inmain.c);
- Keys ‘,’ and ‘.’ are also recorded, but their effect will only concern you if you decide to
    tackle the extra credit of Subsection 4.3.

(^1) OpenGL is blissfully OS-unaware. It has no knowledge of input devices, displays, windows, or even of time. All
OpenGL says is “give me a buffer/raster to draw into.” GLUT is a crossplatform library that gives developers a simple
way to build primitive, cheap-looking interfaces.


# 2 Part II: Basic Version

## 2.1 Movement of the travelers

Travelers can only move vertically or horizontally on the grid. This means that in addition to a
position (row and column index) on the grid, there is also a current orientation of each traveler.
The initial position and orientation of the travelers are generated randomly on the grid.
At each iteration of its main loop, a traveler thread selects randomly a new displacement di-
rection and displacement length. The new direction of displacementmustbe perpendicular to the
current direction. The length of the displacementmustkeep the traveler within the dimensions of
the grid (needless to say, being a length, it must be positive).
A traveler that reaches one of the corners of the grid terminates its execution. If you want to join
that thread in the main thread, for whatever reason, you can only do that after theglutMainLoop()
call at the end of the main function. The reason for this is that in a GLUT-driven program, the
handling of all events and interrupts is left over to GLUT. If you insert apthreadjoincall
(basically, a blocking call) anywhere, you basically block the graphic front end. Keyboard events
won’t be handled anymore and no more rendering will occur. There is no way around that as GLUT
mustrun on the main thread. It is a the price to pay for it being such a light-weight, portable, and
easy to use library.

## 2.2 Color trails

A traveler going through a square will leave a trail of its designated color in that square. By now,
you are experts in the manipulation of color information, so it has not escaped to you that “red,”
“green,” and “blue” travelers don’t need to overwrite each other trails. They will simply write their
trail in the appropriate color channel of the grid square.
I defined the grid as a 2D array ofint. You should have no problem accessing the color
channels of the grid squaregrid[i][j]. In fact, I give an example of how to do this, as a
certainly unnecessary reminder of the operation, in the initialization of the grid included in the
handout. Needless to say, this code should be replaced by the commented-out code that sets all
grid squares black.

## 2.3 Traveler threads

You have to create at least eight traveler threads, assign them a random color among the three
available, an initial position on the grid, and an initial orientation. Use the data types defined in
glfrontEnd.h: TravelerTypefor the choice of ink color andTravelDirectionfor
the orientation.
Two travelers may occupy the same grid square. In other words, only the color information of
a grid square is considered a shared resource under race condition. The “space” within the grid
square is not under race condition for this simulation.
A single-threaded implementation (no traveler thread handling displacements and color trail)
will be penalized by 40 points. An implementation with a single thread of each type will be
penalized 20 points.


# 3 Extra Credit

## 3.1 Color levels (3 points)

In the assignment as discussed so far, a traveler thread marked as “red” (resp. “blue” or “green”)
leaves a colorful trail by setting to0xffits respective color channel. We can tell if a grid square
has been traversed by “red” and “green” travelers as opposed to “blue” travelers only because, in
the case of the former, the square will be colored yellow while, in the case of the latter, it we be
colored blue. There is, however, no way to tell how many of each type have gone through the
square.
In this enhanced version, you shouldincrementthe level of color rather than setting it directly
to0xFF. Don’t increment by 1, since we won’t see any difference, rather by 16 or 32. Be careful
not to overflow the value of a color channel, as it cannot exceed0xFF(that is, 255).

## 3.2 Maintain and synchronize traveler info (up to 8 points)

In themain.csource file, some sections that have been commented out provide support for a data
type namedTravelerInfo. An array of structs of this type can be passed to the graphic front
end so that the current position and orientation of the travelers is now shown on the grid.
You need to properly initialize and maintain the values of the different fields of this array.
Naturally, because there is the possibility of race condition on this data, you must synchronize
access.

## 3.3 Add ink producing threads (up to 15 points)

In the version of the program handed out, the user can partially refill a ink tank by hitting the
appropriate key (‘r’, ‘g’, or ‘b’). We would like to automatize the process and instead have
threads in charge of refilling the tanks. Such threads would run an infinite loop of

- Sleep for a while (simulates time required to “produce” ink;
- Gain access to the appropriate ink tank;
- Add ink (don’t overfill);

If you look carefully at the code ofmain.c, there are variables and functions already defined that
control the size of a refill and the sleep time of a producer. Don’t rename any of these as this would
break code in the graphic front end. The functions to increase or decrease the sleep time are called
when the user presses the keys ‘,’ and ‘.’ (below ‘<’ and ‘>’). To get the full 15 points of extra
credit, you must implement multiple producer threads for each ink color. If you only implement
one of each type, then the maximum amount of extra credit points is 10 points, and only 7 points
if a single thread takes care of the three colors.


## 3.4 Synchronize access to the grid squares (up to 10 points)

Add a synchronization mechanism so that two travelers may not anymore occupy the same grid
square. We understand that this may lead to a deadlock, but you are not asked to detect and resolve,
or to prevent deadlocks.

# 4 What to Hand In

## 4.1 Organization of the submission

Create a new folder namedProg05<your name>. In that folder, place

- a directory containing all the headers and source code required to build your programs.
- your report as a.pdffile.

## 4.2 To verify before you submit the assignment

The graders have to be able to run your code “as is” on their own virtual machine. In particular,
they should not have to make changes/fixes to your code for it to build or run. This is one reason
why I imposed that we would all be working on a VirtualBox virtual machine using the same
distribution of Linux, so that platform differences are nearly eliminated. If the graders have to
make changes to source code, then we open Pandora’s box of whether their changes altered the
proper operation of the program, which changes would have been preferable, etc. I don’t want any
of that.

Important note on the C program: Your C programmust build with no errors. If there are
compiler errors, then you will get a grade of 0 for the code and execution sections of the grad-
ing. This is really the lowest of expectations that you submit code that compiles with no errors,
considering that we provide as starting point a handout that is partially functional.

## 4.3 What’s on the report

The main sections I want to see in this report are:

- A presentation and discussion of the choices you made for the implementation of ink acqui-
    sition and displacement. For example, does a traveler attempt to acquire in one shot then
    units of ink required for its displacement or does it break down the displacement into small
    steps? What does a traveler do if less thannunits of ink are available in the tank?
- A discussion of possible difficulties encountered in this assignment^2.
- A discussion of possible current limitations of your C program.

(^2) Here, I don’t mean “my laptop’s battery is dying” or “I couldn’t get a pizza delivered to my dorm room.”


## 4.4 Grading

- Implementation of synchronization: 25 points
- Correct execution: 30 points
- Code quality (readability, indentation, identifiers, etc.): 30 points
- Report: 15 points
- Penalties:
    - Single-threaded implementation: -40 points
    - One thread per traveler type: -20 points


