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
            float distance)
{
    camera_t camera_loc =
    {
        {
            origin_x,
            origin_y,
            origin_z
        },
        {
            destin_x,
            destin_y,
            destin_z
        },
        distance
    };
    return camera_loc;
}


