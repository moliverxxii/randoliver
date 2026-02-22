/*
 * camera.c
 *
 *  Created on: 1 nov. 2025
 *      Author: moliver
 */
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "camera.h"
#include "utility.h"
#include "image.h"

camera_t
camera_init(float origin_x, float origin_y, float origin_z,
            float destin_x, float destin_y, float destin_z,
            float angle)
{
    camera_t camera =
    {
        {
            {
                origin_x,
                origin_y,
                origin_z
            }
        },
        {
            {
                destin_x,
                destin_y,
                destin_z
            }
        },
        angle
    };
    return camera;
}

