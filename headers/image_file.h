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

//Writes an image into a windows BITMAP stream.
void image_file_write(const char* name_p, const image_t* image_p);

//Returns the name for BMP file with the ".bmp" extension
char* file_name_extension_bmp(const char* input);

// Creates a string with a number appended to the end of the input.
char* file_name_extension_number(const char* input, int number);

#endif /* FILEUTIL_H_ */
