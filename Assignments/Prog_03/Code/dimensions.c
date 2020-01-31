/**
 * @file dimensions.c
 * @author Jordan Coats
 * @date Oct 12 2019
 * @brief Returns a TGA image's dimensions.
 *
 * This program is made for CSC412 prog03 assignment, it will take an input and return
 * the width and height of an image.
 */
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

#include "ImageIO/imageIO_TGA.h"

/**
* @brief First function will be called for in the event that the user inputs the wrong commands for dimensions.c
*
* It will show the correct usage of the program so they don't have to look for documentation.
*/
void print_usage()
{
    printf("Usage: dimensions <input> | dimensions -h <input> | dimensions -w <input> | dimensions -v <input> | dimensions -v -h <input> | dimensions -v -w <input>\n");
    exit(2);
}

/**
* @brief Function for printing the height.
*/
void print_height(const struct RasterImage *object)
{
    printf("%d\n", object->numRows);
}
/**
*  @brief Function for printing the width.
*/
void print_width(const struct RasterImage *object)
{
    printf("%d\n", object->numCols);
}

int main(int argc, char **argv)
{
    FILE *log;
    log = fopen("dimensions.log", "a");
    /**
    * @brief Initializing the RasterImage struct as it is how we get the dimensions of the images easily.
    */
    struct RasterImage object;
    /**
    * @brief As this program always expects an argument after the argv[0] it will return the instructions
    * in the event it isn't given the correct amount.
    */
    if (argc < 2)
    {
        fprintf(log, "Error, too few arguments.\n");
        print_usage();
    }
    /**
    * @brief We use getopt to get the different arguments before the input directory.
    */
    int option;
    /**
    * @brief These flags enable the program to know which commands have run already so there won't be any conflicts.
    */
    int hflag = 0;
    int wflag = 0;
    int vflag = 0;
    fprintf(log, "Reading %s...\n", argv[argc - 1]);
    readTGA_(argv[argc - 1], &object, log);
    /**
    * @brief While loop to loop through the potential options
     */
    while ((option = getopt(argc, argv, "vwh")) != -1)
    {
        /**
        * @brief A switch statement for each of the potential options.
        * 
        * V is for verbose, when it triggers it flips the vFlag so that the program knows to use verbose language.
        * W and H are mutually exclusive and the program will alert the user to this if input together. Otherwise it flips flags.
        * Then a default catch all for anything else input to instead show the instructions.
        */
        switch (option)
        {
        case 'v':
            vflag++;
            fprintf(log, "v\n");
            break;
        case 'w':
            if (hflag)
            {
                printf("Modifiers -h and -w are mutually exclusive.\n");
                fprintf(log, "Error, -h and -w are mutually exclusive.\n");
                print_usage();
            }
            else
            {
                wflag++;
                fprintf(log, "w\n");
            }

            break;
        case 'h':
            if (wflag)
            {
                printf("Modifiers -w and -h are mutually exclusive.\n");
                fprintf(log, "Error, -h and -w are mutually exclusive.\n");
                print_usage();
            }
            else
            {
                hflag++;
                fprintf(log, "h\n");
            }

            break;
        default:
            fprintf(log, "Error, incorrect usage.\n");
            print_usage();
            break;
        }
    }
    /**
    * @brief This is where the program will display the outcome.
    *
    * If the vFlag is flipped it will know to use the verbose language.
    * Otherwise it just depends on which flags are flipped.
    */
    if (hflag == 0)
    {
        if (vflag)
        {
            printf("Width: ");
        }
        print_width(&object);
        fprintf(log, "Successful Width.\n");
    }
    if (wflag == 0)
    {
        if (vflag)
        {
            printf("Height: ");
        }
        print_height(&object);
        fprintf(log, "Successful Height.\n");
    }
    fprintf(log, "Log Closed.\n");
    fclose(log);
    return 0;
}