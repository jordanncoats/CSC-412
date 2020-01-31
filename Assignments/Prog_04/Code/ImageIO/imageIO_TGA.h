#ifndef	IMAGE_IO_TGA_H
#define	IMAGE_IO_TGA_H

#include "RasterImage.h"

typedef	struct ImageStruct {
	unsigned char* raster;
	unsigned int numCols;
	unsigned int numRows;
	int bytesPerPixel;
	int bytesPerRow;
} ImageStruct;

// read a TGA image file and store data in img output data structure
int readTGAImage(const char* path, ImageStruct* img);

// write a TGA image file to the disk using the information contained in the img data structure
int writeTGAImage(const char* path, const ImageStruct* img);

// allocate an empty image such that RGBA are set to zero
int createTGAImage(ImageStruct* img, unsigned int width, unsigned int height);

// free data for TGA image
void freeTGAImage(ImageStruct* img);

#endif	//	IMAGE_IO_TGA_H
