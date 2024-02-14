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

/** Couleur en données brutes BVR.
 *
 */
typedef uint8_t colour_t[COLOUR_COUNT];

/** Couleur sous formes de champs lisibles par un humain.
 *  Compatible pour la conversion avec color_t
 *
 */
typedef struct
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} colour_struct_t;

extern const colour_struct_t BLACK;
extern const colour_struct_t WHITE;
extern const colour_struct_t GREY;
extern const colour_struct_t RED;
extern const colour_struct_t GREEN;
extern const colour_struct_t BLUE;
extern const colour_struct_t CYAN;
extern const colour_struct_t MAGENTA;
extern const colour_struct_t YELLOW;

typedef colour_struct_t (*colour_unary_operator)(colour_struct_t, void*);
typedef colour_struct_t (*colour_binary_operator)(colour_struct_t, colour_struct_t, void*);
typedef colour_struct_t (*colour_ternary_operator)(colour_struct_t, colour_struct_t, colour_struct_t, void*);


colour_struct_t get_random_colour();

colour_struct_t random_delta_colour(colour_struct_t colour, void* parameters_p);

colour_struct_t add_colours(colour_struct_t colour_1, colour_struct_t colour_2, void* parameters_p);

#endif /* HEADERS_COLOURS_H_ */
