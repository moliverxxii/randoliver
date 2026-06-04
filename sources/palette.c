/*
 * colour_palette.c
 *
 *  Created on: 3 juin 2026
 *      Author: moliver
 */
#include <palette.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"

typedef struct palette_t
{
    uint32_t  count;
    colour_t* colour_array_p;
} palette_t;

static float colour_value_access(const void* value_p);

typedef palette_index_t (*palette_index_finder)(const palette_t* palette_p, colour_t colour);

static palette_index_t palette_index_get_rgb(const palette_t* palette_p, colour_t colour);
static palette_index_t palette_index_get_distance(const palette_t* palette_p, colour_t colour);
static palette_index_t palette_index_get_dither(const palette_t* palette_p, colour_t colour);


static palette_index_finder const INDEX_FINDER_TABLE[PALETTE_INDEX_METHOD_COUNT] =
{
    &palette_index_get_rgb,
    &palette_index_get_distance,
    &palette_index_get_dither
};

uint32_t
palette_get_count(palette_bit_depth_e bit_depth)
{
    return 1 << bit_depth;
}

palette_index_t palette_count(const palette_t* palette_p)
{
    return palette_p->count;
}

palette_t*
palette_init(palette_bit_depth_e bitdepth, uint32_t colour_count)
{
    palette_t* palette_p = NULL;
    if(bitdepth < 16)
    {
        if(colour_count == 0)
        {
            colour_count = palette_get_count(bitdepth);
        }
        palette_p = malloc(sizeof(palette_t));

        if(palette_p != NULL)
        {
            palette_p->count     = colour_count;
            palette_p->colour_array_p = malloc(colour_count * sizeof(colour_t));

            if(palette_p->colour_array_p == NULL)
            {
                free(palette_p);
                palette_p = NULL;
            }
            else
            {
                for(palette_index_t colour = 0; colour < palette_p->count; ++colour)
                {
                    palette_p->colour_array_p[colour] = colour_get_random();
                }
                list_t* colour_list_p;
                list_sort_array(&colour_list_p,
                        palette_p->colour_array_p, sizeof(*palette_p->colour_array_p), palette_p->count,
                        &colour_value_access, SORT_ORDER_ASCENDING);
                free(palette_p->colour_array_p);
                palette_p->colour_array_p = list_array(colour_list_p);
                list_free(colour_list_p);
            }
        }
    }

    return palette_p;
}

palette_index_t
palette_index_get(const palette_t* palette_p, colour_t colour,
                  palette_index_method_e method)
{
    palette_index_t index = 0;
    if(0 < method && method<PALETTE_INDEX_METHOD_COUNT)
    {
        if(INDEX_FINDER_TABLE[method] != NULL)
        {
            index = (*INDEX_FINDER_TABLE[method])(palette_p, colour);
        }
    }
    return index;
}


const colour_t*
palette_colour_get(const palette_t* palette_p,
                   palette_index_t index)
{
    const colour_t* colour_p = NULL;
    if(index < palette_p->count)
    {
        colour_p = palette_p->colour_array_p + index;
    }

    return colour_p;
}

void
palette_print(const palette_t* palette_p)
{
    printf("palette %u colours\n", palette_p->count);
    for(palette_index_t colour = 0; colour<palette_p->count; colour++)
    {
        colour_print(*palette_colour_get(palette_p, colour));
    }
}

static float
colour_value_access(const void* value_p)
{
    int32_t value_int;
    const colour_t* colour_p = value_p;
    memcpy(&value_int, colour_p, sizeof(*colour_p));
    return (float) value_int;
}

static palette_index_t
palette_index_get_rgb(const palette_t* palette_p, colour_t colour)
{
    float value = colour_value_access(&colour);

    palette_index_t index = 0;
    for(index = 0; index < palette_p->count - 1; ++index)
    {
        if(colour_value_access(palette_colour_get(palette_p, index)) <= value)
        {
            break;
        }
    }
    return index;
}

static palette_index_t
palette_index_get_distance(const palette_t* palette_p, colour_t colour)
{
    palette_index_t index = 0;
    palette_index_t selected_index = 0;
    uint32_t distance = UINT32_MAX;

    for(index = 0; index < palette_p->count - 1; ++index)
    {
        uint32_t new_distance = colour_distance(*palette_colour_get(palette_p, index), colour);
        if(new_distance < distance)
        {
            selected_index = index;
            distance = new_distance;
        }

        if(distance == 0)
        {
            break;
        }
    }
    return selected_index;
}

static int
is_in_box(colour_t new_colour, const colour_t boundary_box[2])
{
    int in_box_b = 1;
    for(colour_index_t colour_index = 0;
        colour_index < COLOUR_INDEX_COUNT;
        ++colour_index)
    {
        in_box_b &= (boundary_box[0].array[colour_index] <=      new_colour.array[colour_index])
                 && (     new_colour.array[colour_index] <= boundary_box[1].array[colour_index]);
    }
    return in_box_b;
}

static void
update_box(const colour_t* new_colour, const colour_t* indices[2][COLOUR_INDEX_COUNT], colour_t center, colour_t box[2])
{
    for(colour_index_t colour_index = 0;
        colour_index < COLOUR_INDEX_COUNT;
        ++colour_index)
    {
        if(new_colour->array[colour_index] <= center.array[colour_index])
        {
            box[0].array[colour_index] = new_colour->array[colour_index];
            indices[0][colour_index] = new_colour;
        }
        else
        {
            box[1].array[colour_index] = new_colour->array[colour_index];
            indices[1][colour_index] = new_colour;
        }
    }

}

static palette_index_t
palette_index_get_dither(const palette_t* palette_p, colour_t colour)
{
    const colour_t* indices[2][COLOUR_INDEX_COUNT] =
    {
        {NULL, NULL, NULL},
        {NULL, NULL, NULL}
    };

    colour_t boundary_box[2] = {COLOUR_BLACK, COLOUR_WHITE};

    for(palette_index_t index = 0; index < palette_count(palette_p) - 1; ++index)
    {
        const colour_t* palette_colour_p = palette_colour_get(palette_p, index);

        if(is_in_box(*palette_colour_p, boundary_box))
        {
            update_box(palette_colour_p, indices, colour, boundary_box);
        }
    }

    uint16_t colour_distance = UINT16_MAX;
    uint8_t chosen_colour_index = 0;
    uint8_t chosen_boundary     = 0;

    for(colour_index_t colour_index = 0;
        colour_index < COLOUR_INDEX_COUNT;
        ++colour_index)
    {
        for(uint8_t boundary = 0; boundary < 2; ++boundary)
        {
            if(indices[boundary][colour_index] != NULL)
            {
                int16_t selected_colour = (int16_t) indices[boundary][colour_index]->array[colour_index] - (int16_t) colour.array[colour_index];
                uint16_t distance = selected_colour * selected_colour;
                if(distance < colour_distance)
                {
                    colour_distance     = distance;
                    chosen_colour_index = colour_index;
                    chosen_boundary     = boundary;
                }
            }
        }
    }

    palette_index_t selected_index = indices[chosen_boundary][chosen_colour_index] - palette_colour_get(palette_p, 0);
    return selected_index;
}
