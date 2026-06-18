/*
 * camera.c
 *
 *  Created on: 1 nov. 2025
 *      Author: moliver
 */

#include <string.h>
#include "camera.h"

typedef struct camera_t
{
    vector_t origin;
    vector_t direction;
    float angle; //[0, pi]
} camera_t;

camera_t*
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

    camera_t* camera_p = malloc(sizeof(camera));
    if(camera_p != NULL)
    {
        *camera_p = camera;
    }

    return camera_p;
}

camera_t*
camera_copy(const camera_t* camera_p)
{
    camera_t* new_camera_p = malloc(sizeof(*camera_p));
    if(camera_p != NULL)
    {
        *new_camera_p = *camera_p;
    }
    return new_camera_p;
}

void
camera_free(camera_t* camera_p)
{
    free(camera_p);
}


int
camera_is_equal(const camera_t* a_p, const camera_t* b_p)
{
    int is_equal = 0;
    if(a_p != NULL && b_p != NULL)
    {
        is_equal = memcmp(a_p, b_p, sizeof(camera_t)) == 0;
    }
    return is_equal;
}

vector_t*
camera_origin(camera_t* camera_p)
{
    return &camera_p->origin;
}

vector_t*
camera_direction(camera_t* camera_p)
{
    return &camera_p->direction;

}

float
camera_field_of_view(const camera_t* camera_p)
{
    return camera_p->angle;
}
