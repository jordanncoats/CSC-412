# CSC 412 – Operating Systems

## Programming Assignment 02, Fall 2019

```
Thursday, September 19th, 2019
```
Due date:Friday, September 27th, 11:55pm.

# 1 What this Assignment is About

## 1.1 Objectives

The objectives of this assignment are for you to

- Get some practice with C arrays, pointers, and strings.
- Learn how to iterate through the catalog of a directory, in bash.

This is an individual assignment.

## 1.2 Handout

There is no handout for this assignment besides this document as a.pdffile.

# 2 Part I: Selection and Ordering

## 2.1 Partial ordering and space characters

In this week’s lab, you get some practice at performing a partial sorting of the characters in a string.
For this part of the assignment, you must perform a similar task, but this time you must eliminate
repeated characters and handle space characters in the input string.
When you launch a program, the command line is broken down into separate “arguments”
separated by spaces. This works well as long as you don’t want to use as input for your program a
string that contains space or tabulation characters. An imperfect solution would be to reconstitute
the full string in the program by adding spaces between the arguments. Unfortunately this would
not preserve the original form of the input string (tabs, multiple spaces).
The correct solution to this problem is to get complex input strings through a call toscanf
rather than through the initial argument list:

1. Print a message asking the user to enter an input string.
2. Capture the input string withscanf, treating space and tab characters as regulaer characters.
3. Process the string.


```
Program 1.Write a program consiting of a single source file namedtask1.cthat asks
the user to enter a string to process (that string may contain space and tab characters)
and outputs between double quotes a new string in which the letters appear first, then the
numbers, then all the other characters, but each character appears only once (repeats have
been eliminated) and the order of first occurrence is that of the input string. After printing
out the partially sorted line, your program should ask the user for a new string to process,
and should keep doing so enter the user enters the stringEXIT.
```
An example of execution of the program is shown below.

enter an input string: Ab++ CSC412 xAbS-+
"AbCS412+ -"
enter an input string: 1+1=2, 23-9=14, and a+d*c=b
"adcb12394+=, -*"
enter an input string: the end
"thend "
enter an input string: EXIT

## 2.2 Full ordering

For this part, you are going to use theqsortfunction of the standard C library. This function
lets you sort the elements of an array of any data type, as long as you implement a “comparator
function” that specifies how any two elements should be ordered.

```
Program 2.Write a program consiting of a single source file namedtask2.cthat asks
the user to enter a string to process (that string may contain space and tab characters)
and outputs between double quotes a new string in which the letters appear first, then the
numbers, then all the other characters, but each character appears only once (repeats have
been eliminated) and the elements of a same category are sorted by order of increasing
ASCII code. After printing out the partially sorted line, your program should ask the user
for a new string to process, and should keep doing so enter the user enters the stringEXIT.
```
An example of execution of the program is shown below.

enter an input string: Ab++ CSC412 xAbS-+
"ACSb124 +-"
enter an input string: 1+1=2, 23-9=14, and a+d*c=b
"abcd12349 *+,-="
enter an input string: the end
" dehnt"
enter an input string: EXIT


# 3 Part II: Use a custom struct data type

In this part of the assignment, instead of hard-coding the ranges of ASCII codes defining the
different categories of characters that we want to sort out, we are going to design customstruct
data types to specify this information.

## 3.1 Character type specification

In the problem we have been working on so far, one character category (numbers) is defined by a
single range of ASCII codes (any character between the code for’0’and that for’9’, inclusive).
An other category (letters) is defined by two ranges (any character between the code for’a’and
that for’z’, inclusive, or between the code for’A’and that for’Z’, inclusive). The last category
is simply for all other characters.
We don’t need a data type to represent that last category, but the other two will need a new
data type. You are going to define astructdata type that lets you specify whether a category is
defined by a single or multiple ASCII code ranges (why stop at two?), and what these ranges are.
You will also need to store a unique index for each category, as well as a printable name for the
category (e.g. “number”).

## 3.2 A data type to store all the categories

You are going to define anotherstructdata type to hold together the number of categories
defined for a sorting problem as well as the data structures for each of these categories.

## 3.3 Implement the sorting problem using your custom types

You can now replace all hard-coded ranges in the sorting problem by an instance of your new data
type. Of course, you need to modify your sorting function to take this information as an argument.

```
Program 3.Write a program consiting of a single source file namedtask3.cthat repro-
duces the external behavior of the previous version, but uses your customstructdata
types instead of hard-coded category ranges.
```
```
Program 4.Write a program consiting of a single source file namedtask4.cthat adds
to the last version a new character category for mathematical operators (’+’, ’-’, ’*’, ’/’,
’=’, ’!’, ’%’), with an output order of: numbers, then mathematical operators, then letters,
then other characters.
```

## 3.4 Extra credit (up to 8 points)

Of course, any list of characters could be defined as a list of ranges of ASCII codes, even if these
ranges only contain one character. For example, in the case of your category for mathematical
operators (’+’, ’-’, ’*’, ’/’, ’=’, ’!’, ’%’), the character ’!’ appears in the range from ’!’ to ’!’. It
would be more elegant toalsoallow a definition of character types simply made up of a list of
characters (in addition to the range-based definitions).
We are going to give up to 3 points of extra credit for a revised “character category” data type
that handles list of characters, and then up to 5 points for modifying your sorting function to acco-
modate the new data type.

```
Extra Credit 1. Write a program consiting of a single source file named
extraCredit1.cthat adds to the last version a new character category for mathe-
matical operators (’+’, ’-’, ’*’, ’/’, ’=’, ’!’, ’%’), with an output order of numbers, then
mathematical operators, then letters, then other characters.
```
# 4 Part III: Directory Catalog in bash

The script section of the assignment is completely disconnected from the C program. You are
going to write a script that iterates through the list of files in a given directory.

## 4.1 Version 1

For this version of the script, you simply need to list all the files visible inside a directory.

```
Script 1.Write a bash script consiting of a single file namedscript1.shthat takes as
single argument the path to a directory and prints out the list of all the files found in that
directory (excluding “files” that are really directories.
```
## 4.2 Version 2

For this version of the script, you musty now group the files by type, based on the file extension.

```
Script 2.Write a bash script consiting of a single file namedscript2.shthat takes as
single argument the path to a directory and prints out the list of al the files found in that
directory, grouping them by categories: source files (.cand.cppextensions), header
files (.hand.hppextensions), script files (.shextension), text files (.txtextension),
directories, and other.
```

## 4.3 Extra credit: 2 points

```
Extra Credit 2.Write a bash script consiting of a single file namedscript2.shthat is
based on the code of your first script, but forces the output to list the files in lexicograpical
order.
```
## 4.4 Extra credit: 1 point

```
Extra Credit 3.Write a bash script consiting of a single file namedscript2.shthat is
based on the code of your second script, but forces the output fopr each file category to list
the files in lexicograpical order.
```
# 5 Part IV: The Report

Starting with this assignment, you will be expected to write a report, to explain some aspects of
data structure design and algorithms that are not conveniently explained in comments.
This week, the focus is obviously going to be on the data structures that you designed to define
the character categories and lists thereof. In the report, you are going to explain what you were
trying to store, why you chose one primitive data type over an other (e.g. charvs.unsigned
charvs.unsigned shortvs.intvs. customenumtype).
Similarly, in your “structofstructs” data type, do you store directlystructdata or
pointers tostructdata, and why?
Finally, I expect you to identify possible limitations of your programs and scripts relative to
the specifications of the assignment. Are there valid input values for which your program can fail
(give incorrect output or even crash)? Being aware of one’s code limitations and deficiencies is
an important aspect of development and debugging. It calls for thoughtful testing (not just run
for a couple of simple examples, see that you got an output and call it job done): Can you think
of “border cases” that maybe you ignored when you were developing your C program or script?
Maybe there is still time to address these cases. If not, don’t try to hide them: It will look worse if
we find out while grading you that you didn’t even test for the case of an emapty directory in the
bash part of the assignment.


## 5.1 Format of the report

The report should be written in a word processor, using a proportional font, and exported as a
.pdffile that you will join to your submission.

# 6 What to submit

## 6.1 The pieces

If you have kept track of the various steps so far, you should have

- Your report, as a.pdfdoocument;
- Four to six source files, which you should put in a folder namedPrograms;
- Two to fourbashscript files, which you should put in a folder namedScripts.

Place your report, and yourProgramsandScriptsfolders in a folder namedProg 02, zip
that folder (and Idomean zip: Not rar or other compression scheme) and upload the.ziparchive
to your submit folder.

## 6.2 Grading for this assignment

For this assignment, we are going to go for bark over bite, pointing at problems, telling you what
is going to be penalized in future assignments. So, don’t pat yourself on the back if you get a
good grade, ignore the comments and warnings we provide, and act all surprised when point start
coming off in future assignments.

## 6.3 Grading sections

- Report: 15 pts
- Code quality and comformance to specifications: 25 pts
    - C program: 15 pts
    - bashscript: 10 pts
- Execution: 30 pts
    - C program: 15pts
    - bashscript: 15 pts
- Folder organization: 10 pts


6.3.1 Code quality and comformance to specifications

The code of your C programs and scripts is expected to meet standards of quality:

- Correct, consistent indentation;
- Good, consisten choice of identifiers for variables, functions, and data types;
- Appropriate choice of data types;
- Modular implementation;
- Good level of documentation in comments;
- etc.

The amount of points that you can expect to get for this section of the grade will be comensurate
to the degree to which you meet the specifications of the assignment. Obviously, a beautifully
comented and indentedHello World! program will not get any quality points. Similarly, if
you you only complete one task out of four, the quality score will go down accordingly (but not
quite proportionally).


