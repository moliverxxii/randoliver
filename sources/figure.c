/*
 * figure.c
 *
 *  Created on: 5 déc. 2023
 *      Author: moliver
 */
#include "main.h"

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



