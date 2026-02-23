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
#include "vector.h"

typedef struct renderable_i renderable_i;

typedef void (*renderable_render_f)(const void* this_p,
                                    image_t* image_p,
                                    const camera_t* camera_p);

typedef struct renderable_i
{
    renderable_render_f render;
    void* this_p;
} renderable_i;

extern const float CAMERA_SUBDIVISION;



renderable_i renderable_init(renderable_render_f render_function, void* this_p);
void render_vector(vector_t vector, colour_t colour, image_t* image_p, const camera_t* camera_p);

void renderable_cache_clear();
vector_t camera_render_point_position(const camera_t* camera_p, image_t* image_p, vector_t point);

void renderable_render(const renderable_i* this_p, image_t* image_p, const camera_t* camera_p);
void renderable_cache_clear();


#endif /* HEADERS_RENDERABLE_H_ */
