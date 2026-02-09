/*
 * colours.c
 *
 *  Created on: 19 nov. 2023
 *      Author: moliver
 */
#include <stdlib.h>
#include <string.h>
#include "colours.h"
#include "utility.h"

const colour_t BLACK   = {{0x00, 0x00, 0x00}};
const colour_t WHITE   = {{0xFF, 0xFF, 0xFF}};
const colour_t GREY    = {{0x80, 0x80, 0x80}};
const colour_t RED     = {{0x00, 0x00, 0xFF}};
const colour_t GREEN   = {{0x00, 0xFF, 0x00}};
const colour_t BLUE    = {{0xFF, 0x00, 0x00}};
const colour_t CYAN    = {{0xFF, 0xFF, 0x00}};
const colour_t MAGENTA = {{0xFF, 0x00, 0xFF}};
const colour_t YELLOW  = {{0x00, 0xFF, 0xFF}};

colour_t
colour_get_random()
{
    uint32_t colour_raw = rand();
    colour_t colour;
    memcpy(&colour, &colour_raw, sizeof(colour));
    return colour;
}

colour_t
colour_random_delta(colour_t colour, void* parameters_p)
{
    float range = *(int*) parameters_p;
    int colour_index;
    int8_t delta[COLOUR_COUNT];
    uint32_t rand_colour = rand();

    memcpy(delta, &rand_colour, sizeof(delta));
    for(colour_index = 0; colour_index < COLOUR_COUNT; ++colour_index)
    {
        uint8_t* colour_p = (uint8_t*) &colour + colour_index;
        *colour_p += (int8_t) (2*range * ((float) delta[colour_index] + 0.5f)/INT8_MAX);

    }
    return colour;
}

colour_t
colour_add_2(colour_t colour_1, colour_t colour_2, void* parameters_p)
{
    OLI_UNUSED(parameters_p);

    int colour;
    colour_t return_colour;
    for(colour = 0; colour<COLOUR_COUNT; ++colour)
    {
        uint16_t tmp_colour = (uint16_t) colour_1.array[colour] + (uint16_t) colour_2.array[colour];
        return_colour.array[colour] = saturator(tmp_colour, COLOUR_MIN, COLOUR_MAX);
    }
    return return_colour;
}
