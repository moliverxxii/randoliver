/*
 * transform.h
 *
 *  Created on: 6 déc. 2023
 *      Author: moliver
 */

#ifndef HEADERS_TRANSFORM_H_
#define HEADERS_TRANSFORM_H_

#include "main.h"

typedef void (*point_transformer_t)(point_t*);

void transform_figure(figure_t* figure_p, point_transformer_t transformer_p);

void translate_point(point_t* point_p, point_t direction);

void rotate_point(point_t* point_p, point_t axis_a, point_t axis_b, float angle);
void scale_point(point_t* point_p, point_t reference, float scale);

void planar_scale_point(point_t* point, point_t plane_a, point_t normal_b, float scale);


void rand_coord(point_t* m, int width, int height);
void rand_delta_point(point_t* m, int amplitude, int width, int height);


#endif /* HEADERS_TRANSFORM_H_ */
