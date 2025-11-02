/*
 * point.c
 *
 *  Created on: 2 nov. 2025
 *      Author: moliver
 */
#include "point.h"

point_t point_init(vector_axis_t x, vector_axis_t y, vector_axis_t z, colour_struct_t colour)
{
    point_t point =
    {
        {x, y, z},
        colour
    };
    return point;
}


