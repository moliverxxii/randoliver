/*
 * light.h
 *
 *  Created on: 17 févr. 2026
 *      Author: moliver
 */

#ifndef HEADERS_LIGHT_H_
#define HEADERS_LIGHT_H_

#include "colours.h"
#include "vector.h"

typedef enum light_e
{
    LIGHT_POINT,
    LIGHT_INFINITY

} light_e;

typedef struct light_common_t
{
    colour_t colour;
    float power;
} light_common_t;

typedef struct light_point_t
{
    light_common_t light;
    vector_axis_t position;
} light_point_t;

typedef struct light_infinity_t
{
    light_common_t light;
    float latitude;
    float longitude;
} light_infinity_t;

#endif /* HEADERS_LIGHT_H_ */
