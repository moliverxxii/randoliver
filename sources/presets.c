/*
 * presets.c
 *
 *  Created on: 30 mai 2026
 *      Author: moliver
 */

#include <math.h>
#include <palette.h>
#include <stdlib.h>
#include <string.h>

#include "presets.h"

#include "edge.h"
#include "figure.h"
#include "image_file.h"
#include "image_drawing.h"
#include "list.h"
#include "operator.h"
#include "performance.h"
#include "solid.h"
#include "solid_file.h"

static const char* const PRESET_NAME_NONE = "Not a preset";

typedef struct preset_t
{
    const char* const name_p;
    void (*action)();
} preset_t;

static void oli_test_3d_middle_point();
static void oli_test_2d_corners();
static void oli_test_vectors();
static void oli_brown();
static void oli_test_pattern();
static void oli_test_pattern_scan();
static void oli_test_palette();
static void oli_test_pattern_squares();
static void oli_solid();
static void oli_figure();
static void oli_sphere();
static void oli_sphere_2();

static const preset_t PRESET_LIST[] =
{
    {"test 3D point milieu", &oli_test_3d_middle_point},
    {"test 2D coins", &oli_test_2d_corners},
    {"test vectors", &oli_test_vectors},
    {"brownien 1", &oli_brown},
    {"test pattern", &oli_test_pattern},
    {"test pattern scan", &oli_test_pattern_scan},
    {"test palette", &oli_test_palette},
    {"test pattern squares", &oli_test_pattern_squares},
    {"solid file", &oli_solid},
    {"figure", &oli_figure},
    {"sphere 1", &oli_sphere},
    {"sphere 2", &oli_sphere_2}
};


uint32_t
preset_get_total_count()
{
    return sizeof(PRESET_LIST) / sizeof(preset_t);
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
oli_test_3d_middle_point()
{
    int width = 4;
    int height = 4;
    image_t* image_p  = image_init(width, height);
    image_set(image_p);

    point_t* p_p = point_init(0, 0, 0, COLOUR_WHITE);
    camera_t camera = camera_init(-1,
                                  0,
                                  0,
                                  point_vector(p_p)->x,
                                  point_vector(p_p)->y,
                                  point_vector(p_p)->z,
                                  45.0f);
    point_render(p_p, image_p, &camera);
    point_free(p_p);
    image_file_write("test 3D point milieu", image_p, NULL);
    image_free(image_p);
}

static void
oli_test_2d_corners()
{
    int width = 4;
    int height = 4;
    image_t* image_p  = image_init(width, height);
    image_set(image_p);

    image_pixel_set(image_p,         0,         0,   COLOUR_BLUE);
    image_pixel_set(image_p, width - 1,         0,    COLOUR_RED);
    image_pixel_set(image_p, width - 1, height -1, COLOUR_YELLOW);
    image_pixel_set(image_p,         0, height -1,  COLOUR_GREEN);

    image_file_write("test 2d coins", image_p, NULL);
    image_free(image_p);
}

static void
oli_test_vectors()
{
    printf("operator init");
    operator_t* a_p = operator_init_null();
    operator_print(a_p);
    operator_free(a_p);

    a_p = operator_init_id();
    operator_print(a_p);
    operator_free(a_p);

    a_p = operator_init_translation(vector_init(0.5, 0.5, 0.5));
    operator_print(a_p);

    operator_t* b_p = operator_init_id();
    operator_print(b_p);

    printf("operator row\n");
    for(vector_axis_e row = 0; row < VECTOR_AXIS_COUNT; ++row)
    {
        vector_print(operator_row_get(b_p, row));
    }

    printf("operator column\n");
    for(vector_axis_e column = 0; column < VECTOR_AXIS_COUNT; ++column)
    {
        vector_print(operator_column_get(b_p, column));
    }

    printf("new_table\n");
    vector_t table[3] =
    {
        vector_init(1,2,3),
        vector_init(4,5,6),
        vector_init(7,8,9)
    };
    for(int vector = 0; vector < 3; ++vector)
    {
        vector_print(table[vector]);
    }

    printf("vector scalar\n"
           "3D %f\n"
           "4D %f\n",
           vector_scalar(table[0], VECTOR_X),
           vector_scalar_full(table[0], VECTOR_X));

    printf("operator operation\n");
    operator_operation(b_p, table, 3);

    for(int vector = 0; vector < 3; ++vector)
    {
        vector_print(table[vector]);
    }
    printf("operator translation\n");
    vector_t* result_p = operator_operation(a_p, table, 3);

    for(int vector = 0; vector < 3; ++vector)
    {
        vector_print(result_p[vector]);
    }
    free(result_p);


    printf("operator multiply translation x id\n");
    operator_t* c_p = operator_multiply(a_p, b_p);
    operator_print(c_p);

    operator_free(a_p);
    operator_free(b_p);
    operator_free(c_p);
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
oli_test_palette()
{
    int width = 320;
    int height = 240;
    image_t* image_p  = image_init(width, height);
    test_pattern_scan(image_p);

//    palette_t* palette_p = palette_init_extreme();
    palette_t* palette_p = palette_init(7, 0);

#ifndef NDEBUG
    palette_index_method_e method = PALETTE_INDEX_METHOD_DITHER_DISTANCE;
#else
    palette_index_method_e method = PALETTE_INDEX_METHOD_DISTANCE;
#endif //!NDEBUG


    void* parameters_p = colour_operation_reduce_parameters_init(palette_p, method);
    image_file_parameters_t* file_parameters_p = image_file_parameters_init_palette(palette_p, PIXEL_BIT_DEPTH_8b, method);
    image_file_write("oli test pattern palette", image_p, file_parameters_p);

    //on teste si l'operation est injective
    image_process_1(&colour_operation_reduce, image_p, parameters_p);
    image_file_write("oli test pattern palette double", image_p, NULL);
    image_free(image_p);
    colour_operation_reduce_parameters_free(parameters_p);
    image_file_parameters_free_palette(file_parameters_p);
    image_p = image_init(16, 16);
    image_set(image_p);
    for(palette_index_t colour = 0; colour < palette_count(palette_p); ++colour)
    {
        image_pixel_set(image_p,
                        colour%image_width(image_p),
                        colour/image_width(image_p),
                        *palette_colour_get(palette_p, colour));
    }
    palette_free(palette_p);
    image_file_write("palette", image_p, NULL);
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
    image_set(image_p);
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
        *point_colour(point_p) = colour_init(COLOUR_VALUE_MAX * rand_vertical,
                                             COLOUR_VALUE_MAX * rand_horizontal,
                                             COLOUR_VALUE_MAX * rand_vertical);
    }

    camera_t camera = camera_init(10, -40, 30, 0, 0, 0, 4.5);

    image_set(image_p);
    figure_render(sphere_points_p, image_p, &camera);
    figure_free(sphere_points_p);
    image_reduce_bit_depth(image_p, 5, 1);
    image_file_write("sphere", image_p, NULL);
    image_free(image_p);
}

static float vector_z_access(const void* list_element_p)
{
    const vector_t* vector_p = list_element_p;
    return (float) vector_p->z;
}

static float vector_longitude_access(const void* list_element_p)
{
    const vector_t* vector_p = list_element_p;
    return planetary_init_vector(*vector_p).longitude;
}

static void
oli_sphere_2()
{
    int width = 320;
    int height = 240;
    image_t* image_p  = image_init(width, height);

    uint32_t point_count = 10000;
    uint32_t class_count = 200;

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
        *point_colour(point_p) = colour_init(COLOUR_VALUE_MAX * rand_vertical,
                                             COLOUR_VALUE_MAX * rand_horizontal,
                                             COLOUR_VALUE_MAX * rand_vertical);
    }
    vector_t* vector_array_p = malloc(point_count * sizeof(vector_t));
    for(uint32_t point=0; point < point_count; ++point)
    {
        vector_array_p[point] = *point_vector(figure_point(sphere_points_p, point));
    }
    list_t* list_head_vector_p = NULL;
    list_sort_array(&list_head_vector_p,
                    vector_array_p, sizeof(vector_t), point_count,
                    &vector_z_access, SORT_ORDER_ASCENDING);

    figure_free(sphere_points_p);

    free(vector_array_p);
    vector_array_p = list_array(list_head_vector_p);
    list_free(list_head_vector_p);

    for(uint32_t point = 0; point < point_count; point += class_count)
    {
        uint32_t element_count = ((point_count - point) > class_count)
                               ? class_count
                               : (point_count - point);
        list_sort_array(&list_head_vector_p,
                        &vector_array_p[point], sizeof(vector_t), element_count,
                        &vector_longitude_access, SORT_ORDER_ASCENDING);
        vector_t* vector_slice_p = list_array(list_head_vector_p);
        list_free(list_head_vector_p);

        memcpy(vector_array_p + point, vector_slice_p, element_count * sizeof(vector_t));
        free(vector_slice_p);
    }

    uint32_t edge_count = point_count - 1;

    camera_t camera = camera_init(1, 1, 30, 0, 0, 0, 4.5);

    image_set(image_p);
    for(uint32_t edge = 0; edge < edge_count; edge++)
    {
        edge_t* edge_p = edge_init(vector_array_p + edge,
                                   vector_array_p + edge + 1,
                                   colour_init(COLOUR_VALUE_MAX * (1 - vector_array_p[edge].x) / 2,
                                               COLOUR_VALUE_MAX * (1 + vector_array_p[edge].y) / 2,
                                               COLOUR_VALUE_MAX * (1 + vector_array_p[edge].z) / 2));
        edge_render(edge_p, image_p, &camera);
        edge_free(edge_p);
    }

    free(vector_array_p);

    palette_t* palette_p = palette_init(PIXEL_BIT_DEPTH_8b, 0);
    image_file_parameters_t* file_parameters_p = image_file_parameters_init_palette(palette_p, PIXEL_BIT_DEPTH_8b, PALETTE_INDEX_METHOD_DISTANCE);

    image_file_write("sphere 2", image_p, file_parameters_p);
    image_file_parameters_free_palette(file_parameters_p);
}
