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

point_t* vector_to_point(point_t* point_p, vector_t vector);
vector_t* point_to_vector(vector_t* vector_p, point_t point);


//Initialise une sequence de nombre_point point_t.
figure_t init_figure(uint32_t nombre_point);
point_t init_point();

#endif /* HEADERS_FIGURE_H_ */
