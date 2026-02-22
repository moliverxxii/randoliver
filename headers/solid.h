/*
 * solid.h
 *
 *  Created on: 17 févr. 2026
 *      Author: moliver
 */

#ifndef HEADERS_SOLID_H_
#define HEADERS_SOLID_H_

#include "triangle.h"

typedef struct solid_t
{
    triangle_t* array_p;
    vector_t origin;
    float rotation1;
    float rotation2;
} solid_t;

#endif /* HEADERS_SOLID_H_ */
