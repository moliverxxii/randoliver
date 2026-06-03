/*
 * colour_palette.c
 *
 *  Created on: 3 juin 2026
 *      Author: moliver
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "colour_palette.h"
#include "list.h"

typedef struct palette_t
{
    uint32_t  count;
    colour_t* colour_array_p;
} palette_t;

static float colour_value_access(const void* value_p);

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

static float colour_value_access(const void* value_p)
{
    int32_t value_int;
    const colour_t* colour_p = value_p;
    memcpy(&value_int, colour_p, sizeof(*colour_p));
    return (float) value_int;
}

palette_index_t
palette_index_get(const palette_t* palette_p,
                  colour_t colour)
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

void palette_print(const palette_t* palette_p)
{
    printf("palette %u colours\n", palette_p->count);
    for(palette_index_t colour = 0; colour<palette_p->count; colour++)
    {
        colour_print(*palette_colour_get(palette_p, colour));
    }
}
