/*
 * edge.h
 *
 *  Created on: 9 févr. 2026
 *      Author: moliver
 */

#ifndef HEADERS_EDGE_H_
#define HEADERS_EDGE_H_

#include "vector.h"
#include "colours.h"
#include "renderable.h"

typedef struct edge_t edge_t;

edge_t* edge_init(vector_t* a_p, vector_t* b_p, colour_t colour);
void    edge_free(edge_t* edge_p);

vector_t edge_get_vector(const edge_t* edge_p, float fraction);

void edge_render(const edge_t* edge,
                 image_t* image_p,
                 const camera_t* camera_p);

#endif /* HEADERS_EDGE_H_ */
