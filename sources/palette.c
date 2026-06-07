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

typedef struct
{
    const colour_t* colour_p;
    uint32_t proximity;
} colour_candidate;

static float colour_value_access(const void* value_p);

typedef palette_index_t (*palette_index_finder)(const palette_t* palette_p, colour_t colour);

static palette_index_t palette_index_get_rgb(const palette_t* palette_p, colour_t colour);
static palette_index_t palette_index_get_distance(const palette_t* palette_p, colour_t colour);
static palette_index_t palette_index_get_dither(const palette_t* palette_p, colour_t colour);

static int palette_colour_is_in_boundary(colour_t new_colour,
                                  const colour_t boundary_box[2]);
static void palette_colour_update_boundary(const colour_t* new_colour_p,
        colour_candidate indices[2][COLOUR_INDEX_COUNT],
        colour_t center,
        colour_t box[2]);
static const colour_t* palette_colour_proximity_get(
        colour_candidate closest_colours[2][COLOUR_INDEX_COUNT],
        colour_t colour);
static uint32_t palette_colour_candidates_prune(
        colour_candidate closest_colours[2][COLOUR_INDEX_COUNT],
        uint64_t* proximity_total_p);
static const colour_t* palette_colour_candidates_dither(
        colour_candidate closest_colours_p[2][COLOUR_INDEX_COUNT],
        uint64_t proximity_total,
        uint32_t different_indexes);



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

uint8_t
palette_get_bits_per_colour(const palette_t* palette_p)
{
    palette_index_t colours_count = palette_count(palette_p);
    uint8_t bit_count = 0;
    while(colours_count >> (bit_count+1)>0)
    {
        ++bit_count;
    }
    return bit_count;
}


palette_t*
palette_init(palette_bit_depth_e bitdepth, uint32_t colour_count)
{
    palette_t* palette_p = NULL;
    if(bitdepth < PIXEL_BIT_DEPTH_16b)
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
    else
    {
        printf("index too big my boy! : %x\n", index);
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

struct colour_reduce_parameters
{
    palette_t* palette_p;
    palette_index_method_e method;
};

void*
colour_operation_reduce_parameters_init(palette_t* palette_p, palette_index_method_e method)
{
    struct colour_reduce_parameters parameters =
    {
        palette_p,
        method
    };
    struct colour_reduce_parameters* pointer_p = malloc(sizeof(parameters));
    if(pointer_p != NULL)
    {
        *pointer_p = parameters;
    }
    return pointer_p;
}

void
colour_operation_reduce_parameters_free(void* parameters_p)
{
    free(parameters_p);
}


colour_t
colour_operation_reduce(colour_t colour, void* parameters_p)
{
    struct colour_reduce_parameters* values_p = parameters_p;
    return palette_colour_reduce(
            values_p->palette_p,
            colour,
            values_p->method);
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

static palette_index_t
palette_index_get_dither(const palette_t* palette_p, colour_t colour)
{
    colour_candidate closest_colours[2][COLOUR_INDEX_COUNT] =
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

    colour_t colour_boundary[2] = {COLOUR_BLACK, COLOUR_WHITE};

    for(palette_index_t index = 0; index < palette_count(palette_p) - 1; ++index)
    {
        const colour_t* palette_colour_p = palette_colour_get(palette_p, index);

        if(palette_colour_is_in_boundary(*palette_colour_p, colour_boundary))
        {
            palette_colour_update_boundary(palette_colour_p, closest_colours,
                                           colour, colour_boundary);
        }
    }

    const colour_t* chosen_colour_p = palette_colour_proximity_get(closest_colours,
                                                                   colour);

    if(chosen_colour_p == NULL)
    {
        uint64_t proximity_total = 0;
        //on enleve les doublons.
        uint32_t different_indexes = palette_colour_candidates_prune(closest_colours,
                                                                     &proximity_total);

        chosen_colour_p = palette_colour_candidates_dither(closest_colours,
                    proximity_total, different_indexes);
    }

    palette_index_t selected_index = chosen_colour_p - palette_colour_get(palette_p, 0);
    return selected_index;
}

static int
palette_colour_is_in_boundary(colour_t new_colour,
        const colour_t boundary_box[2])
{
    int in_box_b = 1;
    for(colour_index_t colour_index = 0;
        colour_index < COLOUR_INDEX_COUNT;
        ++colour_index)
    {
        in_box_b &= (boundary_box[0].array[colour_index]
                  <=      new_colour.array[colour_index])
                 && (     new_colour.array[colour_index]
                  <= boundary_box[1].array[colour_index]);
    }
    return in_box_b;
}

static void
palette_colour_update_boundary(const colour_t* new_colour_p,
        colour_candidate indices[2][COLOUR_INDEX_COUNT],
        colour_t center,
        colour_t box[2])
{
    for(colour_index_t colour_index = 0;
        colour_index < COLOUR_INDEX_COUNT;
        ++colour_index)
    {
        if(new_colour_p->array[colour_index] < center.array[colour_index])
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

static const colour_t*
palette_colour_proximity_get(
        colour_candidate closest_colours[2][COLOUR_INDEX_COUNT],
        colour_t colour)
{
    const colour_t* chosen_colour_p = NULL;
    for(colour_index_t colour_index = 0;
        colour_index < COLOUR_INDEX_COUNT;
        ++colour_index)
    {
        for(uint8_t boundary = 0; boundary < 2; ++boundary)
        {
            if(closest_colours[boundary][colour_index].colour_p != NULL)
            {
                uint32_t current_distance = colour_distance(colour,
                        *closest_colours[boundary][colour_index].colour_p);
                if(current_distance == 0)
                {
                     chosen_colour_p
                     = closest_colours[boundary][colour_index].colour_p;
                     break;
                }
                else
                {
                    closest_colours[boundary][colour_index].proximity
                     = UINT32_MAX / current_distance;
                }
            }
        }
    }
    return chosen_colour_p;
}

static uint32_t
palette_colour_candidates_prune(
        colour_candidate closest_colours[2][COLOUR_INDEX_COUNT],
        uint64_t* proximity_total_p)
{
    uint32_t candidates = 0;
    for(uint32_t closest_index = 0; closest_index < 6; ++closest_index)
    {
        colour_candidate* compare_colour_p = &closest_colours[0][0] + closest_index;
        if(compare_colour_p->colour_p != NULL)
        {
            for(uint32_t next_index = closest_index + 1; next_index < 6; ++next_index)
            {
                colour_candidate* next_colour_p = &closest_colours[0][0] + next_index;
                if(compare_colour_p->colour_p == next_colour_p->colour_p)
                {
                    *next_colour_p = (colour_candidate) {NULL, 0};
                }
            }
            *proximity_total_p += compare_colour_p->proximity;
        }
        ++candidates;
    }
    return candidates;
}

static const colour_t*
palette_colour_candidates_dither(
        colour_candidate closest_colours_p[2][COLOUR_INDEX_COUNT],
        uint64_t proximity_total,
        uint32_t different_indexes)
{
    const colour_t* chosen_colour_p = NULL;

    uint64_t score = ((uint64_t) proximity_total * (uint64_t) rand())
                   / (uint64_t) RAND_MAX;
    uint64_t score_sum = 0;
    colour_candidate* element = &closest_colours_p[0][0];
    uint32_t position = 0;
    while(score_sum < proximity_total)
    {
        position = element - &closest_colours_p[0][0];

        if(position >= different_indexes)
        {
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
    return chosen_colour_p;
}



