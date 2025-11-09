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
    char* file_name_p;
    FILE* file_p;
} image_file_t;

image_file_t* image_file_init(const char* name, image_t* image);
void image_file_free(image_file_t* image_file_p);
//Writes an image into a windows BITMAP stream.
void image_file_write(image_file_t* image_file_p , const image_t* image_p);

//Returns the name for BMP file with the ".bmp" extension
char* bmp_extension(const char* input);

// Creates a string with a number appended to the end of the input.
char* num_extension(const char* input, int number);

#endif /* FILEUTIL_H_ */
