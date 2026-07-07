/*
 * preset_figure.c
 *
 *  Created on: 8 juil. 2026
 *      Author: moliver
 */
#include <stdlib.h>

#include "preset.h"
#include "figure.h"

static void oli_figure_update();
static void oli_figure_render(image_t* image_p, uint32_t frame);
static void oli_figure_free();

void
preset_add_figure()
{
    preset_add("figure",
               NULL,
               &oli_figure_update,
               &oli_figure_render,
               &oli_figure_free,
               10);
}

static const uint32_t point_count = 100000;
static figure_t* figure_p = NULL;
static uint32_t width = 0;
static uint32_t height = 0;

static void
oli_figure_update()
{
    if(figure_p == NULL)
    {
        if(width > 0 && height > 0)
        {
            figure_p = figure_init(point_count);
            for(uint32_t point = 0; point < figure_length(figure_p); ++point)
            {
                *point_vector(figure_point(figure_p, point))
                        = vector_init(width/2,
                                      height/2, 0);
                *point_colour(figure_point(figure_p, point))
                        = colour_get_random();
            }
        }
    }
    else
    {
        for(uint32_t point = 0; point < figure_length(figure_p); ++point)
        {
           vector_random_delta(point_vector(figure_point(figure_p, point)),
                               point, width, height);
        }
    }


}

static void
oli_figure_render(image_t* image_p, uint32_t frame)
{
    (void) frame;
    if(figure_p != NULL)
    {
        figure_draw(figure_p, image_p);
    }
    else
    {
        width = image_width(image_p),
        height = image_height(image_p);
    }
}

static void
oli_figure_free()
{
    if(figure_p != NULL)
    {
        figure_free(figure_p);
    }
}

