/*
 * triangle.c
 *
 *  Created on: 16 févr. 2026
 *      Author: moliver
 */
#include "triangle.h"
#include "edge.h"

typedef struct triangle_t
{
    renderable_i renderable;
    vector_t* array[3];
    colour_t colour;
} triangle_t;

triangle_t*
triangle_init(vector_t* a_p, vector_t* b_p, vector_t* c_p,
              colour_t colour)
{
    triangle_t* triangle_p = malloc(sizeof(triangle_t));

    if(triangle_p != NULL)
    {
        triangle_t triangle =
        {
            renderable_init(&triangle_render, triangle_p),
            {a_p, b_p, c_p},
            colour
        };
        *triangle_p = triangle;
    }
    return triangle_p;
}

void
triangle_free(triangle_t* triangle_p)
{
    free(triangle_p);
}

static void triangle_render_tent(const triangle_t* triangle_p,
                                 image_t* image_p,
                                 const camera_t* camera_p);

static void triangle_render_crystal(const triangle_t* triangle_p,
                                    image_t* image_p,
                                    const camera_t* camera_p);

void
triangle_render(const void* this_p,
                image_t* image_p,
                const camera_t* camera_p)
{
    const triangle_t* triangle_p = this_p;
    triangle_render_crystal(triangle_p, image_p, camera_p);
}

renderable_i*
triangle_renderable(triangle_t* triangle_p)
{
    return &triangle_p->renderable;
}


static void triangle_render_tent(const triangle_t* triangle_p,
                                 image_t* image_p,
                                 const camera_t* camera_p)
{
    vector_t pivot = *triangle_p->array[2];

    edge_t* slide_edge_p = edge_init(triangle_p->array[0], triangle_p->array[1], triangle_p->colour);
    vector_t image_vectors[]=
    {
        renderable_vector_position(edge_get_vector(slide_edge_p, 0), image_p, camera_p),
        renderable_vector_position(edge_get_vector(slide_edge_p, 1), image_p, camera_p)
    };

    float edge_length = vector_norm(vector_subtract(image_vectors[1], image_vectors[0]));
    uint32_t slide_points = CAMERA_SUBDIVISION*(uint32_t) edge_length;
    vector_t average = edge_get_vector(slide_edge_p, 0);
    edge_t* render_edge_p = edge_init(&pivot, &average, triangle_p->colour);

    for(uint32_t point = 0; point < slide_points; ++point)
    {
        float fraction = (float) point/(slide_points - 1);
        average = edge_get_vector(slide_edge_p, fraction);

        renderable_render(edge_renderable(render_edge_p), image_p, camera_p);
    }
    edge_free(render_edge_p);
    edge_free(slide_edge_p);
}

static void
triangle_render_crystal(const triangle_t* triangle_p,
                        image_t* image_p,
                        const camera_t* camera_p)
{
    vector_t peak = *triangle_p->array[2];

    edge_t* slide_edge_1_p = edge_init(triangle_p->array[0], &peak, triangle_p->colour);
    edge_t* slide_edge_2_p = edge_init(triangle_p->array[1], &peak, triangle_p->colour);

    vector_t image_vectors[]=
    {
        renderable_vector_position(edge_get_vector(slide_edge_1_p, 0), image_p, camera_p),
        renderable_vector_position(edge_get_vector(slide_edge_1_p, 1), image_p, camera_p)
    };
    float edge_length = vector_norm(vector_subtract(image_vectors[1], image_vectors[0]));
    uint32_t slide_points = CAMERA_SUBDIVISION*(uint32_t) edge_length;

    vector_t average_1;
    vector_t average_2;
    edge_t* render_edge_p = edge_init(&average_1, &average_2, triangle_p->colour);

    for(uint32_t point = 0; point < slide_points; ++point)
    {
        float fraction = (float) point/(slide_points - 1);
        average_1 = edge_get_vector(slide_edge_1_p, fraction);
        average_2 = edge_get_vector(slide_edge_2_p, fraction);

        renderable_render(edge_renderable(render_edge_p), image_p, camera_p);
    }
    edge_free(render_edge_p);
    edge_free(slide_edge_1_p);
    edge_free(slide_edge_2_p);

}
