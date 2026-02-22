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
typedef struct
{
    uint32_t amount;
    point_t* array;
} figure_t;
typedef struct image_t image_t;
//Initialise une sequence de nombre_point point_t.
figure_t figure_init(uint32_t nombre_point);
figure_t figure_copy(figure_t figure);
void     figure_free(figure_t* figure_p);
figure_t figure_from_image(const image_t*);
vector_t figure_get_average_point(const figure_t* figure_p);

void camera_render_figure(const camera_t* camera_p,
                          image_t* image_p,
                          figure_t figure);

// Draws a sequence of points into an image.
void image_draw_figure(image_t* image, const figure_t* figure);

#endif /* HEADERS_FIGURE_H_ */
