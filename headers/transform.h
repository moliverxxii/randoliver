/*
 * transform.h
 *
 *  Created on: 6 déc. 2023
 *      Author: moliver
 */

#ifndef HEADERS_TRANSFORM_H_
#define HEADERS_TRANSFORM_H_

#include "main.h"

typedef void (*point_transformer_t)(point_t*, ...);

/**
 * *point_r_p = point_a + point_b
 */
point_t* add_points(point_t* point_r_p, point_t point_a, point_t point_b);

/**
 * *point_r_p = point_a - point_b
 */
point_t* subtract_points(point_t* point_r_p, point_t point_a, point_t point_b);

/**
 * *point_r_p = scale * point
 */
point_t* scale_point(point_t* point_r_p, point_t point, float scale);

/**
 * *point_r_p = -point
 */
point_t* negative_point(point_t* point_r_p, point_t point);

void transform_figure(figure_t* figure_p, point_transformer_t transformer_p);
void translate_point(point_t* point_p, point_t direction);
void rotate_point(point_t* point_p, point_t axis_a, point_t axis_b, float angle);

void radial_scale_point(point_t* point_p, point_t reference, float scale);
void planar_scale_point(point_t* point, point_t plane_a, point_t normal_b, float scale);
void axial_scale_point(point_t* point, point_t axis_a, point_t axis_b, float scale);

void rand_coord(point_t* m, int width, int height);

/**
 * Randomly moves a point.
 */
void rand_delta_point(point_t* m, int amplitude, int width, int height);


#endif /* HEADERS_TRANSFORM_H_ */
