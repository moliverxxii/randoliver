/*
 * image_drawing.h
 *
 *  Created on: 2 nov. 2025
 *      Author: moliver
 */

#ifndef HEADERS_IMAGE_DRAWING_H_
#define HEADERS_IMAGE_DRAWING_H_

#include "image.h"

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



#endif /* HEADERS_IMAGE_DRAWING_H_ */
