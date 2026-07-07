/*
 * preset_brown.c
 *
 *  Created on: 7 juil. 2026
 *      Author: moliver
 */
#include <stdlib.h>

#include "preset.h"
#include "image_drawing.h"

static void oli_brown_init();
static void oli_brown_render(image_t* image_p, uint32_t frame);

void
preset_add_brown()
{
    preset_add("brownien", &oli_brown_init, NULL, &oli_brown_render, NULL, 1);
}

static uint32_t point_count = 0;

static void
oli_brown_init()
{
    point_count = 1000000;
}

static void
oli_brown_render(image_t* image_p, uint32_t frame)
{
    (void) frame;
    uint32_t width  = image_width(image_p);
    uint32_t height = image_height(image_p);
    brownien1(image_p, point_count, 1, width/2, height/2);
}



