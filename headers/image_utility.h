/*
 * imgUtil.h
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */

#ifndef IMGUTIL_H_
#define IMGUTIL_H_

#include "main.h"

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


/** Une camera a utiliser pour le rendu 3D
 *
 */
typedef struct
{
    vector_t origin;
    vector_t direction;
    float distance;
} camera_t;

typedef void (*point_renderer)(const point_t, image_t*);

extern point_renderer public_point_renderer;

/* Elementaires */
//Returns a pointer to an image.
image_t* init_image(int width, int height);

//Turns the image black.
void set_image(image_t* image);

//Displays the values of the pixels. Line by line.
void disp_image(image_t* image);

//Draws a colored rectangle in an image.
void draw_rect(colour_t color, int botLeftX, int botLeftY, int topRightX,
			   int topRightY, image_t* image);
//Frees the memory the image occupies in memory.
void free_image(image_t* image);

void draw_point(const point_t point, image_t* image_p);
void or_point(const point_t point, image_t* image_p);
void xor_point(const point_t point, image_t* image_p);
void average_point(const point_t point, image_t* image_p);

int is_in_image(int x, int y, const image_t* image_p);

// Draws a sequence of points into an image.
void draw_figure(image_t* image, figure_t* figure);

/* Processus */
/**Ajoute un mouvement brownien à l'image de longueur "iterations" et de
 *point
 *de départ (x0,y0).
 */
void brownien1(image_t* image, int iterations, int spread, int x0, int y0);

/**Ajoutes des barres blanches brouillées verticales à l'image.
 *Elles sont espacées centre-à-centre
 *de la distance "spread".
 */
void barres1(image_t* image, int spread);

/**Ajoutes des barres verticales brouillées à l'image.
 *Elles sont espacées centre-à-centre
 *de la distance "spread".
 */
void barres2(image_t* image, int spread);


/* Rendu */
void init_camera(camera_t* camera_p, float origin_x, float origin_y,
        float origin_z, float destin_x, float destin_y, float destin_z,
        float distance);
void render_figure(image_t* image_p, figure_t figure, camera_t camera);

#endif /* IMGUTIL_H_ */
