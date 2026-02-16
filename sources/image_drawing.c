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
    colour_t pixel = {{0, 0xFF, 0}};
    int y = y0;
    int x = x0;
    int colour = 0;
    int delta = 0;
    int i;
    iterations = 1000000;
    //Début du procesus.
    printf("Début du processus\n");
    for(i = 0; i < iterations; ++i)
    {
        image_pixel_set(image, x, y, pixel);
        for(colour = 0; colour < 3; ++colour)
        {

            if(colour == 1)
            {
                delta = rand() % 3 - 1;
                pixel.array[colour] = saturator(pixel.array[colour] + delta,
                COLOUR_MIN,
                COLOUR_MAX);
            }
        }
        delta = rand() % (2 * spread + 1) - spread;
        y += delta;
        y = modulo(y, image_height(image));

        delta = rand() % (2 * spread + 1) - spread;
        x += delta;
        x = modulo(x, image_width(image));
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

    for(y = 0; y < image_height(image); ++y)
    {
        x = 2;
        x_prime = x;
        bool = 1;
        while(bool)
        {

            for(i = 0; i < 3; ++i)
            {
                image_pixel_set(image, x_prime, y, WHITE);
            }
            x += spread;
            x_prime = x + (rand() % 5 - 2);
            bool = x_prime < image_width(image);
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

    for(y = 0; y < image_height(image); ++y)
    {
        x = 2;
        for(i = 0; i < 3; ++i)
            x_prime[i] = x;
        bool = 1;
        while(bool)
        {

            for(i = 0; i < 3; ++i)
            {
                image_pixel_set(image, x_prime[i], y, WHITE);
            }
            x += spread;
            for(i = 0; i < 3; ++i)
                x_prime[i] = x + (rand() % 5 - 2);
            bool = (x_prime[0] < image_width(image)) && (x_prime[1] < image_width(image))
                 && (x_prime[2] < image_width(image));
        }
    }
    printf("Processus terminé\n");
}

void
test_pattern_squares(image_t* image_p, uint32_t period)
{
    uint32_t x_squares = (image_width(image_p)  + period - 1) / period;
    uint32_t y_squares = (image_height(image_p) + period - 1) / period;
    colour_t* square_colours_p = malloc(x_squares*y_squares*sizeof(colour_t));
    for(uint32_t square = 0; square < x_squares * y_squares; ++ square)
    {
        square_colours_p[square] = colour_get_random();
    }
    for(uint32_t y=0; y < image_height(image_p); ++y)
    {
        for(uint32_t x=0; x < image_width(image_p); ++x)
        {
            image_draw_point(image_p,
                             (point_t) {
                                          vector_init(x, y, 0),
                                          square_colours_p[x/period + (y/period)*x_squares]
                                       }
                            );
        }

    }
    free(square_colours_p);
}


