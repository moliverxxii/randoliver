/*
 * camera.h
 *
 *  Created on: 1 nov. 2025
 *      Author: moliver
 */

#ifndef HEADERS_CAMERA_H_
#define HEADERS_CAMERA_H_

#include "figure.h"

/** Une camera a utiliser pour le rendu 3D
 *
 */
typedef struct
{
    vector_t origin;
    vector_t direction;
    float distance;
} camera_t;

/* Rendu */
camera_t camera_init(float origin_x, float origin_y, float origin_z,
                     float destin_x, float destin_y, float destin_z,
                     float distance);



#endif /* HEADERS_CAMERA_H_ */
