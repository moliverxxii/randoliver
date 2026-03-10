/*
 * fileUtil.c
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */

#include <stdlib.h>
#include <string.h>

#include "image_file.h"

static const uint8_t BMP_PLANE_COUNT = 1;
static const uint8_t BITS_PER_BYTE = 8;

#define BITMAP_INFO_HEADER

typedef struct
{
    char* file_name_p;
    FILE* file_p;
} image_file_t;

typedef struct __attribute__((packed))
{
    uint32_t header_size;
    uint16_t width;
    uint16_t height;
    uint16_t planes;
    uint16_t bit_count;
} bitmap_core_header;

typedef struct __attribute__((packed))
{
    uint32_t header_size;
    uint32_t width;
    int32_t  height; //>0 bottom-top, <0 top-bottom on ignore ca de toute facon
    uint16_t planes;
    uint16_t bit_count;
    uint32_t compression;
    uint32_t image_size;
    uint32_t resolution_x; // pixel/m
    uint32_t resolution_y; // pixel/m
    uint32_t colour_number;
    uint32_t important_colours;
} bitmap_info_header;


typedef struct __attribute__((packed))
{
    //FormatHeader
    char     magic_number[2];
    uint32_t file_size;
    uint32_t app_signature;
    uint32_t data_offset;

#ifndef BITMAP_INFO_HEADER
    bitmap_core_header bitmap_header;
#else
    bitmap_info_header bitmap_header;
#endif // !BITMAP_INFO_HEADER
} bmp_file_header_t;

const size_t ROW_PADDING = sizeof(uint32_t);
const size_t HEADER_SIZE = sizeof(bmp_file_header_t);
const char BMP_GENERATOR_SIGNATURE[sizeof(uint32_t)] = {'m', 'o', '2', '2'};

static void image_file_init_header(FILE* file, uint32_t width, uint32_t height);

static image_file_t* image_file_init(const char* name);
static void image_file_free(image_file_t* image_file_p);


image_file_t*
image_file_init(const char* name_p)
{
    char* image_name_p = malloc((strlen(name_p) + 1)* sizeof(char));
    strcpy(image_name_p, name_p);


    char* file_name_p = file_name_extension_bmp(name_p);

    if(!file_name_p)
    {
    	printf("file name error\n");
        return NULL;
    }

    FILE* file_p = fopen(file_name_p, "wb+");
    free(file_name_p);
    file_name_p = NULL;

    if(!file_p)
    {
        printf("image file error\n");
        free(image_name_p);
        return NULL;
    }

    image_file_t image_file = {image_name_p, file_p};

    image_file_t* image_file_p = malloc(sizeof(image_file_t));
    *image_file_p = image_file;

    return image_file_p;
}

void
image_file_free(image_file_t* image_file_p)
{
    free(image_file_p->file_name_p);
    fclose(image_file_p->file_p);
    free(image_file_p);
}


void
image_file_write(const char* name_p, const image_t* image_p)
{
    image_file_t* image_file_p = image_file_init(name_p);
    fseek(image_file_p->file_p, HEADER_SIZE, SEEK_SET);
    image_file_init_header(image_file_p->file_p, image_width(image_p), image_height(image_p));
    for(uint32_t y = image_height(image_p); y>0; --y)
    {
        fwrite(image_row(image_p, y-1),
               sizeof(colour_t),
               image_width(image_p),
               image_file_p->file_p);
        uint8_t pad[3] = {0, 0, 0};
        uint8_t pad_length = (ROW_PADDING - (sizeof(colour_t) * image_width(image_p)) % ROW_PADDING) % ROW_PADDING;
        fwrite(pad, sizeof(uint8_t), pad_length, image_file_p->file_p);
    }
    image_file_free(image_file_p);
}

char*
file_name_extension_bmp(const char* input)
{
    static const char* const extension_p = ".bmp";
    char* output = malloc(strlen(input) + strlen(extension_p) + 1);
    strcpy(output, input);
    strcat(output, extension_p);
    return output;
}

char*
file_name_extension_number(const char* input, int number)
{
    static const char* const separator_p = " ";
    char number_string[12] = {0};
    sprintf(number_string,"%d", number);

    char* output = malloc(strlen(input)
                        + strlen(separator_p)
                        + strlen(number_string)
                        + 1);

    strcpy(output, input);
    strcat(output, separator_p);
    strcat(output, number_string);
    return output;
}

static void
image_file_init_header(FILE* file, uint32_t width, uint32_t height)
{
    fseek(file, 0, SEEK_SET);
    bmp_file_header_t header =
    {
        {'B', 'M'},
        HEADER_SIZE + COLOUR_COUNT * width * height,
        *(uint32_t*) BMP_GENERATOR_SIGNATURE,
        HEADER_SIZE,

        {
#ifndef BITMAP_INFO_HEADER
            sizeof(bitmap_core_header),
#else
            sizeof(bitmap_info_header),
#endif // !BITMAP_INFO_HEADER
            width,
            height,
            BMP_PLANE_COUNT,
            COLOUR_COUNT*BITS_PER_BYTE,
#ifdef BITMAP_INFO_HEADER
            0,
            0,
            RES,
            RES,
            0,
            0
#endif // BITMAP_INFO_HEADER
        }
    };
    fwrite(&header, sizeof(header), 1, file);
}
