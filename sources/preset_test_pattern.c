/*
 * preset_test_pattern.c
 *
 *  Created on: 7 juil. 2026
 *      Author: moliver
 */
#include <stdlib.h>

#include "preset.h"
#include "image.h"
#include "image_drawing.h"

static void oli_test_pattern_render(image_t* image_p, uint32_t frame);
static void oli_test_pattern_scan_render(image_t* image_p, uint32_t frame);
static void oli_test_pattern_scan_2_render(image_t* image_p, uint32_t frame);

void
preset_add_test_pattern()
{
    preset_add("test pattern",
               NULL,
               NULL,
               &oli_test_pattern_render,
               NULL,
               1);
    preset_add("test pattern scan",
               NULL,
               NULL,
               &oli_test_pattern_scan_render,
               NULL,
               1);
    preset_add("test pattern scan dither",
               NULL,
               NULL,
               &oli_test_pattern_scan_2_render,
               NULL,
               1);
}

static void
oli_test_pattern_render(image_t* image_p, uint32_t frame)
{
    (void) frame;
    int width  = image_width(image_p);
    int height = image_height(image_p);
    int denominator = width < height ? width : height;
    image_scale(image_p, 2.0f/denominator, SCALE_ALGORITHM_LINEAR);
    test_pattern_squares(image_p, 1);
    image_scale(image_p, 960, SCALE_ALGORITHM_LINEAR);
    image_scale(image_p, 10.0f/960, SCALE_ALGORITHM_LINEAR);
    image_scale(image_p, 96, SCALE_ALGORITHM_LINEAR);
}

static void
oli_test_pattern_scan_render(image_t* image_p, uint32_t frame)
{
    (void) frame;
    test_pattern_scan(image_p);
    image_reduce_bit_depth(image_p, 5, 0);
}

static void
oli_test_pattern_scan_2_render(image_t* image_p, uint32_t frame)
{
    (void) frame;
    test_pattern_scan(image_p);
    image_reduce_bit_depth(image_p, 5, 1);
}

