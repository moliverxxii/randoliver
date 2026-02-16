/*
 * triangle.c
 *
 *  Created on: 16 févr. 2026
 *      Author: moliver
 */
#include "triangle.h"

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


