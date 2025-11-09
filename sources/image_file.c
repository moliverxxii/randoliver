/*
 * fileUtil.c
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */

#include <stdlib.h>
#include <string.h>

#include "image_file.h"


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

const size_t ROW_PADDING = sizeof(uint32_t);
const size_t HEADER_SIZE = sizeof(bmp_header_t);
const char BMP_GENERATOR_SIGNATURE[sizeof(uint32_t)] = {'m', 'o', '2', '2'};

static void image_file_init_header(FILE* file, int width, int height);

FILE*
image_file_init(const char* name, image_t* image)
{
    //FILE CREATION AND NAME
    char* file_name = bmp_extension(name);
    if(!file_name)
    {
    	printf("file name error\n");
    }
    FILE* image_file = fopen(file_name, "wb+");
    if(!image_file)
    {
    	printf("image file error\n");
    }


    //DATA
    image_file_write(image, image_file);
    return image_file;
}


void
image_file_write(const image_t* image, FILE* image_file)
{
    fseek(image_file, HEADER_SIZE, SEEK_SET);
    image_file_init_header(image_file, image->width, image->height);
    for(uint32_t y = 0; y<image->height; ++y)
    {
        fwrite(image->image[y],
               sizeof(colour_t),
               image->width,
               image_file);
        uint8_t pad[3] = {0, 0, 0};
        uint8_t pad_length = (ROW_PADDING - (sizeof(colour_t) * image->width) % ROW_PADDING) % ROW_PADDING;
        fwrite(pad, sizeof(uint8_t), pad_length, image_file);
    }
}

int
get_int(FILE* file)
{
    int output = 0;
    fread(&output, sizeof(int), 1, file);
    return output;
}

int
get_width(FILE* image_file)
{
    fseek(image_file, 18, SEEK_SET);
    return get_int(image_file);
}

int
get_height(FILE* image_file)
{
    fseek(image_file, 22, SEEK_SET);
    return get_int(image_file);
}

char*
bmp_extension(const char* input)
{
    char* output = NULL;
    int length = strlen(input);
    length += 4;
    output = malloc(length);
    strcpy(output, input);
    strcat(output, ".bmp");
    return output;
}

char*
num_extension(const char* input, int number)
{
    char* output = NULL;
    int length = strlen(input);
    char empty[0x100] = {0};
    length += 2 + sprintf(empty,"%d",number);
    output = malloc(length);
    sprintf(output, "%s %d", input, number);
    return output;
}

static void
image_file_init_header(FILE* file, int width, int height)
{
    fseek(file, 0, SEEK_SET);
    bmp_header_t header =
    {
            {0x42, 0x4D},
            HEADER_SIZE + COLOUR_COUNT * width * height,
            *(uint32_t*) BMP_GENERATOR_SIGNATURE,
            HEADER_SIZE,

            40,
            width,
            height,
            0x00180001,
            0,
            3 * width * height,
            RES,
            RES,
            0,
            0
    };
    fwrite(&header, sizeof(header), 1, file);
}
