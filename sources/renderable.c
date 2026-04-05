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

static void camera_context_update(const camera_t* camera_p);

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

//#define DITHER

void
render_vector_camera_space(vector_t vector, colour_t colour, image_t* image_p, const camera_t* camera_p)
{
#ifdef DITHER
    int x_image = dither(vector.x);
    int y_image = dither(vector.y);
#else //DITHER
    int x_image = vector.x;
    int y_image = vector.y;
#endif //DITHER



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

    render_vector_camera_space(image_point, colour, image_p, camera_p);
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
renderable_vector_position(vector_t point, image_t* image_p,
                           const camera_t* camera_p)
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
    float scale = (float) image_width(image_p)
                / (2 * op_u_scalaire * tan(M_PI*camera_context.angle/360));

    float x_image_scale =  scale * op_v_scalaire;
    float y_image_scale = -scale * op_w_scalaire;

    x_image_scale += (float) image_width(image_p)  / 2;
    y_image_scale += (float) image_height(image_p) / 2;

    return vector_init(x_image_scale, y_image_scale, op_u_scalaire);
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

