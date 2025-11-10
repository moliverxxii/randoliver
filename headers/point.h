/*
 * header.h
 *
 *  Created on: 2 nov. 2025
 *      Author: moliver
 */

#ifndef HEADERS_POINT_H_
#define HEADERS_POINT_H_

#include "transform.h"
#include "colours.h"

/** Un point dans l'espace
 *
 */
typedef struct
{
    vector_t vector;
    colour_struct_t colour;
} point_t;

point_t point_init(vector_axis_t x, vector_axis_t y, vector_axis_t z, colour_struct_t colour);
void point_print(point_t point);



#endif /* HEADERS_POINT_H_ */
