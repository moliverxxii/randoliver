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
    vector_t* array[3];
    colour_t colour;
} triangle_t;

triangle_t*
triangle_init(vector_t* a_p, vector_t* b_p, vector_t* c_p,
              colour_t colour)
{
    triangle_t triangle =
    {
        {a_p, b_p, c_p},
        colour
    };
    triangle_t* triangle_p = malloc(sizeof(triangle));
    if(triangle_p != NULL)
    {
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
triangle_render(const triangle_t* triangle_p,
                image_t* image_p,
                const camera_t* camera_p)
{
    triangle_render_crystal(triangle_p, image_p, camera_p);
}

static void triangle_render_tent(const triangle_t* triangle_p,
                                 image_t* image_p,
                                 const camera_t* camera_p)
{
    vector_t pivot = *triangle_p->array[2];

    edge_t* slide_edge = edge_init(triangle_p->array[0], triangle_p->array[1], triangle_p->colour);
    vector_t image_vectors[]=
    {
        camera_render_point_position(camera_p, image_p, edge_get_vector(slide_edge, 0)),
        camera_render_point_position(camera_p, image_p, edge_get_vector(slide_edge, 1))
    };

    float edge_length = vector_norm(vector_subtract(image_vectors[1], image_vectors[0]));
    uint32_t slide_points = CAMERA_SUBDIVISION*(uint32_t) edge_length;
    vector_t average = edge_get_vector(slide_edge, 0);
    edge_t* render_edge = edge_init(&pivot, &average, triangle_p->colour);

    for(uint32_t point = 0; point < slide_points; ++point)
    {
        float fraction = (float) point/(slide_points - 1);
        average = edge_get_vector(slide_edge, fraction);

        edge_render(render_edge, image_p, camera_p);
    }
    edge_free(render_edge);
    edge_free(slide_edge);
}

static void
triangle_render_crystal(const triangle_t* triangle_p,
                        image_t* image_p,
                        const camera_t* camera_p)
{
    vector_t peak = *triangle_p->array[2];

    edge_t* slide_edge_1 = edge_init(triangle_p->array[0], &peak, triangle_p->colour);
    edge_t* slide_edge_2 = edge_init(triangle_p->array[1], &peak, triangle_p->colour);

    vector_t image_vectors[]=
    {
        camera_render_point_position(camera_p, image_p, edge_get_vector(slide_edge_1, 0)),
        camera_render_point_position(camera_p, image_p, edge_get_vector(slide_edge_1, 1))
    };
    float edge_length = vector_norm(vector_subtract(image_vectors[1], image_vectors[0]));
    uint32_t slide_points = CAMERA_SUBDIVISION*(uint32_t) edge_length;

    vector_t average_1;
    vector_t average_2;
    edge_t* render_edge = edge_init(&average_1, &average_2, triangle_p->colour);

    for(uint32_t point = 0; point < slide_points; ++point)
    {
        float fraction = (float) point/(slide_points - 1);
        average_1 = edge_get_vector(slide_edge_1, fraction);
        average_2 = edge_get_vector(slide_edge_2, fraction);

        edge_render(render_edge, image_p, camera_p);
    }
    edge_free(render_edge);
    edge_free(slide_edge_1);
    edge_free(slide_edge_2);

}
