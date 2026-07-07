/*
 * preset_plane.c
 *
 *  Created on: 7 juil. 2026
 *      Author: moliver
 */

#include "camera.h"
#include "image_drawing.h"
#include "preset.h"
#include "solid.h"
#include "solid_file.h"
#include "solid_plane.h"

static void oli_plane_init();
static void oli_plane_init_2();
static void oli_plane_render(image_t* image_p, uint32_t frame);
static void oli_plane_free();

void
preset_add_plane()
{
    preset_add("plan",
               &oli_plane_init,
               NULL,
               &oli_plane_render,
               &oli_plane_free,
               1);
    preset_add("plan 2",
               &oli_plane_init_2,
               NULL,
               &oli_plane_render,
               &oli_plane_free,
               1);
}

static camera_t* camera_p = NULL;
static solid_t* solid_p = NULL;

static void
oli_plane_init()
{
    camera_p = camera_init(-5, -2, 2,
                           1, 1, 0,
                           CAMERA_PROJECTION_PERSPECTIVE,
                           30);
    solid_p = solid_plane_init(200, 200, 2, 2);
}

static void
oli_plane_init_2()
{
    oli_plane_init();

    float previousvalue = 0;
    for(uint32_t vertex_index = 0; vertex_index < solid_vertex_count(solid_p); ++vertex_index)
    {
        vector_t* vertex_p = solid_vertex(solid_p, vertex_index);
        vector_axis_t random_delta =  0.5 * rand() / (double) RAND_MAX;
        float new_value = 0.05 * random_delta + 0.95 * previousvalue;
        vector_t new_v = vector_add(*vertex_p, vector_scale(VECTOR_Z, new_value));
        *vertex_p = new_v;
        previousvalue = new_value;
    }
}

static void
oli_plane_render(image_t* image_p, uint32_t frame)
{
    (void) frame;
    solid_render(solid_p , image_p, camera_p);
}

static void
oli_plane_free()
{
    solid_free(solid_p);
    camera_free(camera_p);
}
