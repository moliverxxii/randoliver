/*
 * figure.c
 *
 *  Created on: 5 déc. 2023
 *      Author: moliver
 */
#include "main.h"

void
print_point(point_t point)
{
    printf("(x y z) = (%4d %4d %4d | (R G B) = (%02X %02X %02X))\n",
            point.x, point.y, point.z,
            point.colour.red, point.colour.green, point.colour.blue);
}

point_t*
vector_to_point(point_t* point_p, vector_t vector)
{
    if(!point_p)
    {
        return NULL;
    }
    point_p->x = vector.x;
    point_p->y = vector.y;
    point_p->z = vector.z;
    return point_p;
}

vector_t*
point_to_vector(vector_t* vector_p, point_t point)
{
    if(!vector_p)
    {
        return NULL;
    }
    vector_p->x = point.x;
    vector_p->y = point.y;
    vector_p->z = point.z;
    return vector_p;
}


point_t
init_point()
{
    point_t point;
    point.colour = BLACK;
    point.x = 0;
    point.y = 0;
    point.z = 0;
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

point_t
get_average_point(const figure_t* figure_p)
{
    vector_t average = {0, 0, 0};
    uint32_t integral_colours[COLOUR_COUNT];

    for(uint32_t point_count = 0; point_count<figure_p->amount; ++point_count)
    {
        for(int axis=0; axis<3; ++axis)
        {
            (&average.x)[axis] += (&figure_p->sequence[point_count].x)[axis];
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
    colour_t average_colour;
    for(int colour=0; colour<COLOUR_COUNT; ++colour)
    {
        average_colour[colour] = integral_colours[colour] / figure_p->amount;
    }
    point_t average_point;
    average_point.colour = *(colour_struct_t*) average_colour;

    return *vector_to_point(&average_point, average);
}



