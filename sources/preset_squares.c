#include <stdlib.h>

#include "preset.h"
#include "figure.h"
#include "image_drawing.h"

static void oli_test_pattern_squares();
static void oli_test_pattern_squares_update();
static void oli_test_pattern_squares_render(image_t* image_p, uint32_t frame);
static void oli_test_pattern_squares_free();

void
preset_add_squares()
{
    preset_add("test pattern squares",
               &oli_test_pattern_squares,
               &oli_test_pattern_squares_update,
               &oli_test_pattern_squares_render,
               &oli_test_pattern_squares_free,
               32);
}


static void
oli_test_pattern_squares()
{
}

static figure_t* figure_p = NULL;
static uint32_t height;
static uint32_t width;
static image_t* current_image_p = NULL;

static void
oli_test_pattern_squares_update()
{
    if(figure_p != NULL)
    {
        for(uint32_t point = 0; point<figure_length(figure_p); ++point)
        {
           vector_random_delta(point_vector(figure_point(figure_p, point)),
                             8,
                             width,
                             height);
        }
    }

}

static void
oli_test_pattern_squares_render(image_t* image_p, uint32_t frame)
{
    (void) frame;

    if(figure_p == NULL)
    {
        test_pattern_squares(image_p, 1);
        width  = image_width(image_p);
        height = image_height(image_p);
        current_image_p = image_copy(image_p);
    }
    else
    {
        image_draw(image_p, current_image_p);
        figure_draw(figure_p, image_p);

        image_scale(image_p, 1.0f/8, SCALE_ALGORITHM_DUMB);
        image_scale(image_p, 8, SCALE_ALGORITHM_DUMB);
        image_draw(current_image_p, image_p);
    }
    if(figure_p != NULL)
    {
        figure_free(figure_p);
    }
    figure_p = figure_from_image(image_p);

}

static void
oli_test_pattern_squares_free()
{
    figure_free(figure_p);
    image_free(current_image_p);
}


