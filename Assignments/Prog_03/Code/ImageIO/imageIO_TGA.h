/**
 * @file imageIO_TGA.h
 * @author JYH with modifications by Jordan Coats
 * @date Oct 12 2019
 * @brief Headerfile for imageIO_TGA.c
 *
 * This program is made for CSC412 prog03 assignment.
 * 
 */

#ifndef IMAGE_IO_TGA_H
#define IMAGE_IO_TGA_H

#include "RasterImage.h"

void readTGA_(const char *fileName, struct RasterImage *, FILE *log);
int writeTGA_(const char *fileName, const struct RasterImage *, FILE *log);
#endif
