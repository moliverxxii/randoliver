/*
 * presets.c
 *
 *  Created on: 30 mai 2026
 *      Author: moliver
 */

#include <math.h>
#include <stdlib.h>

#include "presets.h"

#include "figure.h"
#include "image_file.h"
#include "image_drawing.h"
#include "performance.h"
#include "solid.h"
#include "solid_file.h"

static const char* const PRESET_NAME_NONE = "Not a preset";

typedef struct preset_t
{
    const char* const name_p;
    void (*action)();
} preset_t;

static void oli_brown();
static void oli_test_pattern();
static void oli_test_pattern_scan();
static void oli_test_pattern_squares();
static void oli_solid();
static void oli_figure();
static void oli_sphere();

static const preset_t PRESET_LIST[] =
{
    {"brownien 1", &oli_brown},
    {"test pattern", &oli_test_pattern},
    {"test pattern scan", &oli_test_pattern_scan},
    {"test pattern squares", &oli_test_pattern_squares},
    {"solid file", &oli_solid},
    {"figure", &oli_figure},
    {"sphere", &oli_sphere}
};


uint32_t
preset_get_total_count()
{
    return sizeof(PRESET_LIST)/ sizeof(preset_t);
}

const char*
preset_get_name(uint32_t preset_index)
{
    if(preset_index > preset_get_total_count())
    {
        return PRESET_NAME_NONE;
    }
    else
    {
        return PRESET_LIST[preset_index].name_p;
    }

}

void
preset_run(uint32_t preset_index)
{
    if(preset_index < preset_get_total_count())
    {
        if(PRESET_LIST[preset_index].action != NULL)
        {
            (*PRESET_LIST[preset_index].action)();
        }
    }
}


static void
oli_brown()
{
    int width = 320;
    int height = 240;
    image_t* image_p  = image_init(width, height);
    image_set(image_p);
    uint32_t point_count = 1000000;

    brownien1(image_p, point_count, 1, width/2, height/2);
    image_file_write("oli brownien", image_p, NULL);
    image_free(image_p);
}


static void
oli_test_pattern()
{
    int width = 1000;
    int height = 1000;
    image_t* image_p  = image_init(2*width, 2*height);
    image_scale(image_p, 1.0f/height, SCALE_ALGORITHM_LINEAR);
    test_pattern_squares(image_p, 1);
    image_scale(image_p, 960, SCALE_ALGORITHM_LINEAR);
    image_scale(image_p, 10.0f/960, SCALE_ALGORITHM_LINEAR);
    image_scale(image_p, 96, SCALE_ALGORITHM_LINEAR);
    image_file_write("oli test pattern", image_p, NULL);
    image_free(image_p);
}

static void
oli_test_pattern_scan()
{
    int width = 320;
    int height = 240;
    image_t* image_p  = image_init(width, height);

    test_pattern_scan(image_p);
    image_reduce_bit_depth(image_p, 5, 1);
    image_file_write("oli test pattern scan", image_p, NULL);

    test_pattern_scan(image_p);
    image_reduce_bit_depth(image_p, 5, 0);
    image_file_write("oli test pattern scan2", image_p, NULL);
    image_free(image_p);
}

static void
oli_test_pattern_squares()
{
    int width = 320;
    int height = 240;
    image_t* image_p  = image_init(width, height);
    test_pattern_squares(image_p, 1);

    int frame_count = 32;

    for(int frame = 0; frame < frame_count; ++frame)
    {
        figure_t* figure_p = figure_from_image(image_p);
        for(uint32_t point = 0; point<figure_length(figure_p); ++point)
        {
           vector_random_delta(point_vector(figure_point(figure_p, point)),
                             8,
                             image_width(image_p),
                             image_height(image_p));
        }

        figure_draw(figure_p, image_p);
        figure_free(figure_p);
        image_scale(image_p, 1.0f/8, SCALE_ALGORITHM_DUMB);
        image_scale(image_p, 8, SCALE_ALGORITHM_DUMB);

        char* file_name_p = file_name_extension_number("squares", frame);
        image_file_write(file_name_p, image_p, NULL);
        free(file_name_p);
    }
    image_free(image_p);
}

static void
oli_solid()
{
    int width = 320;
    int height = 240;
    float scale = 1.0f;
    image_t* image_p  = image_init(width, height);
    image_set(image_p);

    solid_init_const();
    solid_t* solid_p = solid_file_open("scene.txt");


    camera_t camera = camera_init(-5, -1.5, -1,
                                  0, 0, 0,
                                  45);

    renderable_cache_clear();

    solid_render(solid_p, image_p, &camera);
    image_reduce_bit_depth(image_p, 5, 1);

    image_file_write("oli solid", image_p, NULL);

    image_scale(image_p, scale, SCALE_ALGORITHM_LINEAR);
    image_set(image_p);

    image_free(image_p);
    solid_free(solid_p);
}

static void
oli_figure()
{
    int width = 320;
    int height = 240;
    image_t* image_p  = image_init(width, height);
    int frame_count = 1;

    uint32_t point_count = 1000000;
    figure_t* figure_p = figure_init(point_count);
    for(uint32_t point = 0; point < figure_length(figure_p); ++point)
    {
        *point_vector(figure_point(figure_p, point))
                = vector_init(image_width(image_p)/2,
                              image_height(image_p)/2, 0);
        *point_colour(figure_point(figure_p, point))
                = colour_get_random();
    }


    for(int frame = 0; frame < frame_count; ++frame)
    {
        for(uint32_t point = 0; point < figure_length(figure_p); ++point)
        {
           vector_random_delta(point_vector(figure_point(figure_p, point)),
                               point,
                               image_width(image_p),
                               image_height(image_p));
        }
        figure_draw(figure_p, image_p);
        figure_free(figure_p);

        char* file_name_p = file_name_extension_number("oli figure", frame);
        image_file_write(file_name_p, image_p, NULL);
        free(file_name_p);

        image_set(image_p);
    }
    image_free(image_p);
}



static void
oli_sphere()
{
    int width = 320;
    int height = 240;
    image_t* image_p  = image_init(width, height);

    uint32_t point_count = 1000;
    figure_t* sphere_points_p = figure_init(point_count);
    const vector_t START = VECTOR_Z;
    for(uint32_t point = 0; point<point_count; ++point)
    {
        point_t* point_p = figure_point(sphere_points_p, point);
        vector_t* vector_p = point_vector(point_p);
        *vector_p = START;

        float rand_vertical   = (double) rand()/RAND_MAX;
        float rand_horizontal = (double) rand()/RAND_MAX;

        *vector_p = vector_rotate(*vector_p, VECTOR_X,     M_PI * rand_vertical);
        *vector_p = vector_rotate(*vector_p, VECTOR_Z, 2 * M_PI * rand_horizontal);
        *point_colour(point_p) = colour_init(COLOUR_MAX * rand_vertical,
                                             COLOUR_MAX * rand_horizontal,
                                             COLOUR_MAX * rand_vertical);
    }

    camera_t camera = camera_init(10, -40, 30, 0, 0, 0, 4.5);

    image_set(image_p);
    figure_render(sphere_points_p, image_p, &camera);
    figure_free(sphere_points_p);
    image_reduce_bit_depth(image_p, 5, 1);
    image_file_write("sphere", image_p, NULL);
}
