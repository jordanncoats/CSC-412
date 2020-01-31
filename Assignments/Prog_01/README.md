# CSC 412 – Operating Systems

## Programming Assignment 01, Fall 2019

```
Monday, September 9th, 2019
```
Due date:Tuesday, September 17th, 11:55pm.

# 1 What this Assignment is About

## 1.1 Objectives

The objectives of this assignment are for you to

- Setup the environment that you are going to use for all assignments this semester;
- Learn some simple Unix commands (if you were not already familiar with Unix/Linux);
- Get started with C programming and bash scripting;
- Make sure that you follow properly assignment specifications.

This is an individual assignment.

## 1.2 Handout

There is no handout for this assignment besides this document.

## 1.3 On Unix vs. Linux

Throughout this assignment, I say “On Unix,... ” Please be aware that strictly and legally speaking,
Linux isnotUnixTM, but merely “Unix-like,” because it has not been certified by the Open Group
(unlike, say, Apple’s macOS, formerly known as MacOS X). This being said, over 99% of terminal
commands are found on both system, with only minor variations in the list of options.

# 2 Part I: Setup the Environment

As mentioned in the syllabus, all projects will be graded on the—as of today—latest version of
Linux Ubuntu running in a VirtualBox virtual machine. Of course, we can’t control what environ-
ment you use for development, but please be aware that there are subtle differences in the execution
of system calls, and even regular C code, on Linux, Windows, and macOS. Just because your code
builds and runs fine in Xcode on macOS is no guarantee that it will do so on Ubuntu Linux. It is
yourresponsibility to test your code on Ubuntu before you submit it. The graders will not grant
any partial credit for “but it runs in<whatever OS>” excuses.


## 2.1 Virtual Box

Download and install VirtualBox athttps://www.virtualbox.org.

## 2.2 Linux Ubuntu

Go to the Ubuntu web sitehttps://www.ubuntu.comand download the latest stable version
of Ubuntu Desktop. At the time of this writing, this would be Ubuntu 18.04 LTS.

## 2.3 Create a virtual machine and install Ubuntu on it

When you create your virtual machine in VirtualBox, make sure to give it settings in memory and
disk space that are at least as large as the minimum requirements to install Ubuntu. Follow the
directions^1.

```
Step 1.Take a screenshot of the settings of the Ubuntu virtual machine that you just cre-
ated. Name the screenshot filestep1.pdf(of course, convert the file to pdf if this is not
the native format for screenshots on your system).
```
# 3 Part II: Simple Linux Walkthrough

This part of the assignment will consist in a leisurely walk through some common and useful Linux
commands, documented by screenshots that you will take as you complete each step.

## 3.1 Printing the documentation

You can printout the documentation of any Unix command by typingman(formanual) followed
by the name of the command. For example,man cdwill print out documentation about thecd
(forchange directory) command, which lets you navigate the folder hierarchy on your system.
You can also search for command names by typingman -kfollowed by a keyword for the
search. Obviously, if you put a completely generic keyword such asfileyou will get a ridicu-
lously long list of commands.

## 3.2 Create a working directory

If you have moved away from your home folder, you can return there by executing thecdcom-
mand with no parameter. Create a new folder using themkdircommand. Move to that folder and
printout the path this (current) working directory by using thepwdcommand.

(^1) No, I won’t give hand-holding directions for every single subtask of this assignment. You are computer scientists
and you have to be able to follow use online documentation to complete some tasks assigned to you. If you don’t know
already how to do that, this assignment is a good a opportunity to start learning how to do it.


```
Step 2.Take a screenshot of the terminal after you executed thepwdcommand. Name the
screenshot filestep2.pdf(of course, convert the file to pdf if this is not the native format
for screenshots on your system).
```
Print out again the path to the current working directory, but instead of getting the output in the
console,redirectit to a file using the>operator:pwd > path.txt. You can view the content
of the file either by typingpr path.txtormore path.txt(look at the man pages to see
the differences between these two commands.

```
Step 3.Take a screenshot of the terminal showing the content of thepath.txtfile after
you executed the commands listed in this subsection. Name the screenshot filestep3.pdf
(of course, convert the file to pdf if this is not the native format for screenshots on your
system).
```
## 3.3 Create and delete a text file

You can create a new (empty) text file with the terminal commandtouch. You can then verify in
the GUI explorer that the file has indeed been created, and confirm this by listing all the files in the
current working directory. If you haven’t done so already, you should probably spend some quality
time getting acquainted with the various options of thelscommand. I personally find the variant
ls -laparticularly useful. We will see in class during the semester what all the information in
the list output means, and how to exploit and modify it. Now that you see that the file has been
created, delete it using thermcommand.

```
Step 4.Take a screenshot of the terminal and GUI explorer that you have completed this
step in the assignment. Name the screenshot filestep4.pdf(of course, convert the file
to pdf if this is not the native format for screenshots on your system).
```
## 3.4 Edit a file in a text editor

There is an old and very silly dispute among Unix users as to which text editor,vi/vimoremacs,
istheone and true text Unix editor. You can pick your camp in this tired old battle, or you can
simply eschew it by opting for one of the newer text editors (Robert, our TA, advocatesnano,
which I have nothing polite to say about, being aviperson myself, like anybody with proper
taste^2 ).
Use the text editor of your choice to open a next text file, type in the customary lineHello
World!and save the file.

(^2) I never claimed that old, silly disputes were beneath me.


```
Step 5.Take a screenshot of open editor window. Name the screenshot filestep5.pdf
(of course, convert the file to pdf if this is not the native format for screenshots on your
system).
```
## 3.5 Command history

Unix keeps track of the commands that you have executed recently. You can display your recent
history by typing the commandhistory. You can use the up and down arrow keys to bring back
to the current terminal line prompt a command that you have already execute. This is particularly
convenient when you have to execute multiple times long commands with multiple parameters
(coming in future assignments). You can also execute a specific command by typing! followed
directly (no space) by the number of that command in your history. Note that if you have multiple
terminal windows open, they typically will report different command histories because they corre-
spond to different executions of ashell(we will explain this term next week in class).

```
Step 6.Take a screenshot showing that you have experimented with the history features of
Unix. Name the screenshot filestep6.pdf(of course, convert the file to pdf if this is not
the native format for screenshots on your system).
```
## 3.6 Final step (wrap-up)

Save your command history to a file namedhistory.txt, edit this file to add a first line con-
taining your name. Save thehistory.txtfile.

## 3.7 Extra credit (2 points)

You should never use a account with administrator privileges as your everyday account. This is
true whether your main system is mac OS or Windows (or even Linux), and absolutely, definitely
if you do any kind of coding.
Therefore, I will give two points of extra credit for providing an extra screenshot showing that
your Ubuntu virtual machine has one admin account and one non-admin account. Of course, to
get the two points, all the other screenshots provided should show that you used the non-admin
account to do the assignment.


# 4 Part III: A Little C program

## 4.1 What the program should compute

Your program will take either one or two strictly positive integer arguments. If the program was
launched with a single argumentn, then you should compute and output the firstnprime num-
bers. For example, if your executable is namedprogand is launched from the console with the
command

```
./prog 8
```
Then your program should produce the following output:

The first 8 prime numbers are: 2, 3, 5, 7, 11, 13, 17, 19.

If the program was launched with two arguments, then your program should output the list
of all primes numbers in the range specified. example, if your executable is namedprogand is
launched from the console with the command

./prog 11 30

Then your program should produce the following output:

There are 5 prime numbers between 11 and 25: 11, 13, 17, 19, 23.

In the case where the user provided two arguments, your program should not expect the second
argument to be larger than the first. if your executable is namedprogand is launched from the
console with the command

./prog 20 10

Then your program should produce the following output:

There are 4 prime numbers between 20 and 10: 11, 13, 17, 19.

## 4.2 Data validation and error reports

You should check the argument(s) of your program and report an eventual error using one of the
following messages (again assuming that you built an executable namedprog):

- prog launched with no argument.
    Proper usage: prog m [n]
- prog launched with too many arguments.
    Proper usage: prog m [n]
- prog’s argument is not a strictly positive integer.
- prog’s first argument is not a strictly positive integer.
- prog’s second argument is not a strictly positive integer.

Needless to say, the name of the executable should not be hard-coded in your C program.


## 4.3 Regarding output specifications

In this and future assignments, when I give specifications for the format of the output, you are
expected to follow these specifications and will be penalized if you don’t.

## 4.4 Extra credit (2 points)

Handle the case where there is either no prime number or a single number to list and the plural
should not be used in the output. For example:

The first prime number is 2.

There is only one prime number between 12 and 16: 13.

There is no prime number between 24 and 28.

## 4.5 Extra credit [2 points]

In your output, for counts of prime numbers lower than twenty, replace the numerical version of
the counter by its letter version. For example, rather than printing out

There are 5 prime numbers between 11 and 25: 11, 13, 17, 19, 23.

print out

There are five prime numbers between 11 and 25: 11, 13, 17, 19, 23.

# 5 Part IV: a first simple bash script

## 5.1 What it is about

Here again, nothing fancy: We want you to write abashscript that launches an executable,
multiple times.

## 5.2 The script

Your script should take as arguments a path for the executable of the primes program, as well as a
list of strictly positive integers. Then your script should do the following:

- Launch the prime number computation program for the largest of the arguments, to get the
    longest list of prime numbers.
- Launch again the prime program for each pair of arguments, to compute the list of primes
    between these two numbers.


## 5.3 Output of the script

When the numerical arguments to your script are all strictly positive integers, then the entire output
is provided by the primes program. For example, if your C executable is namedprogand is in
the same directory as the bash script, and then your script is launched with the command

```
bash ./myScript.sh ./prog 6 4 3
```
then the output should be:

The first 8 prime numbers are: 2, 3, 5, 7, 11, 13, 17, 19.
There are 1 prime numbers between 6 and 4: 5.
There are 2 prime numbers between 6 and 3: 3, 5.
There are 1 prime numbers between 4 and 3: 3.

On the other hand, if any the script was launched with an invalid argument list, your script
should simply terminate and report:

Invalid argument list.
Proper usage: myScript <path to executable> m1 [m2 [m3 [...]]]

## 5.4 Extra credit: up to 4 points

Provide a more “granulated” error report: too few arguments, which argument is invalid, missing
executable, etc.

## 5.5 Extra credit: 4 points

If a value is repeated in the list of arguments to your script, then use this value only once to launch
the primes program.

# 6 What to submit

## 6.1 The pieces

If you have kept track of the various steps so far, you should have six screenshots documenting the
progressive steps through the assignment, an edited text file listing the history of your commands
in the terminal, a C source file, and the code of abashscrip.

## 6.2 Organization

All the screenshots, named as specified earlier, should go into a folder/directory namedScreenshots,
while the C program and the script should go into a folder namedCode. These two folders and the
history.txtfile should be placed into a folder namedProg 01. Compress this folder into a
ziparchive and upload the archive to your submit folder before the submission deadline.


## 6.3 Grading for this assignment

For this assignment, we are going to go for bark over bite, pointing at problems, telling you what
is going to be penalized in future assignments. So, don’t pat yourself on the back if you get a
good grade, ignore the comments and warnings we provide, and act all surprised when point start
coming off in future assignments.

6.3.1 Code quality

For this part of the grade we look at things such as:

- Proper indentation;
- Good comments;
- Good choice of identifiers (chances are thata,b, andcare not good identifiers, particularly
    if the first two are strings and the third one an integer;
- Consistent choices of identifiers. For example, if you have a variableint beanCount
    that does what it claims to do, then it definitely would be a bad idea to have another variable
    int numofboxes.
- Good implementation decision: avoid super long functions (at your current skill level, if
    your function occupies more than one page of your screen, it’s probably too long); select
    proper data types; create your own when appropriate (and only then), etc.

Your grade for this section will reflect how much you actually implemented. A superbly com-
mented and indented “Hello World!” program won’t bring you many “code quality” points.

6.3.2 No syntax error

Code with syntax errors (compiler errors for the C program) will get a grade of 0 for execution
and quality. Comment out the part with syntax errors and explain in the comments what you were
trying to do.

## 6.4 Comments

There is no separate report for this assignment (coming soon), but you are expected to document
properly your code with comments that explain what you are doing.

## 6.5 Grading sections

- Screenshot for each of the steps in the walk-through: 2.5% each (15% total)
- History file: 10%
- Code quality: 25%
    - C program: 15%


- bashscript: 10%
- Execution: 30%
- C program: 15%
- bashscript: 15%
- Folder organization: 20%

6.5.1 Execution

We are going to test your program and script with different list of arguments, some valid some not.
Your grade for this section will reflect to what extent you produce the desired output for all the
test cases. So, make sure that you test your program and script before you submit; don’t simply
replicate the examples of this handout.


