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
#include "vector.h"

/** Une sequence de points
 *
 */
typedef struct figure_t figure_t;

typedef struct image_t image_t;

//Initialise une sequence de point_t au nombre de point_count.
figure_t* figure_init(uint32_t point_count);
figure_t* figure_copy(const figure_t* figure_p);
void      figure_free(figure_t* figure_p);
figure_t* figure_from_image(const image_t* image_p);
uint32_t  figure_length(const figure_t* figure_p);
point_t*  figure_point(const figure_t* figure_p, uint32_t index);
vector_t  figure_get_average_point(const figure_t* figure_p);

void figure_render(const figure_t* figure_p,
                   image_t* image_p,
                   const camera_t* camera_p);

// Draws a sequence of points into an image.
void figure_draw(const figure_t* figure, image_t* image_p);

#endif /* HEADERS_FIGURE_H_ */
