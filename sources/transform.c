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

point_t*
negative_point(point_t* point_r_p, point_t point)
{
	return scale_point(point_r_p, point, -1);
}


void
transform_figure(figure_t* figure_p, point_transformer_t transformer_p)
{

}

void
translate_point(point_t* point_p, point_t direction)
{
	add_points(point_p, *point_p, direction);
}

void
rotate_point(point_t* point_p, point_t axis_a, point_t axis_b, float angle)
{
}

float
norm_point(point_t point_p)
{
	return (float) sqrt(scalar(point_p, point_p));
}

float
scalar(point_t point_a, point_t point_b)
{
	return point_a.x*point_b.x + point_a.y*point_b.y + point_a.z*point_b.z;
}

void
project_point(point_t* point_p, point_t axis_a, point_t axis_b)
{
	point_t vect_ab;
	point_t vect_ap;
	subtract_points(&vect_ab, axis_b, axis_a);
	subtract_points(&vect_ap, *point_p, axis_a);

	float scale = scalar(vect_ab, vect_ap)/scalar(vect_ab, vect_ab);

	point_t vect_temp;
	scale_point(&vect_temp, vect_ab, scale);
	add_points(point_p, axis_a, vect_temp);
}


void
radial_scale_point(point_t* point_p, point_t reference, float scale)
{
	point_t point_temp = init_point();

	subtract_points(&point_temp, *point_p, reference);
	scale_point(&point_temp, point_temp, scale);
	add_points(point_p, point_temp, reference);
}

void
axial_scale_point(point_t* point_p, point_t axis_a, point_t axis_b, float scale)
{
	point_t point_r = *point_p;
	project_point(&point_r, axis_a, axis_b);

	point_t vect_rp;
	subtract_points(&vect_rp, *point_p, point_r);
	scale_point(&vect_rp, vect_rp, scale);

	add_points(point_p, vect_rp, *point_p);

}

void
planar_scale_point(point_t* point_p, point_t plane_a, point_t normal_b, float scale)
{
}

void
rand_coord_point(point_t* point_p, int height, int width)
{
    int randX = rand() % width;
    int randY = rand() % height;
    point_p->x = randX;
    point_p->y = randY;
}

void
rand_delta_point(point_t* point_p, int amplitude, int width, int height)
{
    point_p->x += rand() % (2 * amplitude + 1) - amplitude;
    modulo(point_p->x, width);
    point_p->y += rand() % (2 * amplitude + 1) - amplitude;
    modulo(point_p->y, height);
}
