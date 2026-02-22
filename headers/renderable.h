/*
 * renderable.h
 *
 *  Created on: 17 févr. 2026
 *      Author: moliver
 */

#ifndef HEADERS_RENDERABLE_H_
#define HEADERS_RENDERABLE_H_

#include "camera.h"
#include "image.h"

typedef struct renderable_i renderable_i;

typedef void (*renderable_render_f)(const renderable_i* item,
                                    image_t* image_p,
                                    const camera_t* camera_p);

typedef struct renderable_i
{
    renderable_render_f render;
} renderable_i;

renderable_i renderable_init(renderable_render_f render_function);

vector_t camera_render_point_position(const camera_t* camera_p, image_t* image_p, vector_t point);

#endif /* HEADERS_RENDERABLE_H_ */
