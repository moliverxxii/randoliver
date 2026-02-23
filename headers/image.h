/*
 * imgUtil.h
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */

#ifndef IMGUTIL_H_
#define IMGUTIL_H_

#include "colours.h"
#include "vector.h"

#define RES 2835

typedef colour_t* row_t;

/** Conteneur avec les donnees de bases d'une image.
 *  Doit être utilisé pour tous les traitements d'images.
 */
typedef struct image_t image_t;

typedef enum
{
    SCALE_ALGORITHM_DUMB = 0,
    SCALE_ALGORITHM_LINEAR,
    SCALE_ALGORITHM_COUNT
} image_scale_algorithm_t;

/** La definition de l'ecan
 *
 */
extern const image_t SYSTEM_SCREEN;

/* Elementaires */
//Returns a pointer to an image.
image_t* image_init(uint32_t width, uint32_t height);

//Frees the memory the image occupies in memory.
void image_free(image_t* image);

//Turns the image black.
void image_set(image_t* image);

colour_t* image_row(const image_t* image_p, uint32_t y);
colour_t* image_data(const image_t* image_p);
uint32_t image_width(const image_t* image_p);
uint32_t image_height(const image_t* image_p);
colour_t image_pixel_get(const image_t* image_p, uint32_t x, uint32_t y);
void     image_pixel_set(image_t* image_p, uint32_t x, uint32_t y, colour_t colour);

void image_scale(image_t* image_p, float scale, image_scale_algorithm_t algorithm);
int image_is_in(const image_t* image_p, int x, int y);

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
void image_draw_rect(colour_t color, uint32_t botLeftX, uint32_t botLeftY, uint32_t topRightX,
			   uint32_t topRightY, image_t* image);


#endif /* IMGUTIL_H_ */
