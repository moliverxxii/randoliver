/*
 * filter.c
 *
 *  Created on: Jan 15, 2020
 *      Author: moliver
 */
#include <string.h>

#include "filter.h"
#include "utility.h"

static colour_struct_t get_blurred_pixel(const image_t* image_p, int pixel_x, int pixel_y, int radius);

void
flou(image_t* image_p, int radius)
{
    if(radius <= 0)
    {
        return;
    }
    image_t* image2 = init_image(image_p->width, image_p->height);
    set_image(image2);

    int x;
    int y;
    for(x = 0; x < image_p->width; ++x)
    {
        for(y = 0; y < image_p->height; ++y)
        {
            *(colour_struct_t*) image2->image[y][x] = get_blurred_pixel(image_p, x, y, radius);
        }
    }

    memcpy(*image_p->image, *image2->image, image_p->width*image_p->height*sizeof(colour_t));

    free_image(image2);
}

void
symetry(image_t* image_p)
{
    int x, y;
    for(x = 0; x < image_p->width/2; ++x)
    {
        for(y = 0; y < image_p->height/2; ++y)
        {
            *(colour_struct_t*) image_p->image[y][image_p->width - 1 - x]
                = *(colour_struct_t*) image_p->image[y][x];
            *(colour_struct_t*) image_p->image[image_p->height - 1 - y][x]
                = *(colour_struct_t*) image_p->image[y][x];
            *(colour_struct_t*) image_p->image[image_p->height - 1 - y][image_p->width - 1 - x]
                = *(colour_struct_t*) image_p->image[y][x];
        }
    }
}

void random_colour_shift(image_t* image_p)
{
    process_1_image(&random_delta_colour, image_p);
}


static colour_struct_t get_blurred_pixel(const image_t* image_p, int pixel_x, int pixel_y, int radius)
{
    if(radius <= 0)
    {
        return *(colour_struct_t*) image_p->image[pixel_y][pixel_x];
    }
    colour_t return_colour = {0, 0, 0};
    for (int colour = 0; colour < COLOUR_COUNT; colour++)
    {
        int sum = 0;
        int count = 0;
        for (int i = -radius; i <= radius; ++i)
        {
            for (int j = -radius; j <= radius; ++j)
            {
                if(!is_in_image(pixel_x + i, pixel_y + j, image_p))
                {
                    continue;
                }
                sum += image_p->image[pixel_y + j][pixel_x + i][colour];
                ++ count;
            }
        }
        return_colour[colour] = (sum + count/2) / count; //Supprimer "+ count/2" pour supprimer l'arrondi entier.
    }
    return *(colour_struct_t*) return_colour;
}
