# CSC 412 – Operating Systems

## Programming Assignment 04, Fall 2019

```
Wednesday, October 23rd, 2019
```
Due date:Friday, November 1st, 11:55pm.

# 1 What this Assignment is About

## 1.1 Objectives

The objectives of this assignment are for you to

- Start experimenting with process creation usingfork();
- Write another image processing application;
- Use a bash script to search through a directory;
- Start using Doxygen to produce documentation for your code.

This is an individual assignment.

## 1.2 Handout

Besides this document, the handout for this assignment is a zip archive containing a solution to the
previous programming assignment.

# 2 Part I: Aggregating Images (Tiling)

## 2.1 Building up texture files

The operator that you are going to implement has actual practical applications in game develop-
ment. What somewhat reduces the use of our little application is that we are going to use the silly
TGA reader that I provided rather than a more complete library that supports “real” image file
formats such asPNG. I will offer extra credit options to overcome this limitation.
The task to perform consist in assembling together several image files to produce a large image
file. In game development, some animations (e.g. some small explosions, or the exhaust of a car
or propellers are actually represented as a sequence of small images). Rather than storing these as
a bunch of small files of same dimensions, it is actually more convenient (and faster to read) to
“tile” all of these small images into one big, as-square-as-possible, image.


## 2.2 Input to the program

The input to the program comes form the argument list and consists of a list of paths to image files,
and the path to an output image. All the input images will exist, have the same dimensions (no
need to check for that), and contain valid, color images in the TGA file format that you are now
familiar with.

## 2.3 What to do

Your program, to be namedtile.c, will determine the dimensions of the grid of “tile” images
that produces the “most square” composite image (something to discuss in your report), copy the
content of the tile image into the composite image, and write that image at the designated location.

# 3 Part II: Dispatcher Program

## 3.1 What it is about

Yourdispatcherprogram will take as arguments the path to an input image folder, the path to
an output folder, and a—possibly long—list of flattened commands involving thesplit,crop,
androtateoperators from the previous assignment. For example, thedispatchercould be
launched with the command
./dispatcher ../Images ../Output rotate LL Renoir.tga split
bottles.tga rotate rRRRLR Seurat.tga crop 23 45 80 100 clown.tga
rotate LL bottles.tga

The dispatcher will create the proper number of child processes to execute the various actions
requested by the user. For example, in the case of the list of commands, above, thedispatcher
should create processes to execute

- rotate LL ../Images/Renoir.tga ../Output
- split ../Images/bottles.tga ../Output
- rotate rRRRLR ../Images/Seurat.tga ../Output
- crop 23 45 80 100 ../Images/clown.tga ../Output
- rotate LL ../Images/bottles.tga ../Output

## 3.2 Implementation

Obviously, thedispatcherwill use thefork()function to create child processes, and these
will have toexecXY()to load the code of the appropriate image operator. Yourdispatcher
process should wait for all the image operators to terminate, and return as exit value the number of
operators that failed (so that an exit value of 0 is the standard return value for no error).


# 4 Part III: Doxygen (Finally)

## 4.1 Doxygen

Doxygen is a nice utility that lets C/C++ developers overcome their envy of Java developers would
could use javadoc to produce automatically good-looking documentation for their code in the form
of web pages, pdf document, or even LaTeX documents. In fact, Doxygen many other languages
(Python Perl, C#, etc), so you should consider using it for all of your development projects.

## 4.2 Install Doxygen

The first thing to do is to install Doxygen on your computer. It doesn’t matter much to me whether
you run Doxygen on a Mac or Windows PC rather than on your Ubuntu virtual machine, and we
probably couldn’t tell on which platform you generated your html documentation. Still, you are
going to have to install some other libraries on your virtual machine, so you may as well learn now
how to use it.
For Doxygen to work properly, you may need to install other utilities that it depends on, or that
enhance it. One of these isdot, another very useful program that you may consider using on its
own in the future to produce diagrams.

## 4.3 What to javadoc-style comment

Doxygen follows the javadoc syntax for documentation-producing comments (in particular, com-
ments start with/**rather than the standard/*).
When documenting a library, it is typically sufficient to produce documentation for the library’s
public API, which means that only the header files would need to get javadoc-style comments.
Here, since we are documenting applications, I also want documentation for “private” functions
and variables, which means, functions whose prototype is defined in the source file and variables
that are global to a source file.
In summary, you need to provide comments for

- All data types defined in a header file or a source file, including an entry for each field of a
    struct;
- All functions declared in a header or source, including
    - a summary of what the function does;
    - aparamfield for each parameter of the function;
    - areturnfield if the function is not ofvoidtype;
- All global variables defined in the various source files of your program.

## 4.4 Output the documentation in html format

Run Doxygen on your project to produce a human-readable documentation in web-page format.


# 5 Part IV: Scripting

## 5.1 Input parameters

Your script should take in two arguments: The path to a directory where to find images in the
uncompressed TGA file format, and the path to an output folder. Your script should then build the
tileexecutable, build up a list of all the image files found in the input folder, and send the paths
to all these image files, along with the output path, to thetileprogram.

Note: Although the image files will all be indexed properly (e.g. img1.tga,img2.tga,
etc.), there is no guarantee about the order in which the file system will return the list of files^1 , and
therefore...

## 5.2 Proper sorting

Your script should ensure that the image files are properly ordered on the list that it sends to the the
tileprogram. In particular, you should make sure thatimg12.tgacomesafterimg9.tga.

# 6 What to submit

## 6.1 The pieces

A complete submission will include:

- a separate source file for each of the image processing, operations;
- the version ofRasterImage.h,RasterImage.c,imageIOtga.h, andimageIOtga.c
    that you used to build your image operators;
- yourbashscript,
- the html documentation produced by Doxygen for your programs,
- a report.

Donotsubmit the image files andmain.cprogram of the handout.

## 6.2 Organization

You should submit a zip archive of a folder namedProg04containing

- your report,
- a directory namedDocumentationcontaining the html documentation produced by Doxy-
    gen,

(^1) A recent article on Ars Technica explains how this “feature” made Python scripts produce different results across
platforms, which may invalidate hundreds of research studies.


- a directory namedCodecontaining
    - your script,
    - a source file for each of the image operators,
    - a directory namedImageIOcontaining the version ofRasterImage.h,RasterImage.c
       ,imageIOtga.h, andimageIOtga.cthat you used to build your image opera-
       tors.

## 6.3 The report

The report should discuss your design decisions. If you had to interpret some of the specifications
of the assignment then state clearly what you did (definitely so if you decided to only implement a
subset of the requirements).
Identify any limitations of your programs (besides the obvious, e.g. the fact that you can only
handle TGA image files). Are there combinations of arguments that cause your program to crash?
Did you decide to only implement a subset of the requirements?
Finally, if you implemented any of the “extra credit” sections, make sure to point it out, so that
the graders know what to look for.

## 6.4 Grading

We are going to test your programs and script with different input files and lists of arguments.
Your grade for this section will reflect to what extent you produce the desired output for all the test
cases. So, make sure that you test your program and script before you submit.
Code with syntax errors (compiler errors for the C program) will get a grade of 0 for execution
and quality. Comment out the part with syntax errors and explain in the comments what you were
trying to do.

- Execution: 45 pts
    - C program: 30 pts
       ∗ tile: 10 pts,
       ∗ dispatcher: 15 pts,
    - bashscript: 15 pts
- Code quality: 25 pts
    - C program: 15 pts
    - bashscript: 10 pts
- Documentation produced by Doxygen: 10 pts
- Report: 10 pts
- Folder organization: 10 pts


