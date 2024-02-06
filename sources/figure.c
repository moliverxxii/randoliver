/*
 * figure.c
 *
 *  Created on: 5 déc. 2023
 *      Author: moliver
 */

#include <stdio.h>
#include "figure.h"

void
print_point(point_t point)
{
    printf("(x y z) = (%4f %4f %4f | (R G B) = (%02X %02X %02X))\n",
            point.vector.x, point.vector.y, point.vector.z,
            point.colour.red, point.colour.green, point.colour.blue);
}


point_t
init_point()
{
    point_t point;
    point.colour = BLACK;
    point.vector = VECTOR_0;
    return point;
}

figure_t
init_figure(uint32_t nombre_point)
{
    figure_t figure;

    figure.amount = nombre_point;
    figure.sequence = (point_t*) malloc(nombre_point * sizeof(point_t));

    int i;
    for(i = 0; i < nombre_point; ++i)
    {
        figure.sequence[i] = init_point();
    }
    return figure;
}

vector_t
get_average_point(const figure_t* figure_p)
{
    vector_t average = {0, 0, 0};
    uint32_t integral_colours[COLOUR_COUNT] = {0, 0, 0};

    for(uint32_t point_count = 0; point_count<figure_p->amount; ++point_count)
    {
        for(int axis=0; axis<3; ++axis)
        {
            (&average.x)[axis] += (&figure_p->sequence[point_count].vector.x)[axis];
        }
        for(int colour=0; colour<COLOUR_COUNT; ++colour)
        {
            integral_colours[colour] += ((uint8_t*) &figure_p->sequence[point_count].colour)[colour];
        }
    }
    for(int axis=0; axis<3; ++axis)
    {
        (&average.x)[axis] /= figure_p->amount;
    }

    return average;
}



