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
    figure.array = (point_t**) malloc(point_count * sizeof(point_t*));

    for(uint32_t point = 0; point < point_count; ++point)
    {
        figure.array[point] = point_init(0, 0, 0, BLACK);
    }
    return figure;
}

figure_t
figure_copy(figure_t figure)
{
    figure_t copy = figure_init(figure.amount);
    for(uint32_t point = 0; point < figure.amount; ++point)
    {
        point_copy(copy.array[point], figure.array[point]);
    }
    return copy;
}

void
figure_free(figure_t* figure_p)
{
    for(uint32_t point = 0; point<figure_p->amount; ++point)
    {
        point_free(figure_p->array[point]);
    }
    free(figure_p->array);
    figure_p->amount = 0;
    figure_p-> array = NULL;
}

figure_t
figure_from_image(const image_t* image_p)
{
    figure_t figure = figure_init(image_width(image_p) * image_height(image_p));
    for(uint32_t point = 0; point<figure.amount; point++)
    {
        colour_t* image_data_p = image_data(image_p);
        point_t* new_point_p = point_init(point%image_width(image_p), point/image_height(image_p), 0, image_data_p[point]);
        point_copy(figure.array[point], new_point_p);
        point_free(new_point_p);
    }
    return figure;
}

vector_t
figure_get_average_point(const figure_t* figure_p)
{
    vector_t average = VECTOR_0;

    for(uint32_t point_count = 0; point_count<figure_p->amount; ++point_count)
    {
        vector_t vector = *point_vector(figure_p->array[point_count]);
        for(int axis=0; axis<VECTOR_AXIS_COUNT; ++axis)
        {
            average.array[axis] += vector.array[axis];
        }
    }
    for(int axis=0; axis<3; ++axis)
    {
        average.array[axis] /= figure_p->amount;
    }

    return average;
}

void
figure_render(const camera_t* camera_p,
                     image_t* image_p,
                     figure_t figure)
{
    for(uint32_t point = 0; point < figure.amount; ++point)
    {
        renderable_render(point_renderable(figure.array[point]), image_p, camera_p);
    }
}

void
figure_draw(const figure_t* figure, image_t* image_p)
{
    for(uint32_t point = 0; point < figure->amount; ++point)
    {
        if(point_is_in_image(figure->array[point], image_p))
        {
            (*public_point_renderer)(figure->array[point], image_p);
        }
    }
}

