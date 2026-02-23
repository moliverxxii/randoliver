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
typedef struct point_t point_t;

point_t* point_init(vector_axis_t x, vector_axis_t y, vector_axis_t z, colour_t colour);
void point_free(point_t* point_p);
void point_print(const point_t* point_p);
void point_copy(point_t* this_p, const point_t* point_p);

void point_render(const void* this_p,
                  image_t* image_p,
                  const camera_t* camera_p);
renderable_i* point_renderable(point_t* point_p);

vector_t* point_vector(point_t* point_p);
colour_t* point_colour(point_t* point_p);

typedef void (*point_renderer_f)(const point_t*, image_t*);

extern point_renderer_f public_point_renderer;

void point_draw(const point_t* point_p, image_t* image_p);
void point_or(const point_t* point_p, image_t* image_p);
void point_xor(const point_t* point_p, image_t* image_p);
void point_average(const point_t* point_p, image_t* image_p);

int point_is_in_image(const point_t* point_p, const image_t* image_p);

#endif /* HEADERS_POINT_H_ */
