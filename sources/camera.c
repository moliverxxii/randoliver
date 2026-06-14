/*
 * camera.c
 *
 *  Created on: 1 nov. 2025
 *      Author: moliver
 */

#include "camera.h"

camera_t
camera_init(float origin_x, float origin_y, float origin_z,
            float destin_x, float destin_y, float destin_z,
            float angle)
{
    camera_t camera =
    {
        vector_init(origin_x, origin_y, origin_z),
        vector_init(destin_x, destin_y, destin_z),
        angle
    };
    return camera;
}

