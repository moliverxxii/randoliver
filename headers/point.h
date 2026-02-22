/*
 * header.h
 *
 *  Created on: 2 nov. 2025
 *      Author: moliver
 */

#ifndef HEADERS_POINT_H_
#define HEADERS_POINT_H_

#include "colours.h"
#include "vector.h"
#include "renderable.h"

/** Un point dans l'espace
 *
 */
typedef struct
{
    vector_t vector;
    colour_t colour;
} point_t;

point_t point_init(vector_axis_t x, vector_axis_t y, vector_axis_t z, colour_t colour);
void point_print(point_t point);

void camera_render_point(const camera_t* camera_p,
                         image_t* image_p,
                         point_t point);

typedef void (*image_point_renderer)(image_t*, point_t);

extern image_point_renderer public_point_renderer;

void image_draw_point(image_t* image_p, point_t point);
void image_or_point(image_t* image_p, point_t point);
void image_xor_point(image_t* image_p, point_t point);
void image_average_point(image_t* image_p, point_t point);

int point_is_in_image(const point_t* point_p, const image_t* image_p);

void camera_cache_clear();

#endif /* HEADERS_POINT_H_ */
