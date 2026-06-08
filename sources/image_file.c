/*
 * fileUtil.c
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */

#include <stdlib.h>
#include <string.h>

#include "image_file.h"
#include "utility.h"

static const uint8_t BMP_PLANE_COUNT = 1;
static const uint8_t BITS_PER_BYTE = 8;

#define BITMAP_INFO_HEADER

typedef struct image_file_parameters_t
{
    const palette_t* palette_p;
    palette_bit_depth_e bit_depth;
    palette_index_method_e palette_method;
} image_file_parameters_t;

typedef struct
{
    char* file_name_p;
    FILE* file_p;
    const palette_t* palette_p;
    palette_bit_depth_e bit_depth;
    palette_index_method_e palette_method;
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

union palette_element_u
{
    colour_t colour;
    uint32_t unused;
};

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

const size_t HEADER_SIZE = sizeof(bmp_file_header_t);
const char* const BMP_GENERATOR_SIGNATURE = "mo22";

static image_file_t* image_file_init(const char* name_p, const image_file_parameters_t* parameters_p);
static void image_file_free(image_file_t* file_p);

static void image_file_seek_header(image_file_t* file_p);
static void image_file_seek_palette(image_file_t* file_p);
static void image_file_seek_bitmap(image_file_t* file_p);

static void image_file_write_header(image_file_t* file_p, uint32_t width, uint32_t height);
static void image_file_write_next_row(image_file_t* file_p, const row_t row, uint32_t width);
//danger! ne marche que si le nombre de bit par pixel divise 8.
static size_t image_file_write_row_palette(FILE* file_p, const row_t row, uint32_t width, const palette_t* palette_p, palette_index_method_e method);
static void image_file_write_palette(image_file_t* file_p);
static void image_file_write_file_size(image_file_t* file_p);

image_file_parameters_t*
image_file_parameters_init_palette(const palette_t* palette_p, palette_index_method_e method)
{
    image_file_parameters_t parameters =
    {
        palette_p,
        palette_get_bits_per_colour(palette_p),
        method
    };
    image_file_parameters_t* parameters_p = malloc(sizeof(image_file_parameters_t));
    if(parameters_p != NULL)
    {
        *parameters_p = parameters;
    }
    return parameters_p;
}

void
image_file_parameters_free_palette(image_file_parameters_t* parameters_p)
{
    free(parameters_p);
}

void
image_file_write(const char* name_p, const image_t* image_p,
                 const image_file_parameters_t* parameters_p)
{
    image_file_t* image_file_p = image_file_init(name_p, parameters_p);
    image_file_write_header(image_file_p, image_width(image_p), image_height(image_p));
    if(image_file_p->palette_p != NULL)
    {
        image_file_write_palette(image_file_p);
    }
    image_file_seek_bitmap(image_file_p);
    for(uint32_t y = image_height(image_p); y>0; --y)
    {
        image_file_write_next_row(image_file_p, image_row(image_p, y-1), image_width(image_p));
    }
    image_file_write_file_size(image_file_p);
    image_file_free(image_file_p);
}

char*
file_name_extension_bmp(const char* name_p)
{
    static const char* const extension_p = ".bmp";
    char* output = malloc(strlen(name_p) + strlen(extension_p) + 1);
    strcpy(output, name_p);
    strcat(output, extension_p);
    return output;
}

char*
file_name_extension_number(const char* name_p, int number)
{
    static const char* const separator_p = " ";
    char number_string[12] = {0};
    sprintf(number_string,"%d", number);

    char* output = malloc(strlen(name_p)
                        + strlen(separator_p)
                        + strlen(number_string)
                        + 1);

    strcpy(output, name_p);
    strcat(output, separator_p);
    strcat(output, number_string);
    return output;
}

static image_file_t*
image_file_init(const char* name_p, const image_file_parameters_t* parameters_p)
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

    image_file_parameters_t parameters =
    {
        NULL,
        PIXEL_BIT_DEPTH_24b,
        PALETTE_INDEX_METHOD_RGB
    };

    if(parameters_p != NULL)
    {
        parameters = *parameters_p;
    }

    image_file_t image_file =
    {
            image_name_p,
            file_p,
            parameters.palette_p,
            parameters.bit_depth,
            parameters.palette_method
    };

    image_file_t* image_file_p = malloc(sizeof(image_file_t));
    *image_file_p = image_file;

    return image_file_p;
}

static void
image_file_free(image_file_t* file_p)
{
    free(file_p->file_name_p);
    fclose(file_p->file_p);
    free(file_p);
}

static void
image_file_seek_header(image_file_t* file_p)
{
    fseek(file_p->file_p, 0, SEEK_SET);
}

static void
image_file_seek_palette(image_file_t* file_p)
{
    fseek(file_p->file_p, HEADER_SIZE, SEEK_SET);
}

static void
image_file_seek_bitmap(image_file_t* file_p)
{
    image_file_seek_header(file_p);
    bmp_file_header_t header;
    fread(&header, sizeof(header), 1, file_p->file_p);
    fseek(file_p->file_p, header.data_offset, SEEK_SET);
}


static void
image_file_write_header(image_file_t* file_p, uint32_t width, uint32_t height)
{
    size_t   palette_size   = 0;
    uint8_t  bits_per_pixel = sizeof(colour_t) * BITS_PER_BYTE;
    uint32_t colour_count   = 0;
    if(file_p->palette_p != NULL)
    {
        palette_size   = colour_count * sizeof(union palette_element_u);
        bits_per_pixel = palette_get_bits_per_colour(file_p->palette_p);
        colour_count   = palette_count(file_p->palette_p);
    }

    bmp_file_header_t header =
    {
        {'B', 'M'},
        0, //ecrit a la toute fin.
        *(uint32_t*) BMP_GENERATOR_SIGNATURE,
        HEADER_SIZE + palette_size,
        {
#ifndef BITMAP_INFO_HEADER
            sizeof(bitmap_core_header),
#else
            sizeof(bitmap_info_header),
#endif // !BITMAP_INFO_HEADER
            width,
            height,
            BMP_PLANE_COUNT,
            bits_per_pixel,
#ifdef BITMAP_INFO_HEADER
            0,
            0,
            RES,
            RES,
            colour_count,
            0
#endif // BITMAP_INFO_HEADER
        }
    };

    image_file_seek_header(file_p);
    fwrite(&header, sizeof(header), 1, file_p->file_p);
}

#define ROW_PADDING sizeof(uint32_t)

static void
image_file_write_next_row(image_file_t* file_p, const row_t row, uint32_t width)
{
    size_t row_size = 0;
    switch(file_p->bit_depth)
    {
    case PIXEL_BIT_DEPTH_24b:
    default:
        row_size = sizeof(colour_t)* fwrite(row,
                                            sizeof(colour_t),
                                            width,
                                            file_p->file_p);
        break;
    case PIXEL_BIT_DEPTH_1b:
    case PIXEL_BIT_DEPTH_4b:
    case PIXEL_BIT_DEPTH_8b:
        row_size = image_file_write_row_palette(file_p->file_p, row, width,
                                                file_p->palette_p, file_p->palette_method);
        break;
    }

    uint8_t pad[ROW_PADDING - 1] = {0, 0, 0};
    uint8_t pad_length = ROW_PADDING * ((row_size + (ROW_PADDING - 1)) / ROW_PADDING)  - row_size;
    fwrite(pad, sizeof(uint8_t), pad_length, file_p->file_p);
}

static size_t
image_file_write_row_palette(FILE* file_p, const row_t row, uint32_t width, const palette_t* palette_p, palette_index_method_e method)
{
    size_t  row_size = 0;
    uint8_t byte     = 0;
    uint8_t bit      = 0;
    palette_bit_depth_e bit_depth = palette_get_bits_per_colour(palette_p);
    for(uint32_t x = 0; x < width; ++x)
    {
        byte |= palette_index_get(palette_p, row[x],
                method);

        if(bit == BITS_PER_BYTE - bit_depth)
        {
            row_size += sizeof(byte) * fwrite(&byte, sizeof(byte), 1, file_p);
            byte = 0;
        }
        else
        {
            byte <<= bit_depth;
        }
        bit = (bit + bit_depth) % BITS_PER_BYTE;
    }

    if(bit > 0)
    {
        byte <<= BITS_PER_BYTE - bit;
        row_size += sizeof(byte) * fwrite(&byte, sizeof(byte), 1, file_p);
    }
    return row_size;
}


static void
image_file_write_palette(image_file_t* file_p)
{
    image_file_seek_palette(file_p);
    palette_index_t colour_count = palette_count(file_p->palette_p);
    union palette_element_u* table = calloc(colour_count, sizeof(union palette_element_u));
    for(palette_index_t colour = 0; colour < colour_count; ++colour)
    {
        table[colour].colour = *palette_colour_get(file_p->palette_p, colour);
    }
    fwrite(table, sizeof(union palette_element_u), colour_count, file_p->file_p);
    free(table);
    long bitmap_offset = ftell(file_p->file_p);
    bmp_file_header_t header;
    image_file_seek_header(file_p);
    fread(&header, sizeof(header), 1, file_p->file_p);
    header.data_offset = bitmap_offset;
    image_file_seek_header(file_p);
    fwrite(&header, sizeof(header), 1, file_p->file_p);
}

static void
image_file_write_file_size(image_file_t* file_p)
{
    fseek(file_p->file_p, 0, SEEK_END);
    long size = ftell(file_p->file_p);
    image_file_seek_header(file_p);
    bmp_file_header_t header;
    fread(&header, sizeof(header), 1, file_p->file_p);
    header.file_size = (uint32_t) size;
    image_file_seek_header(file_p);
    fwrite(&header, sizeof(header), 1, file_p->file_p);
}
