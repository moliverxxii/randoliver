/*
 * triangle.c
 *
 *  Created on: 16 févr. 2026
 *      Author: moliver
 */
#include "triangle.h"
#include "edge.h"

triangle_t
triangle_init(vector_t* a_p, vector_t* b_p, vector_t* c_p,
              colour_t colour)
{
    triangle_t triangle =
    {
        {a_p, b_p, c_p},
        colour
    };
    return triangle;
}

static uint32_t CAMERA_SUBDIVISION = 3;

void
triangle_render(const camera_t* camera_p,
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
        edge_render(camera_p, image_p, render_edge);
    }

}

