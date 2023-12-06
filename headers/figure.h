/*
 * figure.h
 *
 *  Created on: 5 déc. 2023
 *      Author: moliver
 */

#ifndef HEADERS_FIGURE_H_
#define HEADERS_FIGURE_H_
#include "main.h"

/** Un point dans l'espace
 *
 */
typedef struct
{
    colour_struct_t colour;
    int x;
    int y;
    int z;
} point_t;

/** Une sequence de points
 *
 */
typedef struct
{
    uint32_t amount;
    point_t* sequence;
} figure_t;


#endif /* HEADERS_FIGURE_H_ */
