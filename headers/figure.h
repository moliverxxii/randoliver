/*
 * figure.h
 *
 *  Created on: 5 déc. 2023
 *      Author: moliver
 */

#ifndef HEADERS_FIGURE_H_
#define HEADERS_FIGURE_H_

#include <stdint.h>

#include "point.h"
#include "colours.h"
#include "transform.h"

/** Une sequence de points
 *
 */
typedef struct
{
    uint32_t amount;
    point_t* sequence;
} figure_t;

//Initialise une sequence de nombre_point point_t.
figure_t figure_init(uint32_t nombre_point);

vector_t figure_get_average_point(const figure_t* figure_p);


#endif /* HEADERS_FIGURE_H_ */
