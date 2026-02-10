/*
 * figure.c
 *
 *  Created on: 5 déc. 2023
 *      Author: moliver
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "figure.h"
#include "image.h"

figure_t
figure_init(uint32_t point_count)
{
    figure_t figure;

    figure.amount = point_count;
    figure.array = (point_t*) malloc(point_count * sizeof(point_t));

    uint32_t point;
    for(point = 0; point < point_count; ++point)
    {
        figure.array[point] = point_init(0, 0, 0, BLACK);
    }
    return figure;
}

figure_t
figure_copy(figure_t figure)
{
    figure_t copy = figure_init(figure.amount);
    memcpy(copy.array, figure.array, figure.amount * sizeof(point_t));
    return copy;
}

void
figure_free(figure_t* figure_p)
{
    figure_p->amount = 0;
    free(figure_p->array);
    figure_p-> array = NULL;
}

figure_t
figure_from_image(const image_t* image_p)
{
    figure_t figure = figure_init(image_p->width*image_p->height);
    for(uint32_t point = 0; point<figure.amount; point++)
    {
        colour_t* image_data_p = *image_p->image;
        figure.array[point]
            = (point_t) 
            {
                vector_init(point%image_p->width, point/image_p->height, 0),
                image_data_p[point]
            };
    }
    return figure;
}

vector_t
figure_get_average_point(const figure_t* figure_p)
{
    vector_t average = VECTOR_0;

    for(uint32_t point_count = 0; point_count<figure_p->amount; ++point_count)
    {
        for(int axis=0; axis<VECTOR_AXIS_COUNT; ++axis)
        {
            (&average.x)[axis] += (&figure_p->array[point_count].vector.x)[axis];
        }
    }
    for(int axis=0; axis<3; ++axis)
    {
        (&average.x)[axis] /= figure_p->amount;
    }

    return average;
}

