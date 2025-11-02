/*
 * imgUtil.h
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */

#ifndef IMGUTIL_H_
#define IMGUTIL_H_

#include "colours.h"
#include "figure.h"
#include "transform.h"

#define RES 2835

typedef colour_t* row_t;

/** Conteneur avec les donnees de bases d'une image.
 *  Doit être utilisé pour tous les traitements d'images.
 */
typedef struct
{
    unsigned int width;
    unsigned int height;
    row_t*       image;
} image_t;

/** La definition de l'ecan
 *
 */
extern const image_t SYSTEM_SCREEN;

typedef void (*point_renderer)(const point_t, image_t*);

extern point_renderer public_point_renderer;

/* Elementaires */
//Returns a pointer to an image.
image_t* image_init(uint32_t width, uint32_t height);

//Frees the memory the image occupies in memory.
void image_free(image_t* image);

//Turns the image black.
void image_set(image_t* image);

//Colore aléatorement l'image.
void image_random(image_t* image);
int image_get_sum_colour(const image_t* image_p);

void image_process_1(colour_unary_operator operator, image_t* image_p, void* parameters_p);
void image_process_2(colour_binary_operator operator, image_t* image_1_p, const image_t* image_2_p, void* parameters_p);
void image_process_3(colour_ternary_operator operator,
        image_t* image_1_p,
        const image_t* image_2_p,
        const image_t* image_3_p,
        void* parameters_p);

void image_add(image_t* image_1_p, const image_t* image_2_p);

//Displays the values of the pixels. Line by line.
void image_print(const image_t* image);

//Draws a colored rectangle in an image.
void image_draw_rect(colour_t color, int botLeftX, int botLeftY, int topRightX,
			   int topRightY, image_t* image);

void draw_point(const point_t point, image_t* image_p);
void or_point(const point_t point, image_t* image_p);
void xor_point(const point_t point, image_t* image_p);
void average_point(const point_t point, image_t* image_p);

int point_is_in_image(const point_t* point_p, const image_t* image_p);
// Draws a sequence of points into an image.
void image_draw_figure(image_t* image, const figure_t* figure);

#endif /* IMGUTIL_H_ */
