/*
 * colours.c
 *
 *  Created on: 19 nov. 2023
 *      Author: moliver
 */
#include <stdlib.h>

#include "colours.h"
#include "utility.h"

const colour_struct_t BLACK   = {0x00, 0x00, 0x00};
const colour_struct_t WHITE   = {0xFF, 0xFF, 0xFF};
const colour_struct_t GREY    = {0x80, 0x80, 0x80};
const colour_struct_t RED     = {0x00, 0x00, 0xFF};
const colour_struct_t GREEN   = {0x00, 0xFF, 0x00};
const colour_struct_t BLUE    = {0xFF, 0x00, 0x00};
const colour_struct_t CYAN    = {0xFF, 0xFF, 0x00};
const colour_struct_t MAGENTA = {0xFF, 0x00, 0xFF};
const colour_struct_t YELLOW  = {0x00, 0xFF, 0xFF};

colour_struct_t
get_random_colour()
{
    uint32_t colour = rand();
    return *(colour_struct_t*) &colour;
}

colour_struct_t
random_delta_colour(colour_struct_t colour, void* parameters_p)
{
    float range = *(int*) parameters_p;
    int colour_index;
    int8_t delta[COLOUR_COUNT];
    int rand_colour = rand();

    *(colour_struct_t*) delta = *(colour_struct_t*) &rand_colour;
    for(colour_index = 0; colour_index < COLOUR_COUNT; ++colour_index)
    {
        uint8_t* colour_p = (uint8_t*) &colour + colour_index;
        *colour_p += (int8_t) (2*range * ((float) delta[colour_index] + 0.5f)/INT8_MAX);

    }
    return colour;
}

colour_struct_t
add_colours(colour_struct_t colour_1, colour_struct_t colour_2, void* parameters_p)
{
    int colour;
    colour_t return_colour;
    for(colour = 0; colour<COLOUR_COUNT; ++colour)
    {
        uint16_t tmp_colour = (uint16_t) ((uint8_t*) &colour_1)[colour] + ((uint8_t*) &colour_2)[colour];
        return_colour[colour] = saturator(tmp_colour, SAT_MIN, SAT_MAX);
    }
    return *(colour_struct_t*) return_colour;
}
