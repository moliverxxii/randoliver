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
#include "interface.h"

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

static void
scale_pixel(image_t* new_image_p, const image_t* image_p, uint32_t new_x, uint32_t new_y, float scale)
{
    float x_source = new_x/scale;
    float y_source = new_y/scale;
    memcpy(new_image_p->image[new_y][new_x],
           image_p->image[(uint32_t) y_source][(uint32_t) x_source],
           sizeof(colour_t));
}

static void
scale_pixel_linear(image_t* new_image_p, const image_t* image_p, uint32_t new_x, uint32_t new_y, float scale)
{
    float x_n = new_x/scale;
    float y_n = new_y/scale;
    float colour_new[COLOUR_COUNT] = {0, 0, 0};


    if(scale>1.0f)
    {
        float f_new_x = (float) new_x + 1.0f/2;
        float e_new_x = f_new_x / scale;
        int32_t n_x = (int32_t) floorf(e_new_x - 1.0f/2);
        float e_n_x = n_x + 1.0f/2;
        float colour_factor_x[2] = {0, e_new_x - e_n_x};
        colour_factor_x[0] = 1 - colour_factor_x[1];

        float f_new_y = (float) new_y + 1.0f/2;
        float e_new_y = f_new_y / scale;
        int32_t n_y = (int32_t) floorf(e_new_y - 1.0f/2);
        float e_n_y = n_y + 1.0f/2;
        float colour_factor_y[2] = {0, e_new_y - e_n_y};
        colour_factor_y[0] = 1 - colour_factor_y[1];

        float colour_factor_sum = 0;

        for(int32_t n_y_offset=0; n_y_offset<=1; ++n_y_offset)
        {

            if(n_y + n_y_offset >= (int32_t) image_p->height
                || n_y + n_y_offset < 0)
            {
                continue;
            }

            for(int32_t n_x_offset=0; n_x_offset<=1; ++n_x_offset)
            {
                if(n_x + n_x_offset >= (int32_t) image_p->width
                    || n_x + n_x_offset < 0)
                {
                    continue;
                }

                float colour_factor = colour_factor_y[n_y_offset] * colour_factor_x[n_x_offset];
                for(uint8_t colour_index = 0;
                    colour_index < COLOUR_COUNT;
                    ++colour_index)
                {
                    colour_new[colour_index] +=
                             colour_factor * (float) image_p->image[n_y + n_y_offset][n_x + n_x_offset][colour_index];
                }
                colour_factor_sum += colour_factor;

            }
        }


        for(uint8_t colour_index = 0; colour_index < COLOUR_COUNT;
                ++colour_index)
        {
            colour_new[colour_index] /= colour_factor_sum;
        }
    }
    else
    {
        uint32_t x_n_f = (uint32_t) x_n;
        uint32_t y_n_f = (uint32_t) y_n;
        float y_n1 = (new_y + 1)/scale;
        uint32_t y_n1_f = (uint32_t) y_n1;
        float x_n1 = (new_x + 1)/scale;
        uint32_t x_n1_f = (uint32_t) x_n1;
        float y_coef = 0;

        for(uint32_t y_i = y_n_f; y_i <= y_n1_f; ++y_i)
        {
            if(y_i == y_n_f)
            {
                y_coef = (float) y_n_f + 1 - y_n;
            }
            else if(y_i == y_n1_f)
            {
                y_coef = y_n1 - (float) y_n1_f;
            }
            else
            {
                y_coef = 1;
            }

            if(y_coef <= 0)
            {
                continue;
            }

            for(uint32_t x_i = x_n_f; x_i <= x_n1_f; ++x_i)
            {
                float x_coef = 0;

                if(x_i == x_n_f)
                {
                    x_coef = (float) x_n_f + 1 - x_n;
                }
                else if(x_i == x_n1_f)
                {
                    x_coef = x_n1 - (float) x_n1_f;
                }
                else
                {
                    x_coef = 1;
                }

                if(x_coef <= 0)
                {
                    continue;
                }


                for(uint8_t colour_index = 0; colour_index < COLOUR_COUNT;
                        ++colour_index)
                {
                    colour_new[colour_index] +=
                            y_coef * x_coef * (float) image_p->image[y_i][x_i][colour_index];
                }
            }
        }
        for(uint8_t colour_index = 0; colour_index<COLOUR_COUNT; ++colour_index)
        {
            colour_new[colour_index] *= scale*scale;
        }

    }
    colour_t colour = {colour_new[0], colour_new[1], colour_new[2]};
    memcpy(new_image_p->image[new_y][new_x], colour, sizeof(colour_t));
}

void
image_scale(image_t** image_pp, float scale, image_scale_algorithm_t algorithm)
{
    uint32_t new_width  = (*image_pp)->width * scale;
    uint32_t new_height = (*image_pp)->height * scale;
    image_t* new_image_p = image_init(new_width, new_height);
    for(uint32_t y=0; y<new_height; ++y)
    {
        for(uint32_t x=0; x<new_width; ++x)
        {
            switch(algorithm)
            {
            case SCALE_ALGORITHM_DUMB:
                scale_pixel(new_image_p, *image_pp, x, y, scale);
                break;
            case SCALE_ALGORITHM_LINEAR:
                scale_pixel_linear(new_image_p, *image_pp, x, y, scale);
                break;

            }
        }
    }
    image_free(*image_pp);
    *image_pp = new_image_p;
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
