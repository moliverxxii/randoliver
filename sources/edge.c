/*
 * edge.c
 *
 *  Created on: 11 févr. 2026
 *      Author: moliver
 */
#include "edge.h"

edge_t edge_init(vector_t* a_p, vector_t* b_p, colour_t colour)
{
    edge_t edge =
    {
        {a_p, b_p},
        colour
    };
    return edge;
}

vector_t edge_get_vector(const edge_t* edge_p, float fraction)
{
    vector_t average_0 = *edge_p->array[0];
    average_0 = vector_scale(average_0, fraction);

    vector_t average_1 = *edge_p->array[1];
    average_1 = vector_scale(average_1, (float) 1 - fraction);

    return  vector_add(average_0, average_1);
}
