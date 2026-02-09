/*
 * edge.h
 *
 *  Created on: 9 févr. 2026
 *      Author: moliver
 */

#ifndef HEADERS_EDGE_H_
#define HEADERS_EDGE_H_

#include "point.h"

typedef struct
{
    point_t* array[2];
    colour_t colour;
} edge_t;

#endif /* HEADERS_EDGE_H_ */
