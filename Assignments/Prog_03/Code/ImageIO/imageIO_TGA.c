/*----------------------------------------------------------------------------------+
|	This is a modified version of the so-called "Lighthouse Library" for reading	|
|	images encoded in the *uncompressed, uncommented .tga (TARGA file) format. 		|
|	I had been using and modifying this code for years, simply mentioning			|
|	"Source Unknown" in my comments when I finally discovered, thanks to Web		|
|	searches, the origin of this code.  By then it had been adapted to work along	|
|	with reader/writer code for another image file format: the awful PPM/PBM/PGM	|
|	format of obvious Unix origin.													|
|	This is just to say that I am not claiming authorship of this code.  I am only	|
|	distributing it in this form instead of the original because (1) I have long	|
|	lost the original code, and (2) This version works with images organized		|
|	nicely into a struct.															|
|																					|
|	Jean-Yves Hervé		Dept. of Computer Science and Statistics, URI				|
|						2019-10-03													|
+----------------------------------------------------------------------------------*/
/**
 * @file imageIO_TGA.c
 * @author JYH with modifications by Jordan Coats
 * @date Oct 12 2019
 * @brief Read/Write TGA files.
 *
 * This program is made for CSC412 prog03 assignment, 
 * This code is mostly original, just modified to use the rasterImage struct
 */

#include <stdlib.h>
#include <stdio.h>

#include "imageIO_TGA.h"

void swapRGBA_(struct RasterImage *);
/**
*	Utility function for memory swapping
*	Used because TGA stores the RGB data in reverse order (BGR)
*/
void swapRGB_(struct RasterImage *object)
{
	unsigned int imgSize = object->numRows * object->numCols;

	for (unsigned int k = 0; k < object->imgSize; k++)
	{
		unsigned char tmp = object->theData[k * 3 + 2];
		object->theData[k * 3 + 2] = object->theData[k * 3];
		object->theData[k * 3] = tmp;
	}
}

void swapRGBA_(struct RasterImage *object)
{
	unsigned int imgSize = object->numRows * object->numCols;

	for (unsigned int k = 0; k < object->imgSize; k++)
	{
		unsigned char temp = object->theData[k * 4 + 2];
		object->theData[k * 4 + 2] = object->theData[k * 4];
		object->theData[k * 4] = temp;
	}
}

void readTGA_(const char *fileName, struct RasterImage *object, FILE *log)
{
	unsigned char *ptr;
	char head[18];
	FILE *tga_in;
	ImageFileType fileType;

	//printf("Opening\n");

	/* --- open TARGA input file ---------------------------------- */
	tga_in = fopen(fileName, "rb");

	if (tga_in == NULL)
	{
		fprintf(log, "Error, cannot open image\n");
		printf("Cannot open image file\n");
		exit(1);
	}

	/* --- Read the header (TARGA file) --- */
	fread(head, sizeof(char), 18, tga_in);
	/* Get the size of the image */
	object->numCols = (((unsigned int)head[12] & 0xFF) | (unsigned int)head[13] * 256);
	object->numRows = (((unsigned int)head[14] & 0xFF) | (unsigned int)head[15] * 256);

	if ((head[2] == 2) && (head[16] == 24))
		object->theType = RGBA32_RASTER;
	else if ((head[2] == 3) && (head[16] == 8))
		object->theType = GRAY_RASTER;
	else
	{
		fprintf(log, "Error, unsuported TGA image.\n");
		printf("Unsuported TGA image: ");
		printf("Its type is %d and it has %d bits per pixel.\n", head[2], head[16]);
		printf("The image must be uncompressed while having 8 or 24 bits per pixel.\n");
		fclose(tga_in);
		exit(2);
	}

	object->imgSize = object->numCols * object->numRows;
	/* Create the buffer for image */

	if (object->theType == RGBA32_RASTER)
		object->theData = (unsigned char *)malloc(object->imgSize * 4);
	else
		object->theData = (unsigned char *)malloc(object->imgSize);

	if (object->theData == NULL)
	{
		fprintf(log, "Error, Unable to allocate memory.\n");
		printf("Unable to allocate memory\n");
		fclose(tga_in);
		exit(3);
	}

	/* Check if the image is vertically mirrored */
	if (object->theType == RGBA32_RASTER)
	{
		if (head[17] & 0x20)
		{
			ptr = object->theData + object->imgSize * 3 - ((object->numCols) * 3);
			for (unsigned int i = 0; i < object->numRows; i++)
			{
				fread(ptr, 3 * sizeof(char), object->numCols, tga_in);
				ptr -= (object->numCols) * 3;
			}
		}
		else
		{
			unsigned char *dest = object->theData;
			for (unsigned int i = 0; i < object->numRows; i++)
			{
				for (unsigned int j = 0; j < object->numCols; j++)
				{
					fread(dest, 3 * sizeof(char), 1, tga_in);
					dest += 4;
				}
			}
		}

		/**
		 *   tga files store color information in the order B-G-R
		 *  we need to swap the Red and Blue components
		 */

		swapRGBA_(object);
	}
	else
	{
		if (head[17] & 0x20)
		{
			ptr = object->theData + object->imgSize - object->numCols;
			for (unsigned int i = 0; i < object->numRows; i++)
			{
				fread(ptr, sizeof(char), object->numCols, tga_in);
				ptr -= object->numCols;
			}
		}
		else
			fread(object->theData, sizeof(char), object->imgSize, tga_in);
	}
	if (object->theData != NULL)
	{
		switch (object->theType)
		{
		case RGBA32_RASTER:
		case FLOAT_RASTER:
			object->bytesPerPixel = 4;
			break;

		case GRAY_RASTER:
			object->bytesPerPixel = 1;
			break;
		}

		object->bytesPerRow = object->bytesPerPixel * object->numCols;
	}
	else
	{
		fprintf(log, "Error, reading of image file failed.\n");
		printf("Reading of image file failed.\n");
		exit(-1);
	}
	fclose(tga_in);
}

int writeTGA_(const char *fileName, const struct RasterImage *object, FILE *log)
{
	long offset;
	//int		swapflag = 1;
	char head[18];
	FILE *tga_out;

	/* --- open TARGA output file ---------------------------------- */

	tga_out = fopen(fileName, "wb");

	if (!tga_out)
	{
		fprintf(log, "Error, annot create pixel file %s \n", fileName);
		printf("Cannot create pixel file %s \n", fileName);
		return 1;
	}

	/**
	 *  --- create the header (TARGA file) ---
	 */
	head[0] = 0;							 // ID field length.
	head[1] = 0;							 // Color map type.
	head[2] = 2;							 // Image type: true color, uncompressed.
	head[3] = head[4] = 0;					 // First color map entry.
	head[5] = head[6] = 0;					 // Color map lenght.
	head[7] = 0;							 // Color map entry size.
	head[8] = head[9] = 0;					 // Image X origin.
	head[10] = head[11] = 0;				 // Image Y origin.
	head[13] = (char)(object->numCols >> 8); // Image width.
	head[12] = (char)(object->numCols & 0x0FF);
	head[15] = (char)(object->numRows >> 8); // Image height.
	head[14] = (char)(object->numRows & 0x0FF);
	head[16] = 24; // Bits per pixel.
	head[17] = 0;  // Image descriptor bits ;
	fwrite(head, sizeof(char), 18, tga_out);

	for (unsigned int i = 0; i < object->numRows; i++)
	{
		offset = i * 4 * object->numCols;
		for (unsigned int j = 0; j < object->numCols; j++)
		{
			fwrite(&object->theData[offset + 2], sizeof(char), 1, tga_out);
			fwrite(&object->theData[offset + 1], sizeof(char), 1, tga_out);
			fwrite(&object->theData[offset], sizeof(char), 1, tga_out);
			offset += 4;
		}
	}

	fclose(tga_out);

	return 0;
}