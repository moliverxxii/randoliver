/*
 * triangle.c
 *
 *  Created on: 16 févr. 2026
 *      Author: moliver
 */
#include "triangle.h"
#include "edge.h"

enum triangle_reference_e
{
    TRIANGLE_REFERENCE_VECTOR_POINTERS = 0,
    TRIANGLE_REFERENCE_VECTOR_LIST
};

static vector_t* triangle_vector(triangle_t* triangle_p, uint32_t index);

typedef struct triangle_t
{
    renderable_i renderable;
    enum triangle_reference_e type;
    union
    {
        vector_t* array[3];
        struct
        {
            vector_t* array_p;
            uint32_t indices[3];
        } list;
    };
    colour_t colour;
} triangle_t;

triangle_t*
triangle_init(vector_t* a_p, vector_t* b_p, vector_t* c_p,
              colour_t colour)
{
    triangle_t* triangle_p = malloc(sizeof(triangle_t));

    if(triangle_p != NULL)
    {
        triangle_t triangle =
        {
            renderable_init(&triangle_render, triangle_p),
            TRIANGLE_REFERENCE_VECTOR_POINTERS,
            {{a_p, b_p, c_p}},
            colour
        };
        *triangle_p = triangle;
    }
    return triangle_p;
}


triangle_t*
triangle_init_list(vector_t* list_p, uint32_t a, uint32_t b, uint32_t c,
              colour_t colour)
{
    triangle_t* triangle_p = malloc(sizeof(triangle_t));

    if(triangle_p != NULL)
    {
        triangle_t triangle =
        {
            renderable_init(&triangle_render, triangle_p),
            TRIANGLE_REFERENCE_VECTOR_LIST,
            {
                .list =
                {
                    list_p,
                    {a, b, c}
                }
            },
            colour
        };
        *triangle_p = triangle;
    }
    return triangle_p;
}

void
triangle_free(triangle_t* triangle_p)
{
    free(triangle_p);
}

static void triangle_render_tent(const triangle_t* triangle_p,
                                 image_t* image_p,
                                 const camera_t* camera_p);

static void triangle_render_crystal(const triangle_t* triangle_p,
                                    image_t* image_p,
                                    const camera_t* camera_p);

typedef void (* triangle_render_f) (const triangle_t* triangle_p,
                                    image_t* image_p,
                                    const camera_t* camera_p);

enum triangle_render_method
{
    TRIANGLE_RENDER_METHOD_TENT = 0,
    TRIANGLE_RENDER_METHOD_CRYSTAL,
    TRIANGLE_RENDER_METHOD_COUNT,
};

static triangle_render_f const TRIANGLE_RENDER_TABLE[TRIANGLE_RENDER_METHOD_COUNT] =
{
    &triangle_render_tent,
    &triangle_render_crystal
};

void
triangle_render(const void* this_p,
                image_t* image_p,
                const camera_t* camera_p)
{
    const triangle_t* triangle_p = this_p;
    (*TRIANGLE_RENDER_TABLE[TRIANGLE_RENDER_METHOD_CRYSTAL])(triangle_p, image_p, camera_p);
}

renderable_i*
triangle_renderable(triangle_t* triangle_p)
{
    return &triangle_p->renderable;
}


static void triangle_render_tent(const triangle_t* triangle_p,
                                 image_t* image_p,
                                 const camera_t* camera_p)
{
    vector_t pivot = *triangle_vector((triangle_t*) triangle_p, 2);

    edge_t* slide_edge_p = edge_init(triangle_vector((triangle_t*) triangle_p, 0),
                                     triangle_vector((triangle_t*) triangle_p, 1),
                                     triangle_p->colour);
    vector_t image_vectors[]=
    {
        renderable_vector_position(edge_get_vector(slide_edge_p, 0), image_p, camera_p),
        renderable_vector_position(edge_get_vector(slide_edge_p, 1), image_p, camera_p)
    };

    float edge_length = vector_norm(vector_subtract(image_vectors[1],
                                                    image_vectors[0]));
    uint32_t slide_points = CAMERA_SUBDIVISION*(uint32_t) edge_length;
    vector_t average = edge_get_vector(slide_edge_p, 0);
    edge_t* render_edge_p = edge_init(&pivot, &average, triangle_p->colour);

    for(uint32_t point = 0; point < slide_points; ++point)
    {
        float fraction = (float) point/(slide_points - 1);
        average = edge_get_vector(slide_edge_p, fraction);

        renderable_render(edge_renderable(render_edge_p), image_p, camera_p);
    }
    edge_free(render_edge_p);
    edge_free(slide_edge_p);
}

static void
triangle_render_crystal(const triangle_t* triangle_p,
                        image_t* image_p,
                        const camera_t* camera_p)
{
    vector_t peak = *triangle_vector((triangle_t*) triangle_p, 2);

    edge_t* slide_edge_1_p = edge_init(triangle_vector((triangle_t*) triangle_p, 0), &peak, triangle_p->colour);
    edge_t* slide_edge_2_p = edge_init(triangle_vector((triangle_t*) triangle_p, 1), &peak, triangle_p->colour);

    vector_t image_vectors[]=
    {
        renderable_vector_position(edge_get_vector(slide_edge_1_p, 0), image_p, camera_p),
        renderable_vector_position(edge_get_vector(slide_edge_1_p, 1), image_p, camera_p)
    };
    float edge_length = vector_norm(vector_subtract(image_vectors[1], image_vectors[0]));
    uint32_t slide_points = CAMERA_SUBDIVISION*(uint32_t) edge_length;

    vector_t average_1;
    vector_t average_2;
    edge_t* render_edge_p = edge_init(&average_1, &average_2, triangle_p->colour);

    for(uint32_t point = 0; point < slide_points; ++point)
    {
        float fraction = (float) point/(slide_points - 1);
        average_1 = edge_get_vector(slide_edge_1_p, fraction);
        average_2 = edge_get_vector(slide_edge_2_p, fraction);

        renderable_render(edge_renderable(render_edge_p), image_p, camera_p);
    }
    edge_free(render_edge_p);
    edge_free(slide_edge_1_p);
    edge_free(slide_edge_2_p);

}

static vector_t*
triangle_vector(triangle_t* triangle_p, uint32_t index)
{
    vector_t* vector_p = NULL;
    if(index < TRIANGLE_POINT_COUNT)
    {
        switch(triangle_p->type)
        {
        case TRIANGLE_REFERENCE_VECTOR_POINTERS:
            vector_p = triangle_p->array[index];
            break;
        case TRIANGLE_REFERENCE_VECTOR_LIST:
            vector_p = triangle_p->list.array_p
                     + triangle_p->list.indices[index];
            break;
        default:
            break;
        }
    }
    return vector_p;
}
