/*
 * imgUtil.c
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "utility.h"
#include "image.h"

const image_t SYSTEM_SCREEN =
{
    1280,
    720,
    NULL
};


image_t*
image_init(uint32_t width, uint32_t height)
{

    colour_t* image_data_p = malloc(height * width * sizeof(colour_t));

    image_t* image = malloc(sizeof(image_t));
    image->width   = width;
    image->height  = height;
    image->image   = malloc(height * sizeof(row_t));

    uint32_t y;
	row_t row_p = image_data_p;
    for(y = 0; y < height; ++y)
    {
        image->image[y] = row_p;
        row_p += width;
    }
    return image;
}

void
image_free(image_t* image)
{
    free(*(image->image));
    free(image->image);
    free(image);
}



void
image_set(image_t* image)
{
    memset(*image->image, 0, image->width * image->height * sizeof(colour_t) );
}

void
image_random(image_t* image_p)
{
    uint32_t x,y;
    for(x=0; x<image_p->width; ++x)
    {
        for(y=0; y<image_p->height; ++y)
        {
            *(colour_struct_t*) image_p->image[y][x] = colour_get_random();
        }
    }
}

int
image_get_sum_colour(const image_t* image_p)
{
    int sum = 0;
    for(int colour = 0; colour<COLOUR_COUNT; colour++)
    {
        uint32_t x,y;
        for(x=0; x<image_p->width; ++x)
        {
            for(y=0; y<image_p->height; ++y)
            {
                sum += image_p->image[y][x][colour];
            }
        }

    }
    return sum;
}

void
image_process_1(colour_unary_operator operator, image_t* image_p, void* parameters_p)
{
    uint32_t x,y;
    for(x=0; x<image_p->width; ++x)
    {
        for(y=0; y<image_p->height; ++y)
        {
            *(colour_struct_t*) image_p->image[y][x] =  (*operator)( *(colour_struct_t*) image_p->image[y][x], parameters_p);
        }
    }
}

void
image_process_2(colour_binary_operator operator,
        image_t* image_1_p,
        const image_t* image_2_p,
        void* parameters_p)
{
    uint32_t x,y;
    for(x=0; x<image_1_p->width; ++x)
    {
        for(y=0; y<image_1_p->height; ++y)
        {
            *(colour_struct_t*) image_1_p->image[y][x] =
                    (*operator)( *(colour_struct_t*) image_1_p->image[y][x],
                                 *(colour_struct_t*) image_2_p->image[y][x],
                                 parameters_p);
        }
    }
}

void
image_process_3(colour_ternary_operator operator,
        image_t* image_1_p,
        const image_t* image_2_p,
        const image_t* image_3_p,
        void* parameters_p)
{
    uint32_t x,y;
    for(x=0; x<image_1_p->width; ++x)
    {
        for(y=0; y<image_1_p->height; ++y)
        {
            *(colour_struct_t*) image_1_p->image[y][x] =
                    (*operator)( *(colour_struct_t*) image_1_p->image[y][x],
                                 *(colour_struct_t*) image_2_p->image[y][x],
                                 *(colour_struct_t*) image_3_p->image[y][x],
                                 parameters_p);
        }
    }
}

void
image_add(image_t* image_1_p, const image_t* image_2_p)
{
    image_process_2(&colour_add_2, image_1_p, image_2_p, NULL);
}

void
image_print(const image_t* image)
{
    uint32_t x;
    uint32_t y;
    for(x = 0; x < image->width; ++x)
    {
        for(y = 0; y < image->height; ++y)
        {
            printf("Point[%5d,%5d] = (%x,%x,%x)\n", x + 1, y + 1,
                    image->image[y][x][0], image->image[y][x][1],
                    image->image[y][x][2]);
        }
    }
}

void
image_draw_rect(colour_t color, int botLeftX, int botLeftY, int topRightX,
                int topRightY, image_t* image)
{
    int x;
    int y;
    int k;
    for(x = botLeftX; x <= topRightX; ++x)
    {
        for(y = botLeftY; y <= topRightY; ++y)
        {
            for(k = 0; k < 3; ++k)
            {
                image->image[y][x][k] = color[k];
            }
        }
    }
}


point_renderer public_point_renderer = &or_point;

void
draw_point(const point_t point, image_t* image_p)
{
    int x;
    int y;
    x = point.vector.x;
    y = point.vector.y;
    *(colour_struct_t*) image_p->image[y][x] = *(colour_struct_t*) &point.colour;
}

void
or_point(const point_t point, image_t* image_p)
{
    int x;
    int y;
    x = point.vector.x;
    y = point.vector.y;
    uint32_t buffer = *(uint32_t*) image_p->image[y][x];
    buffer ^= *(uint32_t*) &point.colour;
    memcpy(&image_p->image[y][x], &buffer, sizeof(image_p->image[y][x]));

}


void
xor_point(const point_t point, image_t* image_p)
{
    int x;
    int y;
    x = point.vector.x;
    y = point.vector.y;
    uint32_t buffer = *(uint32_t*) image_p->image[y][x];
    buffer ^= *(uint32_t*) &point.colour;
    memcpy(&image_p->image[y][x], &buffer, sizeof(image_p->image[y][x]));

}

void
average_point(const point_t point, image_t* image_p)
{
    int x;
    int y;
    x = point.vector.x;
    y = point.vector.y;
    for(int colour=0; colour<COLOUR_COUNT; ++colour)
    {
    	image_p->image[y][x][colour] = (uint8_t) ((
    												(uint32_t) image_p->image[y][x][colour]
												  + (uint32_t) ((uint8_t*) &point.colour)[colour])/2);
    }
}

int
point_is_in_image(const point_t* point_p, const image_t* image_p)
{
    return (point_p->vector.x >= 0)
            && (point_p->vector.x < image_p->width)
            && (point_p->vector.y >= 0)
            && (point_p->vector.y < image_p->height);
}

void
image_draw_figure(image_t* image, const figure_t* figure)
{
    unsigned int i;
    for(i = 0; i < figure->amount; ++i)
    {
    	(*public_point_renderer)(figure->sequence[i],image);
    }
}
