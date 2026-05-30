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

typedef enum
{
    PIXEL_BIT_DEPTH_1b  =  1,
    PIXEL_BIT_DEPTH_4b  =  4,
    PIXEL_BIT_DEPTH_8b  =  8,
    PIXEL_BIT_DEPTH_16b = 16,
    PIXEL_BIT_DEPTH_24b = 24,
    PIXEL_BIT_DEPTH_32b = 32,
} image_file_bit_depth_e;

union palette_element_u
{
    colour_t colour;
    uint32_t unused;
};

typedef struct
{
    uint32_t count;
    union palette_element_u* palette_p;
} colour_palette_t;

typedef struct image_file_parameters_t image_file_parameters_t;

image_file_parameters_t* image_file_parameters_init(
            image_file_bit_depth_e bitdepth,
            const colour_palette_t* palette_p);

//Writes an image into a windows BITMAP stream.
void image_file_write(const char* name_p, const image_t* image_p,
                      const image_file_parameters_t* parameters_p);

//Returns the name for BMP file with the ".bmp" extension
char* file_name_extension_bmp(const char* input);

// Creates a string with a number appended to the end of the input.
char* file_name_extension_number(const char* input, int number);

#endif /* FILEUTIL_H_ */
