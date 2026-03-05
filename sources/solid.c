/*
 * solid.c
 *
 *  Created on: 25 févr. 2026
 *      Author: moliver
 */

#include <stdint.h>
#include <string.h>

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



static uint32_t solid_count_vertex(uint32_t face_count, triangle_t* faces_p);

static vector_t VERTEX[6];
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
        edge_init(&vertices[0], &vertices[1], RED),
        edge_init(&vertices[1], &vertices[2], RED),
        edge_init(&vertices[2], &vertices[3], RED),
        edge_init(&vertices[3], &vertices[0], RED),
        edge_init(&vertices[0], &vertices[4], BLUE),
        edge_init(&vertices[4], &vertices[2], BLUE),
        edge_init(&vertices[2], &vertices[5], BLUE),
        edge_init(&vertices[5], &vertices[0], BLUE),
        edge_init(&vertices[1], &vertices[4], GREEN),
        edge_init(&vertices[4], &vertices[3], GREEN),
        edge_init(&vertices[3], &vertices[5], GREEN),
        edge_init(&vertices[5], &vertices[1], GREEN)
    };

    triangle_t* faces[] =
    {
        triangle_init(&vertices[0],
                      &vertices[1],
                      &vertices[4], GREEN),
        triangle_init(&vertices[2],
                      &vertices[1],
                      &vertices[4], MAGENTA),
        triangle_init(&vertices[2],
                      &vertices[3],
                      &vertices[4], BLUE),
        triangle_init(&vertices[0],
                      &vertices[3],
                      &vertices[4], RED),
        triangle_init(&vertices[0],
                      &vertices[1],
                      &vertices[5], BLUE),
        triangle_init(&vertices[2],
                      &vertices[1],
                      &vertices[5], RED),
        triangle_init(&vertices[2],
                      &vertices[3],
                      &vertices[5], GREEN),
        triangle_init(&vertices[0],
                      &vertices[3],
                      &vertices[5], MAGENTA),
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
solid_init(uint32_t face_count, triangle_t* faces_p)
{
    return NULL;
}

void
solid_free(solid_t* solid_p)
{
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

