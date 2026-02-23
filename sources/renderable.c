/*
 * renderable.c
 *
 *  Created on: 17 févr. 2026
 *      Author: moliver
 */
#include <math.h>
#include <string.h>

#include "renderable.h"

static void camera_context_update(const camera_t* camera_p);

renderable_i renderable_init(renderable_render_f render_function, void* this_p)
{
    renderable_i renderable = {render_function, this_p};
    return renderable;
}

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

static camera_t local_camera;

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

void renderable_render(const renderable_i* this_p, image_t* image_p, const camera_t* camera_p)
{
    (*this_p->render)(this_p->this_p, image_p, camera_p);
}
