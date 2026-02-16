/*
 * camera.c
 *
 *  Created on: 1 nov. 2025
 *      Author: moliver
 */
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "camera.h"
#include "utility.h"

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

static camera_t local_camera;

static void camera_context_update(const camera_t* camera_p);

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

camera_t
camera_init(float origin_x, float origin_y, float origin_z,
            float destin_x, float destin_y, float destin_z,
            float angle)
{
    camera_t camera =
    {
        {
            {
                origin_x,
                origin_y,
                origin_z
            }
        },
        {
            {
                destin_x,
                destin_y,
                destin_z
            }
        },
        angle
    };
    return camera;
}

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
camera_render_figure(const camera_t* camera_p,
                     image_t* image_p,
                     figure_t figure)
{
    for(uint32_t point = 0; point < figure.amount; ++point)
    {
        camera_render_point(camera_p, image_p, figure.array[point]);
    }
}

const uint32_t CAMERA_SUBDIVISION = 3;

void camera_render_edge(const camera_t* camera_p,
                        image_t* image_p,
                        edge_t edge)
{

    vector_t image_points[]=
    {
        camera_render_point_position(camera_p, image_p, *edge.array[0]),
        camera_render_point_position(camera_p, image_p, *edge.array[1])
    };

    float edge_length = vector_norm(vector_subtract(image_points[1], image_points[0]));
    figure_t edge_figure = figure_init(CAMERA_SUBDIVISION*(uint32_t) edge_length);

    for(uint32_t point = 0; point < edge_figure.amount; ++point)
    {
        float fraction = (float) point/(edge_figure.amount - 1);

        vector_t average = edge_get_vector(&edge, fraction);

        edge_figure.array[point] = point_init(average.x, average.y, average.z, edge.colour);
    }
    camera_render_figure(camera_p, image_p, edge_figure);
    figure_free(&edge_figure);
}

void camera_render_triangle(const camera_t* camera_p,
                            image_t* image_p,
                            triangle_t triangle)
{
    vector_t pivot = *triangle.array[2];

    edge_t slide_edge = edge_init(triangle.array[0], triangle.array[1], triangle.colour);

    vector_t image_points[]=
    {
        camera_render_point_position(camera_p, image_p, *slide_edge.array[0]),
        camera_render_point_position(camera_p, image_p, *slide_edge.array[1])
    };

    float edge_length = vector_norm(vector_subtract(image_points[1], image_points[0]));
    uint32_t slide_points = CAMERA_SUBDIVISION*(uint32_t) edge_length;
    for(uint32_t point = 0; point < slide_points; ++point)
    {
        float fraction = (float) point/(slide_points - 1);

        vector_t average = edge_get_vector(&slide_edge, fraction);
        edge_t render_edge = edge_init(&pivot, &average, triangle.colour);
        camera_render_edge(camera_p, image_p, render_edge);
    }

}

void camera_cache_clear()
{
    if(camera_cache_is_allocated(&camera_cache))
    {
        camera_cache_deallocate(&camera_cache);
    }
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


