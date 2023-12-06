/*
 * transform.c
 *
 *  Created on: 6 déc. 2023
 *      Author: moliver
 */

#include "main.h"


void
transform_figure(figure_t* figure_p, point_transformer_t transformer_p)
{

}

void
translate_point(point_t* point_p, point_t direction)
{
}

void
rotate_point(point_t* point_p, point_t axis_a, point_t axis_b, float angle)
{
}

void
scale_point(point_t* point_p, point_t reference, float scale)
{

}

void
planar_scale_point(point_t* point, point_t plane_a, point_t normal_b, float scale)
{
}

void
rand_coord(point_t* m, int height, int width)
{
    int randX = rand() % width;
    int randY = rand() % height;
    m->x = randX;
    m->y = randY;
}

void
rand_delta_point(point_t* m, int amplitude, int width, int height)
{
    //Randomly moves a point.
    m->x += rand() % (2 * amplitude + 1) - amplitude;
    modulo(m->x, width);
    m->y += rand() % (2 * amplitude + 1) - amplitude;
    modulo(m->y, height);
}
