/*
 * point.c
 *
 *  Created on: 2 nov. 2025
 *      Author: moliver
 */
#include <stdio.h>

#include "point.h"

point_t
point_init(vector_axis_t x, vector_axis_t y, vector_axis_t z, colour_t colour)
{
    point_t point =
    {
        {x, y, z},
        colour
    };
    return point;
}

void
point_print(point_t point)
{
    printf("(x y z) = (%4f %4f %4f | (R G B) = (%02X %02X %02X))\n",
            point.vector.x, point.vector.y, point.vector.z,
            point.colour.red, point.colour.green, point.colour.blue);
}
