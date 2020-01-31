/**
 * @file split.c
 * @author Jordan Coats
 * @date Oct 12 2019
 * @brief Splits a TGA image into RGB channels.
 *
 * This program is made for CSC412 prog03 assignment, it will take an input and output argument
 * to split the TGA file into three TGA file that are all just one channel: Red, Blue, Green.
 */
/* -- Includes -- */

#include <stdlib.h>
#include <stdio.h>               /* for I/O */
#include <string.h>              /* for the filenames */
#include <libgen.h>              /* for basename function */
#include "ImageIO/imageIO_TGA.h" /* for TGA function */

/**
 @brief This is the main function.
 @ return nothing
 */
int main(int argc, char **argv)
{
    FILE *log;
    log = fopen("split.log", "a");
    /**
    * First the program will get the argv of the input and output. Then it will create the three structs for each channel.
    */
    char *input = argv[1];
    char *output = argv[2];
    printf("Start Split: \n");
    fprintf(log, "Start Log.\n");
    fprintf(log, "Reading %s...\n", input);
    struct RasterImage red;
    readTGA_(input, &red, log);
    struct RasterImage blue;
    readTGA_(input, &blue, log);
    struct RasterImage green;
    readTGA_(input, &green, log);
    /**
    * In order to get the filename we use basename to grab the last part of the input e.g. "image.TGA." But since we need to eliminate the .TGA file extension
    * we copy it over to the individual channel names without the last 4 chars. While doing this we set it up so the output directory is right for each.
    */

    char *fileName = basename(argv[1]);
    char *blueName = strndup(fileName, strlen(fileName) - 4);
    char *blueOut = strndup(output, strlen(output));
    char *redName = strndup(fileName, strlen(fileName) - 4);
    char *redOut = strndup(output, strlen(output));
    char *greenName = strndup(fileName, strlen(fileName) - 4);
    char *greenOut = strndup(output, strlen(output));

    /**
    * This double for loop will be repeated for each channel, in which it kills two channels by the position in RGBA.
    * The code for killing the channels is from the assignment's original main.c, just modified slightly.
    */
    for (unsigned int i = 0; i < blue.numRows; i++)
    {
        for (unsigned int j = 0; j < blue.numCols; j++)
        {
            /**
            *Kill Green
            */
            blue.theData[i * blue.bytesPerRow + 4 * j + 1] = 0x00;
            /**
                 *Kill Red
                 */
            blue.theData[i * blue.bytesPerRow + 4 * j] = 0x00;
        }
    }
    /**
    * Then we append the color tag and file extention, it is repeated for each channel but is done the same.
    */
    strcat(blueName, "_b.TGA");
    strcat(blueOut, blueName);
    printf("Writing... %s\n", blueOut);
    /**
    *Then each image will be written with the new name.
    */
    if (writeTGA_(blueOut, &blue, log))
    {
        /**
        * Warning if for whatever reason the image did not write to the directory.
        */
        fprintf(log, "Writing %s failed.\n", blueOut);
        printf("Writing %s failed.\n", blueOut);
    }

    for (unsigned int i = 0; i < green.numRows; i++)
    {
        for (unsigned int j = 0; j < green.numCols; j++)
        {
            /**
                 *Kill Red
                 */
            green.theData[i * green.bytesPerRow + 4 * j] = 0x00;
            /**
                 *Kill Blue
                 */
            green.theData[i * green.bytesPerRow + 4 * j + 2] = 0x00;
        }
    }

    strcat(greenName, "_g.TGA");
    strcat(greenOut, greenName);
    printf("Writing... %s\n", greenOut);
    if (writeTGA_(greenOut, &green, log))
    {
        fprintf(log, "Writing %s failed.\n", greenOut);
        printf("Writing %s failed.\n", greenOut);
    }

    for (unsigned int i = 0; i < red.numRows; i++)
    {
        for (unsigned int j = 0; j < red.numCols; j++)
        {
            /**
            *Kill Green
            */
            red.theData[i * red.bytesPerRow + 4 * j + 1] = 0x00;
            /**
            *Kill Blue
            */
            red.theData[i * red.bytesPerRow + 4 * j + 2] = 0x00;
        }
    }

    strcat(redOut, redName);
    strcat(redOut, "_r.TGA");
    printf("Writing... %s\n", redOut);
    if (writeTGA_(redOut, &red, log))
    {
        fprintf(log, "Writing %s failed.\n", redOut);
        printf("Writing %s failed.\n", redOut);
    }
    /**
    *Completion Message with timestamp and closing the file.
          */
    printf("Split Complete.\n");
    fprintf(log, "Output Directory: %s\n", output);
    fprintf(log, "Closing Log.\n");
    fclose(log);
    return 0;
}