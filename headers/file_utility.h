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
    char magic_number[2];
    uint file_size;
    int app_signature;
    uint data_offset;

    //DIBHeader
    uint dib_header_size;
    uint width;
    uint height;
    uint depth;
    int compression;
    uint image_size;
    int res_x;
    int res_y;
    int color_number;
    int important_colours;
} bmp_header_t;


FILE* init_image_file(char* name, image_t* image);
void init_header(FILE* file, int width, int height);
void write_image(const image_t* image, FILE* imageFile);
int get_int(FILE* file);
int get_width(FILE* imageFile);
int get_height(FILE* imageFile);
char* bmp_extension(const char* input);
char* num_extension(const char* input, int number);

#endif /* FILEUTIL_H_ */
