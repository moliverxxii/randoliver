/*
 * image_drawing.c
 *
 *  Created on: 2 nov. 2025
 *      Author: moliver
 */
#include <stdio.h>

#include "image_drawing.h"
#include "utility.h"

void
brownien1(image_t* image, int iterations, int spread, int x0, int y0)
{
    colour_t pixels[1] =
    {
        {0, 0xFF, 0}
    };
    int y = y0;
    int x = x0;
    int color = 0;
    int delta = 0;
    int i;
    iterations = 1000000;
    //Début du procesus.
    printf("Début du processus\n");
    for(i = 0; i < iterations; ++i)
    {
        for(color = 0; color < 3; ++color)
        {

            image->image[y][x][color] = pixels[0][color];
            if(color == 1)
            {
                delta = rand() % 3 - 1;
                pixels[0][color] = saturator(pixels[0][color] + delta,
                SAT_MIN,
                SAT_MAX);
            }
        }
        delta = rand() % (2 * spread + 1) - spread;
        y += delta;
        y = modulo(y, image->height);

        delta = rand() % (2 * spread + 1) - spread;
        x += delta;
        x = modulo(x, image->width);
    }
    printf("Processus terminé\n");
}

void
barres1(image_t* image, int spread)
{
    printf("Début du processus\n");

    int i = 0;
    uint32_t y;
    uint32_t x;
    uint32_t x_prime;
    int bool = 1;

    for(y = 0; y < image->height; ++y)
    {
        x = 2;
        x_prime = x;
        bool = 1;
        while(bool)
        {

            for(i = 0; i < 3; ++i)
            {
                *(colour_struct_t*) image->image[y][x_prime] = WHITE;
            }
            x += spread;
            x_prime = x + (rand() % 5 - 2);
            bool = x_prime < image->width;
        }
    }
    printf("Processus terminé\n");

}

void
barres2(image_t* image, int spread)
{
    printf("Début du processus\n");

    int i = 0;
    uint32_t y;
    uint32_t x;
    uint32_t x_prime[3];
    int bool;

    for(y = 0; y < image->height; ++y)
    {
        x = 2;
        for(i = 0; i < 3; ++i)
            x_prime[i] = x;
        bool = 1;
        while(bool)
        {

            for(i = 0; i < 3; ++i)
            {
                *(colour_struct_t*) image->image[y][x_prime[i]] = WHITE;
            }
            x += spread;
            for(i = 0; i < 3; ++i)
                x_prime[i] = x + (rand() % 5 - 2);
            bool = (x_prime[0] < image->width) & (x_prime[1] < image->width)
                    & (x_prime[2] < image->width);
        }
    }
    printf("Processus terminé\n");
}

void
test_pattern_squares(image_t* image_p, uint32_t period)
{
    uint32_t x_squares = (image_p->width + period - 1) / period;
    uint32_t y_squares = (image_p->height + period - 1) / period;
    colour_struct_t* square_colours_p = malloc(x_squares*y_squares*sizeof(colour_struct_t));
    for(uint32_t square = 0; square < x_squares * y_squares; ++ square)
    {
        square_colours_p[square] = colour_get_random();
    }
    for(uint32_t y=0; y < image_p->height; ++y)
    {
        for(uint32_t x=0; x < image_p->width; ++x)
        {
            draw_point((point_t) {
                                    {x, y, 0},
                                    square_colours_p[x/period + (y/period)*x_squares]
                                 },
                       image_p);
        }

    }
    free(square_colours_p);
}


