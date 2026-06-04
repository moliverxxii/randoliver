/*
 * colours.h
 *
 *  Created on: 19 nov. 2023
 *      Author: moliver
 */

#ifndef HEADERS_COLOURS_H_
#define HEADERS_COLOURS_H_

#include <stdint.h>

typedef enum
{
	COLOUR_INDEX_BLUE = 0,
	COLOUR_INDEX_GREEN,
	COLOUR_INDEX_RED,
	COLOUR_INDEX_COUNT
} colour_index_t;

typedef uint8_t colour_value_t;

typedef enum
{
    COLOUR_VALUE_MIN = 0,
    COLOUR_VALUE_MAX = UINT8_MAX,
} colour_boundaries_t;

typedef union
{
    struct
    {
        colour_value_t blue;
        colour_value_t green;
        colour_value_t red;
    };
    colour_value_t array[COLOUR_INDEX_COUNT];
} colour_t;

extern const colour_t COLOUR_BLACK;
extern const colour_t COLOUR_WHITE;
extern const colour_t COLOUR_GREY;
extern const colour_t COLOUR_RED;
extern const colour_t COLOUR_GREEN;
extern const colour_t COLOUR_BLUE;
extern const colour_t COLOUR_CYAN;
extern const colour_t COLOUR_MAGENTA;
extern const colour_t COLOUR_YELLOW;

typedef colour_t (*colour_unary_operator)(colour_t, void*);
typedef colour_t (*colour_binary_operator)(colour_t, colour_t, void*);
typedef colour_t (*colour_ternary_operator)(colour_t, colour_t, colour_t, void*);

colour_t colour_init(colour_value_t red,
                     colour_value_t green,
                     colour_value_t blue);

colour_t colour_get_random();

void colour_print(colour_t colour);
uint32_t colour_distance(colour_t a, colour_t b);

colour_t colour_random_delta(colour_t colour, void* parameters_p);

colour_t colour_add_2(colour_t a, colour_t b, void* parameters_p);
colour_t colour_average(colour_t a, colour_t b);

#endif /* HEADERS_COLOURS_H_ */
