/*
 * point.c
 *
 *  Created on: 2 nov. 2025
 *      Author: moliver
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "point.h"
#include "utility.h"

typedef struct point_t
{
    renderable_i renderable;
    vector_t vector;
    colour_t colour;
} point_t;

point_renderer_f public_point_renderer = &point_draw;


point_t*
point_init(vector_axis_t x, vector_axis_t y, vector_axis_t z, colour_t colour)
{
    point_t* point_p = malloc(sizeof(point_t));

    point_t point =
    {
        renderable_init(&point_render, point_p),
        vector_init(x, y, z),
        colour
    };
    if(point_p != NULL)
    {
        *point_p = point;
    }
    return point_p;
}

void
point_free(point_t* point_p)
{
    free(point_p);
}

void
point_print(const point_t* point_p)
{
     printf("xyz: %4f %4f %4f #%02X%02X%02X\n",
            point_p->vector.x, point_p->vector.y, point_p->vector.z,
            point_p->colour.red, point_p->colour.green, point_p->colour.blue);
}

void
point_copy(point_t* this_p, const point_t* point_p)
{
    *this_p = *point_p;
}

void
point_draw(const point_t* point_p, image_t* image_p)
{
    int x = point_p->vector.x;
    int y = point_p->vector.y;
    image_pixel_set(image_p, x, y, point_p->colour);
}

void
point_or(const point_t* point_p, image_t* image_p)
{
    int x = point_p->vector.x;
    int y = point_p->vector.y;
    colour_t colour = image_pixel_get(image_p, x, y);
    uint32_t buffer = *(uint32_t*) colour.array;

    buffer |= *(uint32_t*) &point_p->colour;

    memcpy(&colour, &buffer, sizeof(colour));

    image_pixel_set(image_p, x, y, colour);
}

void
point_xor(const point_t* point_p, image_t* image_p)
{
    int x = point_p->vector.x;
    int y = point_p->vector.y;
    colour_t colour = image_pixel_get(image_p, x, y);
    uint32_t buffer = *(uint32_t*) colour.array;

    buffer ^= *(uint32_t*) &point_p->colour;

    memcpy(&colour, &buffer, sizeof(colour));

    image_pixel_set(image_p, x, y, colour);
}


void
point_average(const point_t* point_p, image_t* image_p)
{
    int x = point_p->vector.x;
    int y = point_p->vector.y;
    image_pixel_set(image_p, x, y, colour_average(image_pixel_get(image_p, x, y), point_p->colour));
}

int
point_is_in_image(const point_t* point_p, const image_t* image_p)
{
    return image_is_in(image_p,
                       point_vector((point_t*) point_p)->x,  //const OK
                       point_vector((point_t*) point_p)->y); //const OK
}

void point_render(const void* this_p,
                  image_t* image_p,
                  const camera_t* camera_p)
{
    point_t* point_p = (point_t*) this_p; //const OK
    render_vector(*point_vector(point_p),
                  *point_colour(point_p),
                  image_p,
                  camera_p);
}

renderable_i*
point_renderable(point_t* point_p)
{
    return &point_p->renderable;
}

vector_t*
point_vector(point_t* point_p)
{
    return &point_p->vector;
}

colour_t*
point_colour(point_t* point_p)
{
    return &point_p->colour;
}

