/*
 * preset_bitarray.c
 *
 *  Created on: 13 juil. 2026
 *      Author: moliver
 */
#include "bitarray.h"
#include "preset.h"

static void oli_bitarray_init();
static void oli_bitarray(image_t* image_p, uint32_t frame);
static void oli_bitarray_update();
void
preset_add_bitarray()
{
    preset_add("bitarray", &oli_bitarray_init, &oli_bitarray_update, &oli_bitarray, NULL, 2000);
}

static const uint32_t stable_start = 8;
static const uint32_t reprod_start = 13;
static const uint32_t overpo_start = 20;
static const uint32_t neighbourhood = 7;
static bitarray_t* kernel_p        = NULL;
static bitarray_t* neighbourhood_p = NULL;


static void
oli_bitarray_init()
{
    kernel_p = bitarray_init(neighbourhood, neighbourhood);
    neighbourhood_p = bitarray_init(neighbourhood, neighbourhood);


    for(uint32_t n_x = 0; n_x < neighbourhood; ++n_x)
    {
        for(uint32_t n_y = 0; n_y < neighbourhood; ++n_y)
        {
         //   bitarray_bit_set(kernel_p, n_x, n_y);
        }
    }
    bitarray_bit_reset(kernel_p, neighbourhood/2, neighbourhood/2);
}


static bitarray_t* array_p = NULL;
static bitarray_t* dest_array_p = NULL;
static uint32_t width  = 0;
static uint32_t height = 0;

static void
oli_bitarray_update()
{
    if(array_p == NULL)
    {
        return;
    }

    for(uint32_t x = 0; x < width; ++x)
    {
        for(uint32_t y = 0; y < height; ++y)
        {
            bitarray_extract(neighbourhood_p,
                             array_p,
                             (int32_t)x-(int32_t) neighbourhood/2,
                             (int32_t)y-(int32_t) neighbourhood/2);
            bitarray_and(neighbourhood_p, kernel_p);
            uint32_t count = bitarray_on_get(neighbourhood_p);
            int value = 0;

            if(count < stable_start || overpo_start <= count)
            {
                value = 0;
            }
            else if(reprod_start <= count)
            {
                value = 1;
            }
            else
            {
                value = bitarray_bit_get(array_p, x, y);
            }

            if(value)
            {
                bitarray_bit_set(dest_array_p, x, y);
            }
            else
            {
                bitarray_bit_reset(dest_array_p, x, y);
            }
        }
    }
    bitarray_extract(array_p, dest_array_p, 0, 0);
}

static void
oli_bitarray(image_t* image_p, uint32_t frame)
{
    (void) frame;
    if(array_p == NULL)
    {
        width        = image_width(image_p);
        height       = image_height(image_p);
        array_p      = bitarray_init(width, height);
        dest_array_p = bitarray_init(width, height);
        for(uint32_t x = 0; x < width/8; ++x)
        {
            for(uint32_t y = 0; y < height; ++y)
            {
                bitarray_bit_set(array_p, x, y);
            }
        }
        for(uint32_t x = 7*width/8; x < width; ++x)
        {
            for(uint32_t y = 0; y < height; ++y)
            {
                bitarray_bit_reset(array_p, x, y);
            }
        }
    }

    for(uint32_t x = 0; x < image_width(image_p); ++x)
    {
        for(uint32_t y = 0; y < image_height(image_p); ++y)
        {
            colour_t colour = bitarray_bit_get(array_p, x, y) ? COLOUR_WHITE : COLOUR_BLACK;
            image_pixel_set(image_p, x, y, colour);
        }
    }
}

