#!/bin/bash
#gets the user's image input/output
echo What is the image input directory?
read inputD
echo What is the image output directory?
read outputD

#This would be repeated for each file if they were working
gcc split.c ImageIO/imageIO_TGA.c -g -o "split.o"

for entry in $inputD/*.tga;
do
    echo $entry
    ./split.o "$entry" "$outputD"/
done