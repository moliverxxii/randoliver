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

