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

typedef struct
{
    vector_t* array[2];
    colour_t colour;
} edge_t;

edge_t edge_init(vector_t* a_p, vector_t* b_p, colour_t colour);

vector_t edge_get_vector(const edge_t* edge_p, float fraction);

#endif /* HEADERS_EDGE_H_ */
