/*
 * figure.c
 *
 *  Created on: 5 déc. 2023
 *      Author: moliver
 */

#include <stdio.h>
#include <stdlib.h>
#include "figure.h"

figure_t
figure_init(uint32_t point_count)
{
    figure_t figure;

    figure.amount = point_count;
    figure.sequence = (point_t*) malloc(point_count * sizeof(point_t));

    uint32_t point;
    for(point = 0; point < point_count; ++point)
    {
        figure.sequence[point] = point_init(0, 0, 0, BLACK);
    }
    return figure;
}

vector_t
figure_get_average_point(const figure_t* figure_p)
{
    vector_t average = {0, 0, 0};

    for(uint32_t point_count = 0; point_count<figure_p->amount; ++point_count)
    {
        for(int axis=0; axis<3; ++axis)
        {
            (&average.x)[axis] += (&figure_p->sequence[point_count].vector.x)[axis];
        }
    }
    for(int axis=0; axis<3; ++axis)
    {
        (&average.x)[axis] /= figure_p->amount;
    }

    return average;
}



