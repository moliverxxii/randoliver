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
    vector_t vector;
    colour_struct_t colour;
} point_t;



/** Une sequence de points
 *
 */
typedef struct
{
    uint32_t amount;
    point_t* sequence;
} figure_t;

void print_point(point_t point);


//Initialise une sequence de nombre_point point_t.
figure_t init_figure(uint32_t nombre_point);
point_t init_point();

vector_t get_average_point(const figure_t* figure_p);


#endif /* HEADERS_FIGURE_H_ */
