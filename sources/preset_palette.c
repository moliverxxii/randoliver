/*
 * preset_palette.c
 *
 *  Created on: 8 juil. 2026
 *      Author: moliver
 */
#include "preset.h"
#include "image_drawing.h"
#include "palette.h"
#include "image_file.h"

static void oli_test_palette();

void preset_add_palette()
{
    preset_add("test palette", &oli_test_palette, NULL, NULL, NULL, 1);

}

//TODO passer la palette en argument d'une maniere ou d'une autre.
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

