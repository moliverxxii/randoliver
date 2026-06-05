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

colour_t
palette_colour_reduce(const palette_t* palette_p,
                      colour_t colour,
                      palette_index_method_e method)
{
    const colour_t* colour_p = palette_colour_get(palette_p, palette_index_get(palette_p, colour, method));
    if(colour_p == NULL)
    {
        printf("colour_p error NULL\n");
        colour_p = &COLOUR_BLACK;
    }
    return *colour_p;
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


struct chosen_colour
{
    const colour_t* colour_p;
    uint32_t proximity;
};

static void
update_box(const colour_t* new_colour_p, struct chosen_colour indices[2][COLOUR_INDEX_COUNT], colour_t center, colour_t box[2])
{
    for(colour_index_t colour_index = 0;
        colour_index < COLOUR_INDEX_COUNT;
        ++colour_index)
    {
        if(new_colour_p->array[colour_index] <= center.array[colour_index])
        {
            box[0].array[colour_index] = new_colour_p->array[colour_index];
            indices[0][colour_index].colour_p = new_colour_p;
        }
        else
        {
            box[1].array[colour_index] = new_colour_p->array[colour_index];
            indices[1][colour_index].colour_p = new_colour_p;
        }
    }

}

static palette_index_t
palette_index_get_dither(const palette_t* palette_p, colour_t colour)
{
    #warning occasional segmentation fault
    struct chosen_colour closest_colours_p[2][COLOUR_INDEX_COUNT] =
    {
        {
            {NULL, 0},
            {NULL, 0},
            {NULL, 0}
        },
        {
            {NULL, 0},
            {NULL, 0},
            {NULL, 0}
        }
    };

    colour_t boundary_box[2] = {COLOUR_BLACK, COLOUR_WHITE};

    for(palette_index_t index = 0; index < palette_count(palette_p) - 1; ++index)
    {
        const colour_t* palette_colour_p = palette_colour_get(palette_p, index);

        if(is_in_box(*palette_colour_p, boundary_box))
        {
            update_box(palette_colour_p, closest_colours_p, colour, boundary_box);
        }
    }

    uint32_t colour_distance_meter = UINT32_MAX;

    const colour_t* chosen_colour_p = NULL;


    for(colour_index_t colour_index = 0;
        colour_index < COLOUR_INDEX_COUNT;
        ++colour_index)
    {
        for(uint8_t boundary = 0; boundary < 2; ++boundary)
        {
            if(closest_colours_p[boundary][colour_index].colour_p != NULL)
            {
                if(0)
                {
                    uint32_t distance = colour_distance(colour, *closest_colours_p[boundary][colour_index].colour_p);
                    if(distance < colour_distance_meter)
                    {
                        colour_distance_meter = distance;
                        chosen_colour_p       = closest_colours_p[boundary][colour_index].colour_p;
                    }
                }
                else
                {
                    uint32_t current_distance = colour_distance(colour, *closest_colours_p[boundary][colour_index].colour_p);
                    if(current_distance == 0)
                    {
                         chosen_colour_p = closest_colours_p[boundary][colour_index].colour_p;
                         break;
                    }
                    else
                    {
                        closest_colours_p[boundary][colour_index].proximity = UINT32_MAX / current_distance;
                    }
                }
            }
        }
    }

    if(chosen_colour_p == NULL)
    {
        uint32_t proximity_total = 0;
        //on enleve les doublons.
        for(uint32_t closest_index = 0; closest_index < 6; ++closest_index)
        {
            struct chosen_colour* compare_colour_p = &closest_colours_p[0][0] + closest_index;
            if(compare_colour_p->colour_p != NULL)
            {
                for(uint32_t next_index = closest_index + 1; next_index < 6; ++next_index)
                {
                    struct chosen_colour* next_colour_p = &closest_colours_p[0][0] + next_index;
                    if(compare_colour_p->colour_p == next_colour_p->colour_p)
                    {
                        *next_colour_p = (struct chosen_colour) {NULL, 0};
                    }
                }
                proximity_total += compare_colour_p->proximity;
            }
        }

        uint32_t score = (proximity_total * (float) rand()) / (RAND_MAX);
        uint32_t score_sum = 0;
        struct chosen_colour* element = &closest_colours_p[0][0];
        uint32_t position = 0;
        while(score_sum < proximity_total)
        {
            position = element - &closest_colours_p[0][0];

            if(position >= 6)
            {
                printf("error dickhead!\n");
                break;
            }

            if(score_sum <= score && score < score_sum + element->proximity)
            {
                chosen_colour_p = element->colour_p;
                break;
            }

            score_sum += element->proximity;
            ++element;
        }
    }

    palette_index_t selected_index = chosen_colour_p - palette_colour_get(palette_p, 0);
    return selected_index;
}
