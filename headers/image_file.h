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
#include "palette.h"

typedef struct image_file_parameters_t image_file_parameters_t;

image_file_parameters_t* image_file_parameters_init(
            palette_bit_depth_e bit_depth,
            const palette_t* palette_p);

image_file_parameters_t* image_file_parameters_init_palette(
        const palette_t* palette_p,
        palette_index_method_e method);
void image_file_parameters_free_palette(image_file_parameters_t* parameters_p);

//Writes an image into a windows BITMAP stream.
void image_file_write(const char* name_p, const image_t* image_p,
                      const image_file_parameters_t* parameters_p);

//Returns the name for BMP file with the ".bmp" extension
char* file_name_extension_bmp(const char* input);

// Creates a string with a number appended to the end of the input.
char* file_name_extension_number(const char* input, int number);

#endif /* FILEUTIL_H_ */
