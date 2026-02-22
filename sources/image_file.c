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

static void image_file_init_header(FILE* file, uint32_t width, uint32_t height);

image_file_t*
image_file_init(const char* name_p, image_t* image_p)
{
    char* image_name_p = malloc((strlen(name_p) + 1)* sizeof(char));
    strcpy(image_name_p, name_p);


    char* file_name_p = bmp_extension(name_p);

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

    image_file_write(image_file_p, image_p);
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
image_file_write(image_file_t* image_file_p , const image_t* image_p)
{
    fseek(image_file_p->file_p, HEADER_SIZE, SEEK_SET);
    image_file_init_header(image_file_p->file_p, image_width(image_p), image_height(image_p));
    for(uint32_t y = 0; y<image_height(image_p); ++y)
    {
        fwrite(image_row(image_p, y),
               sizeof(colour_t),
               image_width(image_p),
               image_file_p->file_p);
        uint8_t pad[3] = {0, 0, 0};
        uint8_t pad_length = (ROW_PADDING - (sizeof(colour_t) * image_width(image_p)) % ROW_PADDING) % ROW_PADDING;
        fwrite(pad, sizeof(uint8_t), pad_length, image_file_p->file_p);
    }
}

char*
bmp_extension(const char* input)
{
    char* output = NULL;
    int length = strlen(input);
    length += 4;
    output = malloc(length+1);
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
image_file_init_header(FILE* file, uint32_t width, uint32_t height)
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
