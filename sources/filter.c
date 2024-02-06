/*
 * filter.c
 *
 *  Created on: Jan 15, 2020
 *      Author: moliver
 */
#include <string.h>

#include "filter.h"
#include "utility.h"
void
flou(image_t* image)
{
    image_t* image2 = init_image(image->width, image->height);
    set_image(image2);

    int x;
    int y;
    colour_struct_t tmp;
    for(x = 1; x < image->width - 1; ++x)
    {
        for(y = 1; y < image->height - 1; ++y)
        {
            for(int i = -1; i < 2; ++i)
            {
                for(int j = -1; j < 2; ++j)
                {
                	tmp = *(colour_struct_t*) image2->image[y][x];
                    for(int colour = 0; colour < 3; colour++)
                    {

                        image2->image[y][x][colour] =
                                saturator(  ( (uint8_t*) &tmp)[colour] + (int) image->image[y + j][x + i][colour] / 5,
										  SAT_MIN,
                                          SAT_MAX);
                    }
                }
            }
        }
    }

    memcpy(*image->image, *image2->image, image->width*image->height*sizeof(colour_t));

    free_image(image2);
}

