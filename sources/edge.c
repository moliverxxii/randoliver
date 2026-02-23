/*
 * edge.c
 *
 *  Created on: 11 févr. 2026
 *      Author: moliver
 */
#include "edge.h"
#include "figure.h"

typedef struct edge_t
{
    vector_t* array[2];
    colour_t colour;
} edge_t;


edge_t*
edge_init(vector_t* a_p, vector_t* b_p, colour_t colour)
{
    edge_t edge =
    {
        {a_p, b_p},
        colour
    };
    edge_t* edge_p = malloc(sizeof(edge));
    if(edge_p != NULL)
    {
        *edge_p = edge;
    }
    return edge_p;
}

void edge_free(edge_t* edge_p)
{
    free(edge_p);
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
edge_render(const edge_t* edge_p,
            image_t* image_p,
            const camera_t* camera_p)
{

    vector_t image_points[]=
    {
        camera_render_point_position(camera_p, image_p, *edge_p->array[0]),
        camera_render_point_position(camera_p, image_p, *edge_p->array[1])
    };

    float edge_length = vector_norm(vector_subtract(image_points[1], image_points[0]));
    uint32_t point_count = CAMERA_SUBDIVISION * (uint32_t) edge_length;
    for(uint32_t point = 0; point < point_count; ++point)
    {
        float fraction = (float) point/(point_count - 1);

        vector_t average = edge_get_vector(edge_p, fraction);
        point_t* edge_point_p = point_init(average.x, average.y, average.z, edge_p->colour);
        renderable_render(point_renderable(edge_point_p), image_p, camera_p);
        point_free(edge_point_p);
    }
}

