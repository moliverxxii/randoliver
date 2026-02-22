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
	COLOUR_BLUE = 0,
	COLOUR_GREEN,
	COLOUR_RED,
	COLOUR_COUNT
} pixel_colour_t;

typedef uint8_t colour_value_t;

typedef enum
{
    COLOUR_MIN = 0,
    COLOUR_MAX = UINT8_MAX,
} colour_boundaries_t;

typedef union
{
    struct
    {
        colour_value_t blue;
        colour_value_t green;
        colour_value_t red;
    };
    colour_value_t array[COLOUR_COUNT];
} colour_t;

extern const colour_t BLACK;
extern const colour_t WHITE;
extern const colour_t GREY;
extern const colour_t RED;
extern const colour_t GREEN;
extern const colour_t BLUE;
extern const colour_t CYAN;
extern const colour_t MAGENTA;
extern const colour_t YELLOW;

typedef colour_t (*colour_unary_operator)(colour_t, void*);
typedef colour_t (*colour_binary_operator)(colour_t, colour_t, void*);
typedef colour_t (*colour_ternary_operator)(colour_t, colour_t, colour_t, void*);


colour_t colour_get_random();

colour_t colour_random_delta(colour_t colour, void* parameters_p);

colour_t colour_add_2(colour_t a, colour_t b, void* parameters_p);
colour_t colour_average(colour_t a, colour_t b);

#endif /* HEADERS_COLOURS_H_ */
