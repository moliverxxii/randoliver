/*
 * solid.c
 *
 *  Created on: 25 févr. 2026
 *      Author: moliver
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "solid.h"
#include "edge.h"

typedef struct solid_t
{
    renderable_i renderable;
    uint32_t vertex_count;
    vector_t* vertex_array_p;
    uint32_t face_count;
    triangle_t** faces_array_p;
    vector_t origin_p;
    float rotation1;
    float rotation2;
    float rotation3;
} solid_t;

static vector_t VERTEX[6];
static edge_t* EDGES[12];
static triangle_t* FACES[8];




static solid_t OCTAHEDRON;
const solid_t* const OCTAHEDRON_P = &OCTAHEDRON;

void
solid_init_const()
{
    vector_t vertices[] =
    {
        vector_init( 1,  0,  0),
        vector_init( 0,  1,  0),
        vector_init(-1,  0,  0),
        vector_init( 0, -1,  0),
        vector_init( 0,  0,  1),
        vector_init( 0,  0, -1)
    };
    memcpy(VERTEX, vertices, sizeof(VERTEX));

    edge_t* edge_array[] =
    {
        edge_init(&VERTEX[0], &VERTEX[1], RED),
        edge_init(&VERTEX[1], &VERTEX[2], RED),
        edge_init(&VERTEX[2], &VERTEX[3], RED),
        edge_init(&VERTEX[3], &VERTEX[0], RED),
        edge_init(&VERTEX[0], &VERTEX[4], BLUE),
        edge_init(&VERTEX[4], &VERTEX[2], BLUE),
        edge_init(&VERTEX[2], &VERTEX[5], BLUE),
        edge_init(&VERTEX[5], &VERTEX[0], BLUE),
        edge_init(&VERTEX[1], &VERTEX[4], GREEN),
        edge_init(&VERTEX[4], &VERTEX[3], GREEN),
        edge_init(&VERTEX[3], &VERTEX[5], GREEN),
        edge_init(&VERTEX[5], &VERTEX[1], GREEN)
    };
    memcpy(EDGES, edge_array, sizeof(EDGES));

    triangle_t* faces[] =
    {
        triangle_init_list(VERTEX, 0, 1, 4, GREEN),
        triangle_init_list(VERTEX, 2, 1, 4, MAGENTA),
        triangle_init_list(VERTEX, 2, 3, 4, BLUE),
        triangle_init_list(VERTEX, 0, 3, 4, RED),
        triangle_init_list(VERTEX, 0, 1, 5, BLUE),
        triangle_init_list(VERTEX, 2, 1, 5, RED),
        triangle_init_list(VERTEX, 2, 3, 5, GREEN),
        triangle_init_list(VERTEX, 0, 3, 5, MAGENTA),
    };
    memcpy(FACES, faces, sizeof(FACES));

    solid_t solid =
    {
        renderable_init(&solid_render, &OCTAHEDRON),
        6,
        VERTEX,
        8,
        FACES,
        VECTOR_0,
        0,0,0
    };

    OCTAHEDRON = solid;
}

solid_t*
solid_init(uint32_t vertex_count, const vector_t* vertices_p,
           uint32_t face_count, const uint32_t **faces_p, const colour_t* colours_p)
{
    solid_t* solid_p = malloc(sizeof(solid_t));
    if(solid_p != NULL)
    {
        solid_t solid =
        {
            renderable_init(solid_render, solid_p),
            vertex_count,
            malloc(vertex_count * sizeof(vector_t)),
            face_count,
            malloc(face_count * sizeof(triangle_t*)),
            VECTOR_0,
            0, 0, 0
        };

        memcpy(solid.vertex_array_p, vertices_p, vertex_count * sizeof(vector_t));

        for(uint32_t face = 0; face < solid.face_count; ++face)
        {
            colour_t colour = colours_p != NULL ? colours_p[face] : colour_get_random();
            solid.faces_array_p[face] = triangle_init_list(solid.vertex_array_p,
                   faces_p[face][0],
                   faces_p[face][1],
                   faces_p[face][2], colour);
        }

        *solid_p = solid;
    }
    return solid_p;
}

void
solid_free(solid_t* solid_p)
{
    for(uint32_t face=0; face < solid_p->face_count; ++face)
    {
        triangle_free(solid_p->faces_array_p[face]);
    }
    free(solid_p->faces_array_p);
    free(solid_p->vertex_array_p);
    free(solid_p);
}

void
solid_render(const void* this_p,
             image_t* image_p,
             const camera_t* camera_p)
{
    const solid_t* solid_p = this_p;
    for(uint32_t face=0; face<solid_p->face_count; ++face)
    {
        triangle_render(solid_p->faces_array_p[face], image_p, camera_p);
    }
}

renderable_i*
solid_renderable(solid_t* solid_p)
{
    return &solid_p->renderable;
}

