/*
 * solid.h
 *
 *  Created on: 17 févr. 2026
 *      Author: moliver
 */

#ifndef HEADERS_SOLID_H_
#define HEADERS_SOLID_H_

#include "renderable.h"
#include "triangle.h"


typedef struct solid_t solid_t;

void solid_init_const();

solid_t* solid_init(uint32_t vertex_count, const vector_t* vertices_p,
                    uint32_t face_count, const uint32_t **faces_pp, const colour_t* colours_p);
void     solid_free(solid_t* solid_p);

void solid_render(const void* this_p,
                  image_t* image_p,
                  const camera_t* camera_p);

renderable_i* solid_renderable(solid_t* solid_p);

extern const solid_t* const OCTAHEDRON_P;

#endif /* HEADERS_SOLID_H_ */
