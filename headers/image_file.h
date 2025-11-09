/*
 * fileUtil.h
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */

#ifndef FILEUTIL_H_
#define FILEUTIL_H_

#include <stdint.h>
#include <stdio.h>

#include "image.h"

typedef struct
{
    FILE* file_p;
    const char* file_name_p;
} image_file_t;

FILE* image_file_init(const char* name, image_t* image);

//Writes an image into a windows BITMAP stream.
void image_file_write(const image_t* image, FILE* imageFile);

//Returns an int from the current file.
int get_int(FILE* file);

//Gets the width of the BITMAP array from a WINDOWS BITMAP stream.
int get_width(FILE* imageFile);

//Gets the height of the BITMAP array from a WINDOWS BITMAP stream.
int get_height(FILE* imageFile);

//Returns the name for BMP file with the ".bmp" extension
char* bmp_extension(const char* input);

// Creates a string with a number appended to the end of the input.
char* num_extension(const char* input, int number);

#endif /* FILEUTIL_H_ */
