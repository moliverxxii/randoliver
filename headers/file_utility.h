/*
 * fileUtil.h
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */

#ifndef FILEUTIL_H_
#define FILEUTIL_H_

#include "main.h"

#define HEADER_SIZE 54

typedef struct __attribute__((packed))
{
    //FormatHeader
    char     magic_number[2];
    uint32_t file_size;
    uint32_t app_signature;
    uint32_t data_offset;

    //DIBHeader
    uint32_t dib_header_size;
    uint32_t width;
    uint32_t height;
    uint32_t depth;
    uint32_t compression;
    uint32_t image_size;
    uint32_t res_x;
    uint32_t res_y;
    uint32_t color_number;
    uint32_t important_colours;
} bmp_header_t;


FILE* init_image_file(char* name, image_t* image);
void init_header(FILE* file, int width, int height);

//Writes an image into a windows BITMAP stream.
void write_image(const image_t* image, FILE* imageFile);

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
