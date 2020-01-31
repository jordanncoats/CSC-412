
//
//  tile.c
//  Prog 04
//
//  Created by Jordan Coats on 11/1/2019.
//
//  Compile: gcc tile.c ImageIO/imageIO_TGA.c -o tile.o -lm
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "ImageIO/imageIO_TGA.h"

int main(int argc, char **argv)
{

#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark Argument count, Square, Outpath
//-------------------------------------------------------------------
#endif

    /** Counts the args minus the first (name) and last (output path).
     *  Stores the output path.
     *  Square created from the number of images to be the "most square." 
     */

    int arg_count = argc - 2;
    char *outFolderPath = argv[argc - 1];
    int square = (int)ceil(sqrt(arg_count));

#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark First Image Struct
//-------------------------------------------------------------------
#endif

    /** First struct. 
    */

    ImageStruct first;
    readTGAImage(argv[1], &first);

    ImageStruct tile;
    createTGAImage(&tile, first.numCols * square, first.numRows * square);

    /** Write first image into the tile. 
    */

    int *rasterIn = (int *)(first.raster);
    int *rasterOut = (int *)(tile.raster);
    for (unsigned i = 0; i < first.numRows; i++)
    {
        for (unsigned j = 0; j < first.numCols; j++)
            rasterOut[j + tile.numCols * i] =
                rasterIn[j + first.numCols * i];
    }
    /** tile_x and tile_y are offsets used for placing the images in the correct tile section. 
    */
    int tile_x = first.numCols, tile_y = 0;

    /** Delete first as it is no longer needed. 
    */
    freeTGAImage(&first);

#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark Loop for images
//-------------------------------------------------------------------
#endif

    for (int k = 2; k < argc - 1; k++)
    {
        /** Loops skipping the first two args until next to the last 
        */

        char *inputImagePath = argv[k];
        ImageStruct image;
        readTGAImage(inputImagePath, &image);
        int *rasterIn = (int *)(image.raster);
        int *rasterOut = (int *)(tile.raster);
        for (unsigned i = 0; i < image.numRows; i++)
        {
            for (unsigned j = 0; j < image.numCols; j++)
                rasterOut[j + tile_x + tile.numCols * (i + tile_y)] =
                    rasterIn[j + image.numCols * i];
        }
        /** Move the offset with each write 
        */
        tile_x += image.numCols;
        if (tile_x >= tile.numCols)
        {
            tile_x = 0;
            tile_y += image.numRows;
        }
        /** Delete image to be used for the next loop 
        */
        freeTGAImage(&image);
    }

#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark Writing Output
//-------------------------------------------------------------------
#endif

    const char *path = outFolderPath;
    writeTGAImage(path, &tile);
    return 0;
}