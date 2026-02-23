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

typedef struct figure_t
{
    uint32_t length;
    point_t** array;
} figure_t;

figure_t*
figure_init(uint32_t point_count)
{
    figure_t figure;

    figure.length = point_count;
    figure.array = (point_t**) malloc(point_count * sizeof(point_t*));

    for(uint32_t point = 0; point < point_count; ++point)
    {
        figure.array[point] = point_init(0, 0, 0, BLACK);
    }

    figure_t* figure_p = malloc(sizeof(figure_t));
    if(figure_p != NULL)
    {
        *figure_p = figure;
    }
    return figure_p;
}

figure_t*
figure_copy(const figure_t* figure_p)
{
    figure_t* copy_p = figure_init(figure_p->length);
    for(uint32_t point = 0; point < copy_p->length; ++point)
    {
        point_copy(copy_p->array[point], figure_p->array[point]);
    }
    return copy_p;
}

void
figure_free(figure_t* figure_p)
{
    for(uint32_t point = 0; point<figure_p->length; ++point)
    {
        point_free(figure_p->array[point]);
    }
    free(figure_p->array);
    free(figure_p);
}

figure_t*
figure_from_image(const image_t* image_p)
{
    figure_t* figure_p = figure_init(image_width(image_p) * image_height(image_p));
    for(uint32_t point = 0; point<figure_p->length; point++)
    {
        colour_t* image_data_p = image_data(image_p);
        point_t* new_point_p = point_init(point%image_width(image_p), point/image_height(image_p), 0, image_data_p[point]);
        point_copy(figure_p->array[point], new_point_p);
        point_free(new_point_p);
    }
    return figure_p;
}

uint32_t
figure_length(const figure_t* figure_p)
{
    return figure_p->length;
}

point_t*
figure_point(const figure_t* figure_p, uint32_t index)
{
    return figure_p->array[index];
}


vector_t
figure_get_average_point(const figure_t* figure_p)
{
    vector_t average = VECTOR_0;

    for(uint32_t point_count = 0; point_count<figure_p->length; ++point_count)
    {
        vector_t vector = *point_vector(figure_p->array[point_count]);
        for(int axis=0; axis<VECTOR_AXIS_COUNT; ++axis)
        {
            average.array[axis] += vector.array[axis];
        }
    }
    for(int axis=0; axis<VECTOR_AXIS_COUNT; ++axis)
    {
        average.array[axis] /= figure_p->length;
    }

    return average;
}

void
figure_render(const figure_t* figure_p,
              image_t* image_p,
              const camera_t* camera_p)
{
    for(uint32_t point = 0; point < figure_length(figure_p); ++point)
    {
        renderable_render(point_renderable(figure_point(figure_p, point)), image_p, camera_p);
    }
}

void
figure_draw(const figure_t* figure, image_t* image_p)
{
    for(uint32_t point = 0; point < figure->length; ++point)
    {
        if(point_is_in_image(figure->array[point], image_p))
        {
            (*public_point_renderer)(figure->array[point], image_p);
        }
    }
}

