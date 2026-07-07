/*
 * renderable.c
 *
 *  Created on: 17 févr. 2026
 *      Author: moliver
 */
#include <math.h>
#include <string.h>

#include "renderable.h"
#include "utility.h"
#include "operator.h"

typedef struct
{
    vector_axis_t z; //distance a la camera
    vector_t normal; //vecteur normal a la surface
    vector_t camera_direction;
    colour_t light_colour;
    float    light_power;
} pixel_render_data_t;

typedef struct render_cache_t
{
    const image_t* current_image_p;
    uint32_t width;
    uint32_t height;
    pixel_render_data_t** render_data_row;
} render_cache_t;

static render_cache_t camera_cache =
{
    NULL,
    0,
    0,
    NULL
};

const float CAMERA_SUBDIVISION = 2.5;

static void camera_context_update(const camera_t* camera_p, const image_t* image_p);

static int            render_cache_is_same_image(const render_cache_t* camera_cache_p,
                                                 const image_t* image_p);
static void           render_cache_allocate(render_cache_t* camera_cache_p,
                                            const image_t* image_p);
static void           render_cache_deallocate(render_cache_t* camera_cache_p);
static int            render_cache_is_allocated(const render_cache_t* camera_cache_p);
static vector_axis_t  render_cache_z_index_get(const render_cache_t* camera_cache_p,
                                               uint32_t x, uint32_t y);
static void           render_cache_z_index_set(render_cache_t* camera_cache_p,
                                               uint32_t x, uint32_t y,
                                               vector_axis_t u_index);



renderable_i renderable_init(renderable_render_f render_function, void* this_p)
{
    renderable_i renderable = {render_function, this_p};
    return renderable;
}

void renderable_render(const renderable_i* renderable_p, image_t* image_p,
                       const camera_t* camera_p)
{
    (*renderable_p->render)(renderable_p->this_p, image_p, camera_p);
}

void
renderable_cache_clear()
{
    if(render_cache_is_allocated(&camera_cache))
    {
        render_cache_deallocate(&camera_cache);
    }
}

void
render_vector_camera_space(vector_t vector, colour_t colour, image_t* image_p)
{
    int x_image = vector.x;
    int y_image = vector.y;

    if(!render_cache_is_same_image(&camera_cache, image_p))
    {
        if(render_cache_is_allocated(&camera_cache))
        {
            render_cache_deallocate(&camera_cache);
        }
        render_cache_allocate(&camera_cache, image_p);
    }

    if(image_is_in(image_p, x_image, y_image))
    {
        if(vector.z < render_cache_z_index_get(&camera_cache,
                                               x_image, y_image))
        {
            render_cache_z_index_set(&camera_cache,
                                     x_image, y_image, vector.z);
            image_pixel_set(image_p, x_image, y_image, colour);
        }
    }
}

void
render_vector(vector_t vector, colour_t colour, image_t* image_p, const camera_t* camera_p)
{
    vector_t image_point = renderable_vector_position(vector, image_p, camera_p);
    if(image_point.z < 0)
    {
        return;
    }

    render_vector_camera_space(image_point, colour, image_p);
}

static struct camera_context_t
{
    camera_projection_e projection_type;
    union
    {
        float angle;
        float pixel_per_unit;
    } projection;
    vector_t o;
    vector_t f;
    vector_t of;
    float norme_of;
    operator_t* transformation_p;
    float scale;
} camera_context;

static void
camera_context_update(const camera_t* camera_p, const image_t* image_p)
{
    float angle = camera_field_of_view(camera_p);
    vector_t o = *camera_origin((camera_t*) camera_p);
    vector_t f = *camera_direction((camera_t*) camera_p);
    vector_t of = vector_subtract(f, o);

    float norme_of = vector_norm(of);
    vector_t u = vector_scale(of, 1/norme_of);

    vector_t v;
    v.x = -u.y / sqrt(pow(u.x, 2) + pow(u.y, 2));
    v.y =  u.x / sqrt(pow(u.x, 2) + pow(u.y, 2));
    v.z = 0;

    vector_t w = vector_product(u, v);

    if(camera_context.transformation_p != NULL)
    {
        operator_free(camera_context.transformation_p);
    }

    operator_t* origin_change_p = operator_init_translation(vector_negative(o));
    operator_t* orientation_change_p = operator_init_lines(u, v, w, VECTOR_0);

    operator_t* matrix_p = operator_multiply(orientation_change_p, origin_change_p);
    operator_free(origin_change_p);
    operator_free(orientation_change_p);

    float scale = 1;
    switch(camera_projection(camera_p))
    {
    case CAMERA_PROJECTION_ORTHOGRAPHIC:
        scale = camera_field_of_view(camera_p);
        break;
    case CAMERA_PROJECTION_PERSPECTIVE:
        scale = (float) image_width(image_p) / (2 * tan(M_PI*angle/360));
        break;
    default:
        break;
    }

    struct camera_context_t context =
    {
        camera_projection(camera_p),
        {
            angle
        },
        o,
        f,
        of,
        norme_of,
        matrix_p,
        scale
    };
    camera_context = context;
}

static camera_t* local_camera = NULL;

vector_t
renderable_vector_position(vector_t p, image_t* image_p,
                           const camera_t* camera_p)
{
    if(!camera_is_equal(local_camera, camera_p))
    {
        camera_context_update(camera_p, image_p);
        if(local_camera != NULL)
        {
            camera_free(local_camera);
        }
        local_camera = camera_copy(camera_p);
    }

    vector_t p_2 = VECTOR_0;

    vector_t* p_2_p = operator_operation(camera_context.transformation_p, &p , 1);
    p_2 = *p_2_p;
    free(p_2_p);

    //projection
    float x_image_scale =  p_2.y;
    float y_image_scale = -p_2.z;

    switch(camera_context.projection_type)
    {
    case CAMERA_PROJECTION_PERSPECTIVE:
        x_image_scale /= p_2.x;
        y_image_scale /= p_2.x;
        //fallthrough
    case CAMERA_PROJECTION_ORTHOGRAPHIC:
        x_image_scale *= camera_context.scale;
        y_image_scale *= camera_context.scale;
        break;
    default:
        break;
    }

    //projection dans l'image
    x_image_scale += (float) image_width(image_p)  / 2;
    y_image_scale += (float) image_height(image_p) / 2;

    return vector_init(x_image_scale, y_image_scale, p_2.x);
}

void
render_triangle(image_t* image_p, vector_t a, vector_t b, vector_t c, colour_t colour)
{
    const vector_t* top_p    = NULL;
    const vector_t* bottom_p = NULL;
    const vector_t* middle_p = NULL;

    //points des triangles ordonnes de haut en bas.
    if(a.y < b.y)
    {
        top_p    = &a;
        middle_p = &b;
    }
    else
    {
        top_p    = &b;
        middle_p = &a;
    }

    if(middle_p->y < c.y)
    {
        bottom_p = &c;
    }
    else
    {
        bottom_p = middle_p;
        if(top_p->y < c.y)
        {
            middle_p = &c;
        }
        else
        {
            middle_p = top_p;
            top_p = &c;
        }
    }

    int top_y = (int) ceilf(top_p->y);
    int bottom_y = (int) ceilf(bottom_p->y);

    vector_t top_bottom_v = vector_subtract(*bottom_p, *top_p);
    for(int image_y = top_y; image_y < bottom_y; ++image_y)
    {
        //segment 1
        float coefficient = ((float) image_y - top_p->y)
                          / (bottom_p->y     - top_p->y);
        vector_t p = vector_add(*top_p, vector_scale(top_bottom_v, coefficient));

        //segment 2
        const vector_t* d_p = ((vector_axis_t) image_y < middle_p->y)
                            ? top_p
                            : bottom_p;

        coefficient = ((float) image_y - middle_p->y)
                             / (d_p->y - middle_p->y);

        vector_t middle_d_v = vector_subtract(*d_p, *middle_p);
        vector_t q = vector_add(*middle_p, vector_scale(middle_d_v, coefficient));

        float left  = p.x;
        float right = q.x;
        if(q.x < p.x)
        {
            left  = q.x;
            right = p.x;
        }

        for(int image_x = ceilf(left); image_x < ceilf(right); ++image_x)
        {
            vector_axis_t z = p.z + (q.z - p.z) * (left - p.x) / (q.x - p.x);
            vector_t vector = vector_init(image_x, image_y, z);
            render_vector_camera_space(vector, colour, image_p);
        }
    }

}

static int
render_cache_is_same_image(const render_cache_t* camera_cache_p,
                               const image_t* image_p)
{
    return camera_cache_p->current_image_p == image_p;
}

static void
render_cache_allocate(render_cache_t* camera_cache_p, const image_t* image_p)
{
    render_cache_t cache =
    {
            image_p,
            image_width(image_p),
            image_height(image_p),
            NULL,
    };
    pixel_render_data_t* render_data = malloc(image_width(image_p)
                                 * image_height(image_p)
                                 * sizeof(pixel_render_data_t));
    if(render_data != NULL)
    {
        for(uint32_t pixel=0; pixel<cache.width * cache.height; ++pixel)
        {
            render_data[pixel].z = INFINITY;
        }
        cache.render_data_row = malloc(image_height(image_p)
                              * sizeof(pixel_render_data_t*));
        if(cache.render_data_row != NULL)
        {
            pixel_render_data_t* row_p = render_data;
            for(uint32_t row=0; row<cache.height; ++row)
            {
                cache.render_data_row[row] = row_p;
                row_p += cache.width;
            }
        }
    }
    *camera_cache_p = cache;
}

static void
render_cache_deallocate(render_cache_t* camera_cache_p)
{
    free(*camera_cache_p->render_data_row);
    free(camera_cache_p->render_data_row);
    *camera_cache_p = (render_cache_t)
    {
        NULL,
        0,
        0,
        NULL,
    };
}

static int
render_cache_is_allocated(const render_cache_t* camera_cache_p)
{
    return camera_cache_p->current_image_p  != NULL
        && camera_cache_p->render_data_row  != NULL
        && *camera_cache_p->render_data_row != NULL;
}

static vector_axis_t
render_cache_z_index_get(const render_cache_t* camera_cache_p,
                             uint32_t x, uint32_t y)
{
    return camera_cache_p->render_data_row[y][x].z;
}


static void
render_cache_z_index_set(render_cache_t* camera_cache_p,
                             uint32_t x, uint32_t y, vector_axis_t u_index)
{
    camera_cache_p->render_data_row[y][x].z = u_index;
}

