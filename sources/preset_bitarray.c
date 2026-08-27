/*
 * preset_bitarray.c
 *
 *  Created on: 13 juil. 2026
 *      Author: moliver
 */
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "bitarray.h"
#include "preset.h"

#define CHARACTER_COUNT 40


typedef struct
{
    uint32_t underpopulation; //start
    uint32_t reproduction_start; //start
    uint32_t reproduction_end; //start
    uint32_t overpopulation; //start
    bitarray_t* kernel_p;
} game_rules_t;

static void oli_bitarray_init();
static void oli_bitarray(image_t* image_p, uint32_t frame);
static void oli_bitarray_update();

static void game_rules_print(const game_rules_t* rules_p);

void
preset_add_bitarray()
{
    preset_add("bitarray", &oli_bitarray_init, &oli_bitarray_update, &oli_bitarray, NULL, 1000);
}

static game_rules_t rules =
{
        0,
        0,
        0,
        0,
        NULL
};

static uint32_t neighbourhood_horizontal = 0;
static uint32_t neighbourhood_vertical   = 0;
static bitarray_t* neighbourhood_p = NULL;

static const char * const GAME_PARAMETERS_FORMAT = "%u , %u , %u, %u";

static void
oli_bitarray_init()
{
    char line_p[CHARACTER_COUNT + 1];

    //regles
    FILE* rule_file_p = fopen("kernel.txt", "r");
    fgets(line_p, CHARACTER_COUNT, rule_file_p);
    sscanf(line_p, GAME_PARAMETERS_FORMAT,
           &rules.underpopulation,
           &rules.reproduction_start,
           &rules.reproduction_end,
           &rules.overpopulation);

    game_rules_print(&rules);

    uint32_t rows_pp[40][40] = {{0}};
    uint32_t line = 0;
    int line_valid = 0;
    while(NULL != fgets(line_p, CHARACTER_COUNT, rule_file_p))
    {
        for(uint32_t index = 0; index < strlen(line_p); ++index)
        {
            if(isspace(line_p[index]))
            {
                break;
            }

            line_valid = 1;
            int is_on = line_p[index] == '1';
            printf("%1c", is_on ? '1' : ' ');
            if(index + 1 >= neighbourhood_horizontal)
            {
                neighbourhood_horizontal = index + 1;
            }
            rows_pp[line][index] = is_on;
        }
        if(line_valid)
        {
            ++neighbourhood_vertical;
            ++line;
            line_valid = 0;
            printf("\n");
        }
    }

    fclose(rule_file_p);

    rules.kernel_p = bitarray_init(neighbourhood_horizontal, neighbourhood_vertical);
    neighbourhood_p = bitarray_init(neighbourhood_horizontal, neighbourhood_vertical);

    for(uint32_t n_x = 0; n_x < neighbourhood_horizontal; ++n_x)
    {
        for(uint32_t n_y = 0; n_y < neighbourhood_vertical; ++n_y)
        {
            if(rows_pp[n_y][n_x])
            {
                bitarray_bit_set(rules.kernel_p, n_x, n_y);
            }
            else
            {
                bitarray_bit_reset(rules.kernel_p, n_x, n_y);
            }
        }
    }
    bitarray_print(rules.kernel_p);
}


static bitarray_t* array_p = NULL;
static bitarray_t* dest_array_p = NULL;
static uint32_t width  = 0;
static uint32_t height = 0;

static void
oli_bitarray_update()
{
    if(array_p == NULL)
    {
        return;
    }

    for(uint32_t x = 0; x < width; ++x)
    {
        for(uint32_t y = 0; y < height; ++y)
        {
            bitarray_extract(neighbourhood_p,
                             array_p,
                             (int32_t)x-(int32_t) neighbourhood_horizontal/2,
                             (int32_t)y-(int32_t) neighbourhood_vertical/2);
            bitarray_and(neighbourhood_p, rules.kernel_p);
            uint32_t count = bitarray_on_get(neighbourhood_p);
            int value = 0;

            value = bitarray_bit_get(array_p, x, y);

            if(value)
            {
                if(count <= rules.underpopulation || rules.overpopulation <= count)
                {
                    value = 0;
                }
            }
            else
            {
                if(rules.reproduction_start <= count && count <= rules.reproduction_end)
                {
                    value = 1;
                }

            }

            if(value)
            {
                bitarray_bit_set(dest_array_p, x, y);
            }
            else
            {
                bitarray_bit_reset(dest_array_p, x, y);
            }
        }
    }
    bitarray_extract(array_p, dest_array_p, 0, 0);
}

static void
game_rules_print(const game_rules_t* rules_p)
{
    printf("rules: %u,%u,%u,%u\n",
           rules_p->underpopulation,
           rules_p->reproduction_start,
           rules_p->reproduction_end,
           rules_p->overpopulation);
}

static int probability(float p)
{
    int value = rand();
    int threshold = p*RAND_MAX;

    return value <= threshold;
}

static void
oli_bitarray(image_t* image_p, uint32_t frame)
{
    (void) frame;
    if(array_p == NULL)
    {
        width        = image_width(image_p);
        height       = image_height(image_p);
        array_p      = bitarray_init(width, height);
        dest_array_p = bitarray_init(width, height);
        for(uint32_t x = 0; x < image_width(image_p); ++x)
        {
            float probability_x = (float) ((x + width/2)%width) / (width - 1);

            for(uint32_t y = 0; y < image_height(image_p); ++y)
            {
                if(probability(probability_x))
                {
                    bitarray_bit_set(array_p, x, y);
                }
                else
                {
                    bitarray_bit_reset(array_p, x, y);
                }
            }

        }
    }

    for(uint32_t x = 0; x < image_width(image_p); ++x)
    {
        for(uint32_t y = 0; y < image_height(image_p); ++y)
        {
            colour_t colour = bitarray_bit_get(array_p, x, y) ? COLOUR_WHITE : COLOUR_BLACK;
            image_pixel_set(image_p, x, y, colour);
        }
    }
}

