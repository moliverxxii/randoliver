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

static uint32_t CAMERA_SUBDIVISION = 3;

void
triangle_render(const triangle_t* triangle,
                image_t* image_p,
                const camera_t* camera_p)
{
    vector_t pivot = *triangle->array[2];

    edge_t* slide_edge = edge_init(triangle->array[0], triangle->array[1], triangle->colour);

    vector_t image_vectors[]=
    {
        camera_render_point_position(camera_p, image_p, edge_get_vector(slide_edge, 0)),
        camera_render_point_position(camera_p, image_p, edge_get_vector(slide_edge, 1))
    };

    float edge_length = vector_norm(vector_subtract(image_vectors[1], image_vectors[0]));
    uint32_t slide_points = CAMERA_SUBDIVISION*(uint32_t) edge_length;
    vector_t average = edge_get_vector(slide_edge, 0);
    edge_t* render_edge = edge_init(&pivot, &average, triangle->colour);

    for(uint32_t point = 0; point < slide_points; ++point)
    {
        float fraction = (float) point/(slide_points - 1);
        average = edge_get_vector(slide_edge, fraction);

        edge_render(render_edge, image_p, camera_p);
    }
    edge_free(render_edge);
    edge_free(slide_edge);

}

