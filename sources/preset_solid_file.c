/*
 * preset_solid_file.c
 *
 *  Created on: 8 juil. 2026
 *      Author: moliver
 */
#include "camera.h"
#include "solid_file.h"
#include "preset.h"

static void oli_solid_init();
static void oli_solid_render(image_t* image_p, uint32_t frame);
static void oli_solid_free();



void
preset_add_solid_file()
{
    preset_add("solid file",
               &oli_solid_init,
               NULL,
               &oli_solid_render,
               &oli_solid_free,
               1);
}

static solid_t* solid_p = NULL;
static camera_t* camera_p = NULL;


static void
oli_solid_init()
{
    solid_init_const();

    solid_p = solid_file_open("scene.txt");

    camera_p = camera_init(-5, -1.5, -1,
                           0, 0, 0,
                           CAMERA_PROJECTION_PERSPECTIVE,
                           45);
}

static void
oli_solid_render(image_t* image_p, uint32_t frame)
{
    (void) frame;
    renderable_cache_clear();

    solid_render(solid_p, image_p, camera_p);
}

static void
oli_solid_free()
{
    solid_free(solid_p);
    camera_free(camera_p);
}


