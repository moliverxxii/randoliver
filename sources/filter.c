/*
 * filter.c
 *
 *  Created on: Jan 15, 2020
 *      Author: moliver
 */
#include <string.h>

#include "point.h"
#include "filter.h"
#include "utility.h"

static colour_t get_blurred_pixel(const image_t* image_p, int pixel_x, int pixel_y, int radius);

void
flou(image_t* image_p, int radius)
{
    if(radius <= 0)
    {
        return;
    }
    image_t* image2 = image_init(image_width(image_p), image_height(image_p));
    image_set(image2);

    uint32_t x;
    uint32_t y;
    for(x = 0; x < image_width(image_p); ++x)
    {
        for(y = 0; y < image_height(image_p); ++y)
        {
            image_pixel_set(image2, x, y, get_blurred_pixel(image_p, x, y, radius));
        }
    }

    memcpy(image_data(image_p), image_data(image2), image_width(image_p) * image_height(image_p) * sizeof(colour_t));

    image_free(image2);
}

void
symetry(image_t* image_p)
{
    uint32_t x, y;
    for(x = 0; x < image_width(image_p)/2; ++x)
    {
        for(y = 0; y < image_height(image_p)/2; ++y)
        {
            image_pixel_set(image_p, image_width(image_p) - (1 + x),                                   y, image_pixel_get(image_p, x, y));
            image_pixel_set(image_p,                                  x, image_height(image_p) - (1 + y), image_pixel_get(image_p, x, y));
            image_pixel_set(image_p, image_width(image_p) - (1 + x), image_height(image_p) - (1 + y), image_pixel_get(image_p, x, y));
        }
    }
}

void
random_colour_shift(image_t* image_p, int delta)
{
    image_process_1(&colour_random_delta, image_p, &delta);
}

static colour_t
get_blurred_pixel(const image_t* image_p, int pixel_x, int pixel_y, int radius)
{
    if(radius <= 0)
    {
        return image_pixel_get(image_p, pixel_x, pixel_y);
    }
    colour_t return_colour = {{0, 0, 0}};
    for (int colour = 0; colour < COLOUR_COUNT; colour++)
    {
        float sum = 0;
        int count = 0;
        for (int i = -radius; i <= radius; ++i)
        {
            for (int j = -radius; j <= radius; ++j)
            {
                point_t* point_p = point_init(pixel_x + i, pixel_y + j, 0, BLACK);
                int is_in_image = point_is_in_image(point_p, image_p);
                point_free(point_p);
                if(!is_in_image)
                {
                    continue;
                }
                sum += image_pixel_get(image_p, pixel_x + i, pixel_y + j).array[colour];
                ++count;
            }
        }
        sum /= count;
        return_colour.array[colour] = dither(sum);
    }
    return return_colour;
}
