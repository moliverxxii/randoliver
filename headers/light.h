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
#include "renderable.h"

typedef enum light_e
{
    LIGHT_POINT,
    LIGHT_INFINITY

} light_e;

typedef struct light_common_t
{
    renderable_i renderable_i;
    light_e type;
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

typedef union
{
    light_point_t point;
    light_infinity_t infinity;
} light_t;

light_t* light_init_point(vector_t origin, colour_t colour, float_t power);

void light_render(const void*, image_t* image_p, const camera_t* camera_p);

#endif /* HEADERS_LIGHT_H_ */
