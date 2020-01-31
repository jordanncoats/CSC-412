## CSC 412 – Operating Systems

### Programming Assignment 03, Fall 2019

```
Thursday, October 3rd, 2019
```
Due date:Monday, October 14th, 11:55pm.

## 1 What this Assignment is About

### 1.1 Objectives

The objectives of this assignment are for you to

- Write C code making use of 1D and 2D arrays, and exploiting pointer algebra;
- Work with a small third-party library (here, to load and write TGA images);
- Write some simple image processing applications;
- Use a bash script to search through a directory;
- Start using Doxygen to produce documentation for your code.

This is an individual assignment.

### 1.2 Handout

Besides this document, the handout for this assignment is a zip archive containing the code of a
small library for loading and writing images in the TGA file format, a simple program demonstrat-
ing the use of the library on a sample image, and a set of images in the TGA file format.

## 2 Part I: TGA Image Files and the Code Handout

### 2.1 The TGA file format

The TGA (Targa file) image format is one of way too many image formats that you may run into.
Other formats provide better compression or support for a wider range of colors, so why use this
format at all? Because the format foruncompressed .tgaimage files is the only truly multi-
platform format that is easy to read and write. We could use a more complete library such as
freeimageorImageMagick, but, as long as your image file stores data uncompressed and
does not contain any comments, the code supplied here will do the job.


### 2.2 The code supplied

The code supplied consists of:

- RasterImage.his the header file that defines the (currently empty)RasterImagedata
    type, along with some enumerated types.
- imageIOTGA.handimageIOTGA.care respectively the header file and the source
    code for the implementation of functions to read and write color and gray-level images in
    the uncompressed TGA format.
- main.cis the code of a small program that shows how to use the library to read and write
    image files, and to access the pixel data of an image.

### 2.3 About the RasterImage.h header file

I want to attract your attention to a few important points regarding this header file.

First, if you are familiar with javadoc style of comments, you probably recognized the char-
acteristic/**. For many years, C++ programmers secretly envied their Java-programming col-
leagues who could produce good-looking code documentation through the javadoc system. They
envied them until a brave soul came up with Doxygen, which interprets javadoc-style comments
in a multitude of programming languages (C, C++, Python, Perl, etc.). Therefore, coding in C is
not a good excuse to have a lousy documentation anymore, and, starting with this assignment, you
will be expected to write javadoc-style comments and produce documentation in the form of html
files. I will return to this point later in this document.
Second, the twoenumtypes,ImageFileTypeandImageType, mention some types that
we won’t encounter in this assignment. We will only deal withkTGACOLORimages that we
will store asRGBA32RASTER. I might as well observe here that I love usingenumtypes to
represent and group constants that belong together. I already alluded to that in the C code samples
that I posted on Sakai at the beginning of the semester. Enumerated types work well with switch
statements, are easy to expand, and have all the advantages of an integral type (for calculations and
array indices) while offering some—limited—protection regarding range.
Next, theRasterImagestruct type is currently empty, and one of your first tasks will be to
define its various fields. All of what defines an image (dimensions, type of the data, pointer to the
raster, etc.) should be defined in this type. Please note that there is no need to store a file type,
because not all images are read from as file (for example, the images that yourcrop,rotate,
etc. operators produce). Furthermore, once the image has been loaded in a raster in memory, there
is no difference what its origin was.
Feel free to add any fields that you feel are necessary (but then you will need to justify your
choices in the report). For example, you may consider storing a “2D raster” array scaffolded on
top of the 1D raster (for extra credit).


## 2.4 Color images and rasters

Pretty much all libraries dealing with image and video data^1 manipulate image data under the form
of a 1D “raster,” as shown in Figure 1.

#### 0 1 2 3 4 5

# Theimageasweseeit

# Therasterthatstorestheimage

```
row 0 row 1 row 2 row 3 row 4 row 5
```
```
Figure 1: An image stored as a 1D raster.
```
So, if our image hasmrows ofncolumns and each pixel is stored onkbytes (for a total
ofn×kbytes per row andm×n×kbytes for the entire image), then the pixel at rowiand
columnjin the image can be addressed at indexi××k+j×kof the 1D raster.
In the uncompressed TGA file format, an image file stores each pixel as 3 unsigned bytes (range
0 to 255) encoding the red, green and blue color channels. When we load an image in memory,
however, it is more convenient to store each pixel on an even 4 bytes. Besides allowing image
processing and other graphic applications to use this additional byte to encodetransparency(or
the “αchannel,” as it is called), this also means that a pixel occupies the same space as anint, so

(^1) An important exception is Apple’s old QuickTime library and thelibquicktimeopen-source replication of
the API for a tiny bit of QuickTime. I mention this here because we may have an encounter withlibquicktime
before the end of the semester.


that we can view our 1D raster either as anint*pointing to a pixel or as anunsigned char*
pointing to a color channel of a pixel.
Note that different applications may store the bytes of the color channel in different orders. The
most common orders areargb(alpha, red, green, blue) andrgba. Throughout this assignment,
and future assignments dealing with images, we will use exclusively thergbaformat.
What this means is that if I want to access the color information of the pixel at rowiand
columnjof the samem×nimage, then I could get a pointer to this pixel’s color channels, each
seen as anunsigned charby writing

unsigned char* rgba = (unsigned char*)raster + 4*(i*n + j);

Now, I can view this pointer as an array and access directly my color components:

- rgba[0]: red channel,
- rgba[1]: green channel,
- rgba[2]: blue channel,
- rgba[3]: alpha channel.

Alternatively, I could see my pixel as a 4-byteintthat I access through anint*pointer by
writing

int pixel = *((int*)raster + i*n + j);

Now I can access my color channels by extracting the different bytes of thepixelvariable, using
the bitwise|and&operators, and bit shifting, as we saw in Lab 03. There is just a small problem:
the byte order. Different CPU architectures have a different way to encodeint,float, etc. Or
rather, the encoding is the same, but the byte order is different. Intel processors (and their AMD
clones) are “small-endian,” which means that their least significant byte is stored first, and their
most significant byte is stored last.

### 2.5 The demo program main.c

This small programs reads a TGA image into a 1D raster and performs a simple operation on the
raster data (“kill” one of the color channels). It then writes the resulting image as a new TGA
file. In other words, it demonstrates all the elementary operations that you will need to combine
to complete this assignment. In addition, I give two different versions of the operation. Make sure
that you understand well all parts of the code. Post questions on the forums.

### 2.6 Extra credit (6 points)

Modify the code ofRasterImage.h,RasterImage.c,imageIOTGA.h, andimageIOTGA.c
to implement the 2D raster functionality. Remember that this 2D raster does not constitute a dupli-
cation of the storage. It is just a scaffolding on top of the 1D raster giving you a more convenient
way to access the data of the 1D raster. Verify that your implementation is correct by using the
2D raster rather than the 1D raster to “kill” that color channel inmain.c.


## 3 Part II: Image Operators

You are going to implement several small programs that all perform a simple operation on an
image. These programs will all use the handout code provided to read and write images.

### 3.1 Report dimensions

This program nameddimensionstakes as main argument the path to a TGA image file and
reports the dimensions of the image. This program may be called with different optional modifiers
that affect the output of the program:

- -hOutput image height only;
- -wOutput image width only;
- -vverbose output.

The modifier-vmay be combined with-hor with-w, in either order, but the modifiers-hand
-ware mutually exclusive.
Assuming that the program is executed with the image filexxx.tgaprovided with the hand-
out (that image has a width of 640 and a height of 640), possible outputs would be:

- dimensions ../Images/xxx.tga→640 480
- dimensions -h ../Images/xxx.tga→ 480
- dimensions -w ../Images/xxx.tga→ 640
- dimensions -v ../Images/xxx.tga→width: 640, height: 480
- dimensions -v -h../Images/xxx.tga→height: 480
- dimensions -w -v../Images/xxx.tga→width: 640

Error handling:
Your program should report if the image is not found (invalid path) or cannot be read, or if invalid
optional modifiers are passed to the program.

### 3.2 Cropping

This program namedcroptakes as arguments the path to a valid TGA image file, the path to an
output directory, thexandycoordinates of a cropping rectangles within the image, and the width
and height of that cropping rectangle. It writes in the output folder a new image file that contains
the sub-image defined by the crop rectangle.
The name of the output image should be that of the input image, with the[cropped]suf-
fix. For example, if the input image is namedmyImage.tga, then the output image should be
myImage [cropped].tga.


Extra credit (up to 6 points)
Before writing the output inage, verify if a file with the same name already exists in the output
folder (2 points). If it does, add an index to the new output image ([cropped 2],[cropped 3],
etc.) for 3 points of extra credit. For an additional point, retroactively rename the first output image
[cropped 1]

### 3.3 Split color channels

This program namedsplittakes as arguments the path to a valid TGA image file and the
path to an output directory. It outputs in that folder the three image files obtained by separat-
ing the red, green, and blue channels of the input image. If the name of the input image was
myImage.tga, then the output images should be namedmyImager.tga,myImageg.tga,
andmyImageb.tga.

### 3.4 Extra credit (up to 6 points)

Before writing the output image, verify if a file with the same name already exists in the output
folder (2 points). If it does, add an index to the new output image (3 points). For example, when
writing a cropped version ofmyImage.tga, if the filemyImage [cropped].tgaalready
exists, name the new filemyImage [cropped 2].tga, unlessmyImage [cropped 2].tga
already exists, in which case name itmyImage [cropped 3].tga, etc. For an additional
point, retroactively modify the suffix of the first output image[cropped 1]

### 3.5 Image rotation

This program namedrotatetakes as arguments a string specifying the rotation to perform, the
path to a valid TGA image file, and the path to an output directory. It writes in the output folder a
new image file that contains the rotated image.
The string specifying the rotation can be one of the following:l(rotate left by 90 degree),r
(rotate right by 90 degree),llorrr(rotate by 180 degree).
The name of the output image should be that of the input image, with the[cropped]suf-
fix. For example, if the input inage is namedmyImage.tga, then the output image should be
myImage [cropped].tga.

Extra credit (up to 3 points)
For 2 points, accept the rotation specification string in uppercase or lowercase (or mixed). For an
additional point, take any string made up ofLorRand reduce it to the effective rotation spec-
ified. For example,LLRRRRRLRRLLR, which amounts to 6 left rotations and 8 right rotations,
effectively reduces toLLorRR.

### 3.6 Log files

Yourdimensions,crop,split, androtateprograms should maintain a log file of all the
commands that they have executed, with a report of possible errors. Be careful that you don’t want


the log of the last execution to overwrite the previous one, but to beaddedto them. This means
that when you open the log file to write into it, you shouldn’t do
fopen(fileName, ‘‘w’’)
but
fopen(fileName, ‘‘a’’)

## 4 What to Do, Part III: Scripting

### 4.1 Input parameters

Your script should take in two arguments: The path to a directory where to find images in the
uncompressed TGA file format, and the path to an output folder. The location of the script file
relative to the rest of the code is explained in Subsection 5.2.

### 4.2 Tasks to perform

Your script should first build the executable for each of the image processing programs, and then,
for each TGA file found in the input folder, use the appropriate image operator to produce in the
output folder:

- the images storing the division of the input image into four quadrants;
- three images storing the separate color channels of the input image;
- the result of a left rotation of the input image.

## 5 What to submit

### 5.1 The pieces

A complete submission will include:

- a separate source file for each of the image processing, operations;
- the version ofRasterImage.h,RasterImage.c,imageIOtga.h, andimageIOtga.c
    that you used to build your image operators;
- yourbashscript,
- the html documentation produced by Doxygen for your programs,
- a report.

Donotsubmit the image files andmain.cprogram of the handout.


### 5.2 Organization

You should submit a zip archive of a folder namedProg03containing

- your report,
- a directory namedDocumentationcontaining the html documentation produced by Doxy-
    gen,
- a directory namedCodecontaining
    - your script,
    - a source file for each of the image operators,
    - a directory namedImageIOcontaining the version ofRasterImage.h,RasterImage.c
       ,imageIOtga.h, andimageIOtga.cthat you used to build your image opera-
       tors.

### 5.3 The report

The report should discuss your design decisions. If you had to interpret some of the specifications
of the assignment then state clearly what you did (definitely so if you decided to only implement a
subset of the requirements).
Identify any limitations of your programs (besides the obvious, e.g. the fact that you can only
handle TGA image files). Are there combinations of arguments that cause your program to crash?
Did you decide to only implement a subset of the requirements?
Finally, if you implemented any of the “extra credit” sections, make sure to point it out, so that
the graders know what to look for.

5.3.1 Execution

We are going to test your programs and script with different input files and lists of arguments.
Your grade for this section will reflect to what extent you produce the desired output for all the test
cases. So, make sure that you test your program and script before you submit.

5.3.2 Code quality

For this part of the grade we look at things such as:

- Proper indentation;
- Good comments. This part of the grade does not cover the javadoc-style comments (they
    are evaluated in the Documentation section) but comments in the code, explaining important
    blocks of code. Comments should not be simple translations of the code in plain language
    (e.g.x +=2;being commented as “add 2 to x”).
- Good choice of identifiers (chances are thata,b, andcare not good identifiers, particularly
    if the first two are strings and the third one an integer. In any case, identifiers should clearly
    indicate the purpose of a variable or function.


- Consistent choice of identifiers. For example, if you have a variableint beanCountthat
    does what it claims to do, then it would be a bad idea to have another variableint numofboxes.
- Good implementation decision: avoid super long functions (at your current skill level, if
    your function occupies more than one page of your screen, it’s probably too long); select
    proper data types; create your own when appropriate.

Your grade for this section will reflect how much you actually implemented. A superbly com-
mented and indented “Hello World!” program won’t bring you many “code quality” points.

5.3.3 No syntax error

Code with syntax errors (compiler errors for the C program) will get a grade of 0 for execution
and quality. Comment out the part with syntax errors and explain in the comments what you were
trying to do.

### 5.4 Grading

- Execution: 45%
    - C program: 30%
       ∗ dimensions: 7%,
       ∗ split: 6%,
       ∗ rotate: 8%,
       ∗ crop: 9%.
    - bashscript: 15%
- Code quality: 25%
    - C program: 15%
    - bashscript: 10%
- Documentation produced by Doxygen: 10%
- Report: 10%
- Folder organization: 10%


