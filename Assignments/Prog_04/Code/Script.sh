#!/bin/bash

gcc tile.c /ImageIO/imageIO_TGA.c -o -lm "tile.o"


LIST=" "
for entry in $1/*.tga | sort -V;
do
    echo $entry
    LIST+="${entry} "
done

./tile.o $LIST $2
echo LIST