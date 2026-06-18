/*
 * solid_plane.c
 *
 *  Created on: 15 juin 2026
 *      Author: moliver
 */
#include <string.h>

#include "solid_plane.h"

solid_t*
solid_plane_init(uint32_t L, uint32_t H, vector_axis_t l_u, vector_axis_t h_u)
{
    //sommets
    uint32_t L_N = L + 1;
    uint32_t H_N = H + 1;
    uint32_t LH_N = L_N * H_N;
    vector_t* vector_array_p = malloc(LH_N * sizeof(vector_t));

    float step_x = l_u/L;
    float step_y = h_u/H;
    vector_t vector_step_x = vector_scale(VECTOR_X, step_x);
    vector_t vector_step_y = vector_scale(VECTOR_Y, step_y);

    vector_t vector_y = VECTOR_0;
    for(uint32_t y = 0; y < H_N;
        ++y, vector_y = vector_add(vector_y, vector_step_y))
    {
        vector_t vector_x = VECTOR_0;
        for(uint32_t x = 0; x < L_N;
            ++x, vector_x = vector_add(vector_x, vector_step_x))
        {
            vector_array_p[y*L_N + x] = vector_add(vector_x, vector_y);
        }
    }

    //faces
    uint32_t S = L * H;
    uint32_t T = 2 * S;
    const uint32_t** triangle_vertices_pp = malloc(T * sizeof(uint32_t*));
    uint32_t* indices_p = malloc(3 * T * sizeof(uint32_t));
    colour_t* colours_p = malloc (T * sizeof(colour_t));

    for(uint32_t y = 0; y < H; ++y)
    {
        for(uint32_t x = 0; x < L; ++x)
        {
            uint32_t quad = y * L + x;

            uint32_t* triangle_1_p = &indices_p[3*2*quad];
            uint32_t* triangle_2_p = &indices_p[3*(2*quad+1)];
            uint32_t base_index = y * L_N + x;
            uint32_t triangle_1[3] =
            {
                 base_index,
                 base_index + 1,
                 base_index + L_N
            };
            memcpy(triangle_1_p, triangle_1, sizeof(triangle_1));

            uint32_t triangle_2[3] =
            {
                    base_index + 1,
                    base_index + L_N,
                    base_index + L_N + 1
            };
            memcpy(triangle_2_p, triangle_2, sizeof(triangle_2));

            triangle_vertices_pp[2 * quad]     = triangle_1_p;
            triangle_vertices_pp[2 * quad + 1] = triangle_2_p;
            colour_t colour = colour_init((COLOUR_VALUE_MAX * x)/L,
                                          (COLOUR_VALUE_MAX * y)/H,
                                          0);
            colours_p[2 * quad]     = colour;
            colours_p[2 * quad + 1] = colour;
        }
    }
    solid_t* solid_p = solid_init(LH_N, vector_array_p, T, triangle_vertices_pp, colours_p);
    free(vector_array_p);
    free(triangle_vertices_pp);
    free(indices_p);
    free(colours_p);
    return solid_p;
}

void
solid_plane_free()
{

}
