/*
 * point.c
 *
 *  Created on: 2 nov. 2025
 *      Author: moliver
 */
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "point.h"
#include "utility.h"

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
static void camera_context_update(const camera_t* camera_p);

image_point_renderer public_point_renderer = &image_draw_point;




point_t
point_init(vector_axis_t x, vector_axis_t y, vector_axis_t z, colour_t colour)
{
    point_t point =
    {
        vector_init(x, y, z),
        colour
    };
    return point;
}

void
point_print(point_t point)
{
    printf("(x y z) = (%4f %4f %4f | (R G B) = (%02X %02X %02X))\n",
            point.vector.x, point.vector.y, point.vector.z,
            point.colour.red, point.colour.green, point.colour.blue);
}

void
image_draw_point(image_t* image_p, point_t point)
{
    int x = point.vector.x;
    int y = point.vector.y;
    image_pixel_set(image_p, x, y, point.colour);
}

void
image_or_point(image_t* image_p, point_t point)
{
    int x = point.vector.x;
    int y = point.vector.y;
    colour_t colour = image_pixel_get(image_p, x, y);
    uint32_t buffer = *(uint32_t*) colour.array;

    buffer |= *(uint32_t*) &point.colour;

    memcpy(&colour, &buffer, sizeof(colour));

    image_pixel_set(image_p, x, y, colour);
}


void
image_xor_point(image_t* image_p, point_t point)
{
    int x = point.vector.x;
    int y = point.vector.y;
    colour_t colour = image_pixel_get(image_p, x, y);
    uint32_t buffer = *(uint32_t*) colour.array;

    buffer ^= *(uint32_t*) &point.colour;

    memcpy(&colour, &buffer, sizeof(colour));

    image_pixel_set(image_p, x, y, colour);
}

void
image_average_point(image_t* image_p, point_t point)
{
    int x = point.vector.x;
    int y = point.vector.y;
    image_pixel_set(image_p, x, y, colour_average(image_pixel_get(image_p, x, y), point.colour));
}

int
point_is_in_image(const point_t* point_p, const image_t* image_p)
{
    return (point_p->vector.x >= 0)
            && (point_p->vector.x < image_width(image_p))
            && (point_p->vector.y >= 0)
            && (point_p->vector.y < image_height(image_p));
}

void
camera_render_point(const camera_t* camera_p,
                    image_t* image_p,
                    point_t point)
{
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

    point_t render_point = point_init(x_image, y_image, image_point.z, point.colour);
    if(point_is_in_image(&render_point, image_p))
    {
        if(image_point.z < camera_cache_u_index_get(&camera_cache, x_image, y_image))
        {
            camera_cache_u_index_set(&camera_cache, x_image, y_image, image_point.z);

            (*public_point_renderer)(image_p, render_point);
        }
    }



}

void
camera_cache_clear()
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

static camera_t local_camera;

static struct camera_context_t
{
    float angle;
    vector_t o;
    vector_t f;
    vector_t of;
    float norme_of;
    vector_t u;
    vector_t v;
    vector_t w;
} camera_context;

vector_t
camera_render_point_position(const camera_t* camera_p, image_t* image_p, vector_t point)
{
    if(memcmp(camera_p, &local_camera, sizeof(local_camera)) != 0)
    {
        camera_context_update(camera_p);
        local_camera = *camera_p;
    }

    //I P /1
    vector_t p = point;

    //II PO /1
    vector_t op = vector_subtract(p, camera_context.o);

    //III PO /2
    float op_u_scalaire = vector_scalar(op, camera_context.u);
    float op_v_scalaire = vector_scalar(op, camera_context.v);
    float op_w_scalaire = vector_scalar(op, camera_context.w);

    //IV u>0
    if(op_u_scalaire <= 0)
    {
        return vector_init(0, 0, -1);
    }

    //V
    float scale = (float) image_width(image_p) / (2 * op_u_scalaire * tan(camera_context.angle/2));

    float x_image_scale = -scale * op_v_scalaire;
    float y_image_scale =  scale * op_w_scalaire;

    x_image_scale += (float) image_width(image_p)  / 2;
    y_image_scale += (float) image_height(image_p) / 2;

    return vector_init(x_image_scale, y_image_scale, op_u_scalaire);
}

static void
camera_context_update(const camera_t* camera_p)
{
    float angle = camera_p->angle;
    vector_t o = camera_p->origin;
    vector_t f = camera_p->direction;
    vector_t of = vector_subtract(f, o);

    float norme_of = vector_norm(of);
    vector_t u = vector_scale(of, 1/norme_of);

    vector_t v;
    v.x = -u.y / sqrt(pow(u.x, 2) + pow(u.y, 2));
    v.y =  u.x / sqrt(pow(u.x, 2) + pow(u.y, 2));
    v.z = 0;

    vector_t w = vector_product(u, v);

    struct camera_context_t context =
    {
            angle,
            o,
            f,
            of,
            norme_of,
            u,
            v,
            w
    };
    camera_context = context;
}







