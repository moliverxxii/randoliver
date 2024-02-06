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

colour_struct_t get_random_colour();

#endif /* HEADERS_COLOURS_H_ */
