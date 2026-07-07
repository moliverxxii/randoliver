/*
 * preset_middle_point.c
 *
 *  Created on: 7 juil. 2026
 *      Author: moliver
 */

#include <stdlib.h>

#include "point.h"
#include "preset.h"
#include "preset_middle_point.h"

static void oli_3d_middle_point_init();
static void oli_3d_middle_point_render(image_t* image_p, uint32_t frame);
static void oli_3d_middle_point_free();

static point_t* p_p;
static camera_t* camera_p;

void
preset_add_middle_point()
{
    preset_add("3D point milieu",
               &oli_3d_middle_point_init,
               NULL,
               &oli_3d_middle_point_render,
               &oli_3d_middle_point_free,
               10000);
}

static void
oli_3d_middle_point_init()
{
    p_p = point_init(0, 0, 0, COLOUR_WHITE);
    camera_p = camera_init(-1,
                           0,
                           0,
                           point_vector(p_p)->x,
                           point_vector(p_p)->y,
                           point_vector(p_p)->z,
                           CAMERA_PROJECTION_PERSPECTIVE,
                           45.0f);
}

static void
oli_3d_middle_point_render(image_t* image_p, uint32_t frame)
{
    (void) frame;
    point_render(p_p, image_p, camera_p);
}

static void
oli_3d_middle_point_free()
{
    point_free(p_p);
    camera_free(camera_p);
}

