/*
 * preset_corners.c
 *
 *  Created on: 7 juil. 2026
 *      Author: moliver
 */

#include <stdlib.h>

#include "preset.h"

static void oli_test_2d_corners_render(image_t* image_p, uint32_t frame);

void
preset_add_corners()
{
    preset_add("test 2D coins",        NULL                       , NULL, &oli_test_2d_corners_render,  NULL,                      1);
}

static void
oli_test_2d_corners_render(image_t* image_p, uint32_t frame)
{
    (void) frame;
    uint32_t width  = image_width(image_p);
    uint32_t height = image_height(image_p);
    image_pixel_set(image_p,         0,         0,   COLOUR_BLUE);
    image_pixel_set(image_p, width - 1,         0,    COLOUR_RED);
    image_pixel_set(image_p, width - 1, height -1, COLOUR_YELLOW);
    image_pixel_set(image_p,         0, height -1,  COLOUR_GREEN);
}


