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


void transform_figure(figure_t* figure_p, point_transformer_t transformer_p);
void translate_point(point_t* point_p, point_t direction);
void rotate_point(vector_t* vector_p, vector_t axis_a, vector_t axis_b, float angle);

void project_point(vector_t* vector_p, vector_t axis_a, vector_t axis_b);

void radial_scale_point(vector_t* vector_p, vector_t reference, float scale);
void axial_scale_point(vector_t* vector_p, vector_t axis_a, vector_t axis_b, float scale);
void planar_scale_point(vector_t* vector_p, vector_t plane_a, vector_t normal_b, float scale);

void rand_coord_point(vector_t* point_p, int height, int width);

/**
 * Randomly moves a point.
 */
void rand_delta_point(vector_t* point_p, int amplitude, int width, int height);





#endif /* HEADERS_TRANSFORM_H_ */
