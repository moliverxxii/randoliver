/*
 * edge.c
 *
 *  Created on: 11 févr. 2026
 *      Author: moliver
 */
#include "edge.h"

typedef struct edge_t
{
    renderable_i renderable;
    vector_t* array[2];
    colour_t colour;
} edge_t;


edge_t*
edge_init(vector_t* a_p, vector_t* b_p, colour_t colour)
{
    edge_t* edge_p = malloc(sizeof(edge_t));
    if(edge_p != NULL)
    {
        edge_t edge =
        {
            renderable_init(&edge_render, edge_p),
            {a_p, b_p},
            colour
        };
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

void
edge_render(const void* this_p,
            image_t* image_p,
            const camera_t* camera_p)
{

    const edge_t* edge_p = this_p;

    vector_t image_points[]=
    {
        renderable_vector_position(*edge_p->array[0], image_p, camera_p),
        renderable_vector_position(*edge_p->array[1], image_p, camera_p)
    };

    float edge_length = vector_norm(vector_subtract(image_points[1], image_points[0]));
    uint32_t point_count = CAMERA_SUBDIVISION * (uint32_t) edge_length;
    for(uint32_t point = 0; point < point_count; ++point)
    {
        float fraction = (float) point/(point_count - 1);

        vector_t average = edge_get_vector(edge_p, fraction);
        render_vector(average, edge_p->colour, image_p, camera_p);
    }
}

renderable_i*
edge_renderable(edge_t* edge_p)
{
    return &edge_p->renderable;
}
