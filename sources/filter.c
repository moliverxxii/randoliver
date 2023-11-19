/*
 * filter.c
 *
 *  Created on: Jan 15, 2020
 *      Author: moliver
 */

#include "main.h"

void
flou(image_t* image)
{
    image_t* image2 = init_image(image->width, image->height);

    int x;
    int y;
    int tmp;
    colour_struct_t* point;
    for(x = 1; x < image->width - 1; ++x)
    {
        for(y = 1; y < image->height - 1; ++y)
        {
            for(int i = -1; i < 2; ++i)
            {
                for(int j = -1; j < 2; ++j)
                {
                    for(int col = 0; col < 3; col++)
                    {
                        tmp = image2->image[x][y][col];

                        image2->image[x][y][col] =
                                saturator(
                                        tmp
                                                + ((int) image->image[x + i][y
                                                        + j][col]) / 5,
                                        SAT_MIN,
                                        SAT_MAX);
                    }
                }
            }
        }
    }

    for(x = 0; x < image->width; ++x)
    {
        for(y = 0; y < image->height; ++y)
        {
            point = (colour_struct_t*) image->image[x][y];
            *point = *((colour_struct_t*) (image2->image[x][y]));

        }
    }
//  freeImage(image2,width,height);
}

