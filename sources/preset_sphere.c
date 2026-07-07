/*
 * preset_sphere.c
 *
 *  Created on: 8 juil. 2026
 *      Author: moliver
 */
#include <stdlib.h>
#include <math.h>

#include "figure.h"
#include "preset.h"
#include "list.h"
#include "edge.h"
#include "image_file.h"

static void oli_sphere_init();
static void oli_sphere_render(image_t* image_p, uint32_t frame);
static void oli_sphere_free();
static void oli_sphere_2_init();
static void oli_sphere_2_render(image_t* image_p, uint32_t frame);
static void oli_sphere_2_free();

void
preset_add_sphere()
{
    preset_add("sphere 1",
               &oli_sphere_init,
               NULL,
               &oli_sphere_render,
               &oli_sphere_free,
               1);
    preset_add("sphere 2",
               &oli_sphere_2_init,
               NULL,
               &oli_sphere_2_render,
               &oli_sphere_2_free,
               1);
}

static const uint32_t point_count = 10000;
figure_t* sphere_points_p = NULL;
camera_t* camera_p        = NULL;

static void
oli_sphere_init()
{
    sphere_points_p = figure_init(point_count);
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

    camera_p = camera_init(10, -40, 30,
                           0, 0, 0,
                           CAMERA_PROJECTION_PERSPECTIVE,
                           4.5);
}

static void
oli_sphere_render(image_t* image_p, uint32_t frame)
{
    (void) frame;
    figure_render(sphere_points_p, image_p, camera_p);
}

static void
oli_sphere_free()
{
    camera_free(camera_p);
    figure_free(sphere_points_p);
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


static const uint32_t class_count = 200;
static vector_t* vector_array_p = NULL;

static void
oli_sphere_2_init()
{
    sphere_points_p = figure_init(point_count);
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
    vector_array_p = malloc(point_count * sizeof(vector_t));
    for(uint32_t point=0; point < point_count; ++point)
    {
        vector_array_p[point] = *point_vector(figure_point(sphere_points_p, point));
    }
    figure_free(sphere_points_p);

    list_sort_array(vector_array_p, sizeof(vector_t), point_count,
                    &vector_z_access, SORT_ORDER_ASCENDING);


    for(uint32_t point = 0; point < point_count; point += class_count)
    {
        uint32_t element_count = ((point_count - point) > class_count)
                               ? class_count
                               : (point_count - point);
        list_sort_array(&vector_array_p[point], sizeof(vector_t), element_count,
                        &vector_longitude_access, SORT_ORDER_ASCENDING);
    }


    camera_p = camera_init(1, 5, 5,
                           0, 0, 0,
                           CAMERA_PROJECTION_PERSPECTIVE,
                           4.5);
}


static void
oli_sphere_2_render(image_t* image_p, uint32_t frame)
{
    (void) frame;
    uint32_t edge_count = point_count - 1;
    for(uint32_t edge = 0; edge < edge_count; edge++)
    {
        edge_t* edge_p = edge_init(vector_array_p + edge,
                                   vector_array_p + edge + 1,
                                   colour_init(COLOUR_VALUE_MAX * (1 - vector_array_p[edge].x) / 2,
                                               COLOUR_VALUE_MAX * (1 + vector_array_p[edge].y) / 2,
                                               COLOUR_VALUE_MAX * (1 + vector_array_p[edge].z) / 2));
        edge_render(edge_p, image_p, camera_p);
        edge_free(edge_p);
    }
}

static void
oli_sphere_2_free()
{
    camera_free(camera_p);
    free(vector_array_p);
}
