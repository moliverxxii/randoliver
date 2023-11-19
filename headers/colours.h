/*
 * colours.h
 *
 *  Created on: 19 nov. 2023
 *      Author: moliver
 */

#ifndef HEADERS_COLOURS_H_
#define HEADERS_COLOURS_H_

#include "main.h"

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
typedef uchar colour_t[COLOUR_COUNT];

/** Couleur sous formes de champs lisibles par un humain.
 *  Compatible pour la conversion avec color_t
 *
 */
typedef struct
{
    uchar blue;
    uchar green;
    uchar red;
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


#endif /* HEADERS_COLOURS_H_ */
