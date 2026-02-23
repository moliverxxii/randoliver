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

typedef struct camera_cache_t
{
    uint32_t width;
    uint32_t height;
    const image_t* current_image_p;
    vector_axis_t* pixel_u_index;
    vector_axis_t** pixel_u_row;
} camera_cache_t;

static camera_cache_t camera_cache =
{
    0,
    0,
    NULL,
    NULL,
    NULL
};

static int  camera_cache_is_same_image(const camera_cache_t* camera_cache_p, const image_t* image_p);
static void camera_cache_allocate(camera_cache_t* camera_cache_p, const image_t* image_p);
static void camera_cache_deallocate(camera_cache_t* camera_cache_p);
static int  camera_cache_is_allocated(const camera_cache_t* camera_cache_p);
static vector_axis_t camera_cache_u_index_get(const camera_cache_t* camera_cache_p, uint32_t x, uint32_t y);
static void camera_cache_u_index_set(camera_cache_t* camera_cache_p, uint32_t x, uint32_t y, vector_axis_t u_index);

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
    printf("(x y z) = (%4f %4f %4f | (R G B) = (%02X %02X %02X))\n",
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
    return (point_p->vector.x >= 0)
            && (point_p->vector.x < image_width(image_p))
            && (point_p->vector.y >= 0)
            && (point_p->vector.y < image_height(image_p));
}

void point_render(const void* this_p,
                  image_t* image_p,
                  const camera_t* camera_p)
{
    point_t point = *(const point_t*) this_p;
    vector_t image_point = camera_render_point_position(camera_p, image_p, point.vector);
    if(image_point.z < 0)
    {
        return;
    }
    //VI
    int x_image = dither(image_point.x);
    int y_image = dither(image_point.y);

    if(!camera_cache_is_same_image(&camera_cache, image_p))
    {
        if(camera_cache_is_allocated(&camera_cache))
        {
            camera_cache_deallocate(&camera_cache);
        }
        camera_cache_allocate(&camera_cache, image_p);
    }

    point_t* render_point_p = point_init(x_image, y_image, image_point.z, point.colour);
    if(point_is_in_image(render_point_p, image_p))
    {
        if(image_point.z < camera_cache_u_index_get(&camera_cache, x_image, y_image))
        {
            camera_cache_u_index_set(&camera_cache, x_image, y_image, image_point.z);

            (*public_point_renderer)(render_point_p, image_p);
        }
    }
    point_free(render_point_p);
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

void
renderable_cache_clear()
{
    if(camera_cache_is_allocated(&camera_cache))
    {
        camera_cache_deallocate(&camera_cache);
    }
}

static int
camera_cache_is_same_image(const camera_cache_t* camera_cache_p, const image_t* image_p)
{
    return camera_cache_p->current_image_p == image_p;
}

static void
camera_cache_allocate(camera_cache_t* camera_cache_p, const image_t* image_p)
{
    camera_cache_t cache =
    {
            image_width(image_p),
            image_height(image_p),
            image_p,
            NULL,
            NULL
    };
    cache.pixel_u_index = malloc(image_width(image_p) * image_height(image_p) * sizeof(vector_axis_t));
    if(cache.pixel_u_index != NULL)
    {
        for(uint32_t pixel=0; pixel<cache.width * cache.height; ++pixel)
        {
            cache.pixel_u_index[pixel] = INFINITY;
        }
        cache.pixel_u_row = malloc(image_height(image_p) * sizeof(vector_axis_t*));
        if(cache.pixel_u_row != NULL)
        {
            vector_axis_t* row_p = cache.pixel_u_index;
            for(uint32_t row=0; row<cache.height; ++row)
            {
                cache.pixel_u_row[row] = row_p;
                row_p += cache.width;
            }
        }
    }
    *camera_cache_p = cache;
}

static void
camera_cache_deallocate(camera_cache_t* camera_cache_p)
{
    free(camera_cache_p->pixel_u_row);
    free(camera_cache_p->pixel_u_index);
    *camera_cache_p = (camera_cache_t)
    {
        0, 0,
        NULL, NULL, NULL
    };
}

static int
camera_cache_is_allocated(const camera_cache_t* camera_cache_p)
{
    return camera_cache_p->current_image_p != NULL
        && camera_cache_p->pixel_u_index   != NULL
        && camera_cache_p->pixel_u_row     != NULL;
}

static vector_axis_t
camera_cache_u_index_get(const camera_cache_t* camera_cache_p, uint32_t x, uint32_t y)
{
    return camera_cache_p->pixel_u_row[y][x];
}


static void
camera_cache_u_index_set(camera_cache_t* camera_cache_p, uint32_t x, uint32_t y, vector_axis_t u_index)
{
    camera_cache_p->pixel_u_row[y][x] = u_index;
}








