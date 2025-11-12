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
    image_t* image2 = image_init(image_p->width, image_p->height);
    image_set(image2);

    uint32_t x;
    uint32_t y;
    for(x = 0; x < image_p->width; ++x)
    {
        for(y = 0; y < image_p->height; ++y)
        {
            image2->image[y][x] = get_blurred_pixel(image_p, x, y, radius);
        }
    }

    memcpy(*image_p->image, *image2->image, image_p->width*image_p->height*sizeof(colour_t));

    image_free(image2);
}

void
symetry(image_t* image_p)
{
    uint32_t x, y;
    for(x = 0; x < image_p->width/2; ++x)
    {
        for(y = 0; y < image_p->height/2; ++y)
        {
            image_p->image[y][image_p->width - (1 + x)]
                = image_p->image[y][x];
            image_p->image[image_p->height - (1 + y)][x]
                = image_p->image[y][x];
            image_p->image[image_p->height - (1 + y)][image_p->width - (1 + x)]
                = image_p->image[y][x];
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
        return image_p->image[pixel_y][pixel_x];
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
                point_t point = point_init(pixel_x + i, pixel_y + j, 0, BLACK);
                if(!point_is_in_image(&point, image_p))
                {
                    continue;
                }
                sum += image_p->image[pixel_y + j][pixel_x + i].bytes[colour];
                ++count;
            }
        }
        sum /= count;
        return_colour.bytes[colour] = dither(sum);
    }
    return return_colour;
}
