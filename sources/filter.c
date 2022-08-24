/*
 * filter.c
 *
 *  Created on: Jan 15, 2020
 *      Author: moliver
 */

#include "main.h"

void flou(image_t image, int width, int height)
{
    image_t image2 = initImage(width,height);

    int x;
    int y;
    for(x=1; x<width-1; ++x )
    {
        for(y=1; y<height-1; ++y)
        {
            for(int i=-1;i<2;++i)
            {
                for(int j=-1;j<2;++j)
                {
                    image2[x][y][0] |= image[x+i][y+j][0];
                    image2[x][y][1] |= image[x+i][y+j][1];
                    image2[x][y][2] |= image[x+i][y+j][2];
                }

            }
        }
    }

    for(x=0;x<width;++x)
    {
        for(y=0;y<height;++y)
        {
            image[x][y][0] |= image2[x][y][0];
            image[x][y][1] |= image2[x][y][1];
            image[x][y][2] |= image2[x][y][2];

        }
    }
//  freeImage(image2,width,height);
}


