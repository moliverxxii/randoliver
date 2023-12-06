/*
 * transform.c
 *
 *  Created on: 6 déc. 2023
 *      Author: moliver
 */

#include "main.h"

point_t*
add_points(point_t* point_r_p, point_t point_a, point_t point_b)
{
	if(!point_r_p)
	{
		return NULL;
	}
	memcpy(point_r_p, &point_a, sizeof(point_t));
	point_r_p->x += point_b.x;
	point_r_p->y += point_b.y;
	point_r_p->z += point_b.z;
	return point_r_p;

}

point_t*
subtract_points(point_t* point_r_p, point_t point_a, point_t point_b)
{
	point_t minus_point_b;
	return add_points(point_r_p, point_a, *negative_point(&minus_point_b, point_b));
}


point_t*
negative_point(point_t* point_r_p, point_t point)
{
	return scale_point(point_r_p, point, -1);
}


point_t*
scale_point(point_t* point_r_p, point_t point, float scale)
{
	if(!point_r_p)
	{
		return point_r_p;
	}
	memcpy(point_r_p, &point, sizeof(point));
	point_r_p->x *= scale;
	point_r_p->y *= scale;
	point_r_p->z *= scale;
	return point_r_p;

}


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
radial_scale_point(point_t* point_p, point_t reference, float scale)
{

}

void
planar_scale_point(point_t* point, point_t plane_a, point_t normal_b, float scale)
{
}

void
axial_scale_point(point_t* point, point_t axis_a, point_t axis_b, float scale)
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
    m->x += rand() % (2 * amplitude + 1) - amplitude;
    modulo(m->x, width);
    m->y += rand() % (2 * amplitude + 1) - amplitude;
    modulo(m->y, height);
}
