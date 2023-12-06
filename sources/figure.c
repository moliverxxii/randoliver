/*
 * figure.c
 *
 *  Created on: 5 déc. 2023
 *      Author: moliver
 */
#include "main.h"


point_t
init_point()
{
    point_t point;
    point.colour = (colour_struct_t
            )
            {0, 0, 0};
    point.x = 0;
    point.y = 0;
    point.z = 0;
    return point;
}

figure_t
init_figure(uint32_t nombre_point)
{
    //Initialise une sequence de nombre_Point point_t.
    figure_t figure;

    figure.amount = nombre_point;
    figure.sequence = (point_t*) malloc(nombre_point * sizeof(point_t));

    int i;
    for(i = 0; i < nombre_point; ++i)
    {
        figure.sequence[i] = init_point();
    }
    return figure;
}



