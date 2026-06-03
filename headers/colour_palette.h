/*
 * colour_palette.h
 *
 *  Created on: 3 juin 2026
 *      Author: moliver
 */

#ifndef HEADERS_COLOUR_PALETTE_H_
#define HEADERS_COLOUR_PALETTE_H_

#include <stdint.h>

#include "colours.h"

typedef enum
{
    PIXEL_BIT_DEPTH_1b  =  1,
    PIXEL_BIT_DEPTH_4b  =  4,
    PIXEL_BIT_DEPTH_8b  =  8,
    PIXEL_BIT_DEPTH_16b = 16,
    PIXEL_BIT_DEPTH_24b = 24,
    PIXEL_BIT_DEPTH_32b = 32,
} palette_bit_depth_e;

typedef enum
{
    PALETTE_INDEX_METHOD_VALUE = 0,
    PALETTE_INDEX_METHOD_BLUE,
    PALETTE_INDEX_METHOD_RED,
    PALETTE_INDEX_METHOD_GREEN,
    PALETTE_INDEX_METHOD_DISTANCE,
} palette_index_method_e;

typedef struct palette_t palette_t;

typedef uint32_t palette_index_t;

palette_index_t palette_get_count(palette_bit_depth_e bit_depth);
palette_index_t palette_count(const palette_t* palette_p);

palette_t* palette_init(palette_bit_depth_e bitdepth,
                                      palette_index_t colour_count);

palette_index_t palette_index_get(const palette_t* palette_p,
                                                colour_t colour);

const colour_t* palette_colour_get(const palette_t* palette_p,
                                   palette_index_t index);

void palette_print(const palette_t* palette_p);
#endif /* HEADERS_COLOUR_PALETTE_H_ */
