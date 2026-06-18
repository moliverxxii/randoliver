/*
 * camera.h
 *
 *  Created on: 1 nov. 2025
 *      Author: moliver
 */

#ifndef HEADERS_CAMERA_H_
#define HEADERS_CAMERA_H_

#include "vector.h"

/** Une camera a utiliser pour le rendu 3D
 *
 */
typedef struct camera_t camera_t;

/* Rendu */
camera_t* camera_init(float origin_x, float origin_y, float origin_z,
                     float destin_x, float destin_y, float destin_z,
                     float field_of_view);

camera_t* camera_copy(const camera_t* camera_p);
void camera_free(camera_t* camera_p);

int camera_is_equal(const camera_t* a_p, const camera_t* b_p);
vector_t* camera_origin(camera_t* camera_p);
vector_t* camera_direction(camera_t* camera_p);
float camera_field_of_view(const camera_t* camera_p);





#endif /* HEADERS_CAMERA_H_ */
