/*
 * edge.c
 *
 *  Created on: 11 févr. 2026
 *      Author: moliver
 */
#include "edge.h"
#include "figure.h"

edge_t
edge_init(vector_t* a_p, vector_t* b_p, colour_t colour)
{
    edge_t edge =
    {
        {a_p, b_p},
        colour
    };
    return edge;
}

vector_t
edge_get_vector(const edge_t* edge_p, float fraction)
{
    vector_t average_0 = *edge_p->array[0];
    average_0 = vector_scale(average_0, fraction);

    vector_t average_1 = *edge_p->array[1];
    average_1 = vector_scale(average_1, 1.0f - fraction);

    return  vector_add(average_0, average_1);
}

const uint32_t CAMERA_SUBDIVISION = 3;

void
camera_render_edge(const camera_t* camera_p,
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

