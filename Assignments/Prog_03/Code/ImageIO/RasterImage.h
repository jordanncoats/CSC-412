/**
 * @file RasterImage.h
 * @author JYH with modifications by Jordan Coats
 * @date Oct 12 2019
 * @brief Header for RasterImage struct.
 */

#ifndef RASTER_IMAGE_H
#define RASTER_IMAGE_H

/**
 * @brief ImageFileType enum for the type of file.
 */

typedef enum ImageFileType
{
	kUnknownType = -1,
	kTGA_COLOR, /**< 24-bit color image. */
	kTGA_GRAY,
	kPPM, /**< 24-bit color image. */
	kPGM  /**< 8-bit gray-level image. */
} ImageFileType;

/**	
 * @brief This is the enum type that refers to images loaded in memory.
 * 
 *  Whether they were read from a file, played from a movie, captured from a live
 *	video stream, or the result of calculations.
 *	Feel free to edit and add types you need for your project.
 */
typedef enum ImageType
{
	RGBA32_RASTER, /**< Color image with 4 bytes per pixel. */
	GRAY_RASTER,   /**< Gray image with 1 byte per pixel. */
	FLOAT_RASTER   /**< Monochrome image (either gray or one color channel of a color image stored in a float raster. */

} ImageType;

/**	
 * @brief This struct was created by Jordan Coats
 * 
 * It is used to store the image that is read by imageIO_TGA.c then used for each program for the assignment
 */
typedef struct RasterImage
{
	unsigned int imgSize;   /**< stores the complete size (non negative) in the event the programs need to know what they're working with.. */
	unsigned char *theData; /**< char* theData to store all the RGBA data for the iamge. */
	unsigned int numCols;   /**< unsigned ints for numCols to get the dimensions of the image. */
	unsigned int numRows;   /**< unsigned ints for numRows to get the dimensions of the image. */
	int bytesPerPixel;		/**< int bytesPerPixel give how the data is structured (e.g. RGBA or otherwise). */
	int bytesPerRow;		/**< int bytesPerRow give how the data is structured per row. */
	ImageType theType;		/**< ImageType theType is to store the enum data from above. */
} RasterImage;

#endif ///	RASTER_IMAGE_H
