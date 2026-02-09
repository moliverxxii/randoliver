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

static struct camera_context_t
{
    float distance;
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
            float distance)
{
    camera_t camera =
    {
        {
            origin_x,
            origin_y,
            origin_z
        },
        {
            destin_x,
            destin_y,
            destin_z
        },
        distance
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
    vector_t p = point.vector;

    vector_t op;
    vector_subtract(&op, p, camera_context.o);

    float op_u_scalaire = vector_scalar(op, camera_context.u);
    float op_v_scalaire = vector_scalar(op, camera_context.v);
    float op_w_scalaire = vector_scalar(op, camera_context.w);

    int x_image = SYSTEM_SCREEN.width / 2
            + camera_context.distance * op_v_scalaire / op_u_scalaire;
    int y_image = SYSTEM_SCREEN.height / 2
            + camera_context.distance * op_w_scalaire / op_u_scalaire;

    if(op_u_scalaire <= 0)
    {
        return;
    }

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
    printf("Render start.\n");

    for(uint32_t point = 0; point < figure.amount; ++point)
    {
        camera_render_point(camera_p, image_p, figure.sequence[point]);
    }
    printf("Render end.\n");

}

static void
camera_context_update(const camera_t* camera_p)
{
    float distance = camera_p->distance;
    vector_t o = camera_p->origin;
    vector_t f = camera_p->direction;
    vector_t of;
    vector_subtract(&of, f, o);

    float norme_of = vector_norm(of);
    vector_t u;
    vector_scale(&u, of, 1/norme_of);

    vector_t v;
    v.x = -u.y / sqrt(pow(u.x, 2) + pow(u.y, 2));
    v.y =  u.x / sqrt(pow(u.x, 2) + pow(u.y, 2));
    v.z = 0;

    vector_t w;
    w.x = -u.z * u.x / sqrt(pow(u.x, 2) + pow(u.y, 2));
    w.y =  u.z * u.y / sqrt(pow(u.x, 2) + pow(u.y, 2));
    w.z = sqrt(pow(u.x, 2) + pow(u.y, 2));

    struct camera_context_t context =
    {
            distance,
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

