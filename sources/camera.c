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

void
camera_render_point(const camera_t* camera_p,
                    image_t* image_p,
                    point_t point)
{
    if(memcmp(camera_p, &local_camera, sizeof(local_camera)) != 0)
    {
        camera_context_update(camera_p);
        local_camera = *camera_p;
    }

    //I P /1
    vector_t p = point.vector;

    //II PO /1
    vector_t op = vector_subtract(p, camera_context.o);

    //III PO /2
    float op_u_scalaire = vector_scalar(op, camera_context.u);
    float op_v_scalaire = vector_scalar(op, camera_context.v);
    float op_w_scalaire = vector_scalar(op, camera_context.w);

    //IV u>0
    if(op_u_scalaire <= 0)
    {
        return;
    }

    //V
    float scale = (float) image_p->width / (2 * op_u_scalaire * tan(camera_context.angle/2));

    float x_image_scale = -scale * op_v_scalaire;
    float y_image_scale =  scale * op_w_scalaire;

    //VI
    int x_image = dither(x_image_scale + image_p->width / 2);
    int y_image = dither(y_image_scale + image_p->height / 2);

    point_t render_point = point_init(x_image, y_image, 0, point.colour);
    if(point_is_in_image(&render_point, image_p))
    {
        (*public_point_renderer)(image_p, render_point);
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

const uint32_t CAMERA_SUBDIVISION = 400;

void camera_render_edge(const camera_t* camera_p,
                        image_t* image_p,
                        edge_t edge)
{
    figure_t edge_figure = figure_init((uint32_t) CAMERA_SUBDIVISION);

    for(uint32_t point = 0; point < edge_figure.amount; ++point)
    {
        float fraction = (float) point/(edge_figure.amount - 1);

        vector_t average_0 = *edge.array[0];
        average_0 = vector_scale(average_0, fraction);

        vector_t average_1 = *edge.array[1];
        average_1 = vector_scale(average_1, (float) 1 - fraction);

        average_0 = vector_add(average_0, average_1);
        edge_figure.array[point] = point_init(average_0.x, average_0.y, average_0.z, edge.colour);
    }
    camera_render_figure(camera_p, image_p, edge_figure);
    figure_free(&edge_figure);
}

void camera_render_triangle(const camera_t* camera_p,
                            image_t* image_p,
                            triangle_t triangle)
{

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

