/*
 * imgUtil.c
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */
#include "main.h"

image_t*
init_image(int width, int height)
{
    //Returns a pointer to an image.

    unsigned int x;
    unsigned int y;
    colour_t* image_data_p = malloc(width*height*sizeof(colour_t));

    image_t* image = (image_t*) malloc(sizeof(image_t));
    image->width = width;
    image->height = height;
    image->image = (colour_t**) malloc(width * sizeof(colour_t*));

	colour_t * row_p = image_data_p;
    for(y = 0; y < height; ++y)
    {
        image->image[y] = row_p;
        row_p += width;
    }
    return image;
}

void
set_image(image_t* image)
{
    //Turns the image black.
    unsigned int x;
    unsigned int y;
    unsigned int color;

    for(x = 0; x < image->width; ++x)
    {
        for(y = 0; y < image->height; ++y)
        {
            for(color = 0; color < COLOUR_COUNT; ++color)
            {
                image->image[y][x][color] = 0x0;
            }
        }
    }
}

void
disp_image(image_t* image)
{
    //Displays the values of the pixels. Line by line.
    int x;
    int y;
    for(x = 0; x < image->width; ++x)
    {
        for(y = 0; y < image->height; ++y)
        {
            printf("Point[%d,%d] = (%x,%x,%x)\n", x + 1, y + 1,
                    image->image[y][x][0], image->image[y][x][1],
                    image->image[y][x][2]);
        }
    }
}

void
draw_rect(colour_t color, int botLeftX, int botLeftY, int topRightX,
          int topRightY, image_t* image)
{
    //Draws a colored rectangle in an image.
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

point_t
init_point()
{
    point_t point;
    point.colour = (colour_struct_t
            )
            {0, 0, 0};
    point.x = 0;
    point.y = 0;
    point.z = 0;
    return point;
}

figure_t
init_figure(uint nombre_Point)
{
    //Initialise une sequence de nombre_Point point_t.
    figure_t figure;

    figure.amount = nombre_Point;
    figure.sequence = (point_t*) malloc(nombre_Point * sizeof(point_t));

    int i;
    for(i = 0; i < nombre_Point; ++i)
    {
        figure.sequence[i] = init_point();
    }
    return figure;
}

void
draw_figure(image_t* image, figure_t* figure)
{
    // Draws a sequence of points into an image.
    unsigned int i;
    int x;
    int y;
    colour_struct_t* point;
    colour_struct_t f_point = {0, 0, 0};
    for(i = 0; i < figure->amount; ++i)
    {
        x = figure->sequence[i].x;
        y = figure->sequence[i].y;
        if((x >= 0) && (x < image->width) && (y >= 0) && (y < image->height))
        {
            point = (colour_struct_t*) (image->image[y][x]);
            *point = *((colour_struct_t*) &figure->sequence[i]);
        }
    }
}


void
free_image(image_t* image)
{
    //Frees the memory the image occupies in memory.
    int x;
    int y;
    free(*(image->image));
    free(image->image);
    free(image);
    image = NULL;
}

void
brownien1(image_t* image, int iterations, int spread, int x0, int y0)
{
    /*Ajoute un mouvement brownien à l'image de longueur "iterations" et de
     *point
     *de départ (x0,y0).
     */
    uchar pixels[1][3] =
    {
    {0, 0xFF, 0}};
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
    /*Ajoutes des barres blanches brouillées verticales à l'image.
     Elles sont espacées centre-à-centre
     *de la distance "spread".
     */
    printf("Début du processus\n");

    colour_t pixel =
    {0xFF, 0xFF, 0xFF};
    int i = 0;
    int y;
    int x;
    int xPrime;
    char bool = 1;

    for(y = 0; y < image->height; ++y)
    {
        x = 2;
        xPrime = x;
        bool = 1;
        while(bool)
        {

            for(i = 0; i < 3; ++i)
            {
                image->image[xPrime][y][i] = pixel[i];
            }
            x += spread;
            xPrime = x + (rand() % 5 - 2);
            bool = xPrime < image->width;
        }
    }
    printf("Processus terminé\n");

}

void
barres2(image_t* image, int spread)
{
    /*Ajoutes des barres verticales brouillées à l'image.
     Elles sont espacées centre-à-centre
     *de la distance "spread".
     */
    printf("Début du processus\n");

    colour_t pixel =
    {0xFF, 0xFF, 0xFF};
    int i = 0;
    int y;
    int x;
    uint xPrime[3];
    uchar bool;

    for(y = 0; y < image->height; ++y)
    {
        x = 2;
        for(i = 0; i < 3; ++i)
            xPrime[i] = x;
        bool = 1;
        while(bool)
        {

            for(i = 0; i < 3; ++i)
            {
                image->image[xPrime[i]][y][i] = pixel[i];
            }
            x += spread;
            for(i = 0; i < 3; ++i)
                xPrime[i] = x + (rand() % 5 - 2);
            bool = (xPrime[0] < image->width) & (xPrime[1] < image->width)
                    & (xPrime[2] < image->width);
        }
    }
    printf("Processus terminé\n");

}

void
rand_coord(point_t* m, int height, int width)
{
    int randX = rand() % width;
    int randY = rand() % height;
    m->x = randX;
    m->y = randY;
}

void
rand_delta_point(point_t* m, int amplitude, int width, int height)
{
    //Randomly moves a point.
    m->x += rand() % (2 * amplitude + 1) - amplitude;
    modulo(m->x, width);
    m->y += rand() % (2 * amplitude + 1) - amplitude;
    modulo(m->y, height);
}

void
free_points(point_t** seq, int n)
{
    int i;
    for(i = 0; i < n; ++i)
    {
        free(seq[i]);
    }
    free(seq);

}

void
init_camera(camera_t* camera_p, float origin_x, float origin_y,
        float origin_z, float destin_x, float destin_y, float destin_z,
        float distance)
{
    camera_t camera_loc;
    camera_loc.origin.x = origin_x;
    camera_loc.origin.y = origin_y;
    camera_loc.origin.z = origin_z;
    camera_loc.direction.x = destin_x;
    camera_loc.direction.y = destin_y;
    camera_loc.direction.z = destin_z;
    camera_loc.distance = distance;

    (*camera_p) = camera_loc;
}

void
render_figure(image_t* image, figure_t figure, camera_t camera)
{

    vector_t o;
    vector_t f;
    vector_t p;
    vector_t op;

    vector_t u;
    vector_t v;
    vector_t w;

    int x_image;
    int y_image;
    colour_struct_t* point;

    float distance = camera.distance;

    o.x = camera.origin.x;
    o.y = camera.origin.y;
    o.z = camera.origin.z;

    f.x = camera.direction.x;
    f.y = camera.direction.y;
    f.z = camera.direction.z;

    float norme_of = sqrt(
            pow(f.x - o.x, 2) + pow(f.y - o.y, 2) + pow(f.z - o.z, 2));
    u.x = (f.x - o.x) / norme_of;
    u.y = (f.y - o.y) / norme_of;
    u.z = (f.z - o.z) / norme_of;

    v.x = -u.y / sqrt(pow(u.x, 2) + pow(u.y, 2));
    v.y = u.x / sqrt(pow(u.x, 2) + pow(u.y, 2));
    v.z = 0;

    w.x = -u.z * u.x / sqrt(pow(u.x, 2) + pow(u.y, 2));
    w.y = u.z * u.y / sqrt(pow(u.x, 2) + pow(u.y, 2));
    w.z = sqrt(pow(u.x, 2) + pow(u.y, 2));

    //	printf("u %f,%f,%f\n",u.x,u.y,u.z);
    //	printf("v %f,%f,%f\n",v.x,v.y,v.z);
    //	printf("w %f,%f,%f\n",w.x,w.y,w.z);

    float op_u_scalaire;
    float op_v_scalaire;
    float op_w_scalaire;

    for(int i = 0; i < figure.amount; ++i)
    {
        p.x = figure.sequence[i].x;
        p.y = figure.sequence[i].y;
        p.z = figure.sequence[i].z;

        op.x = p.x - o.x;
        op.y = p.y - o.y;
        op.z = p.z - o.z;
        //		printf("op %f , %f , %f\n",op.x,op.y, op.z);

        op_u_scalaire = op.x * u.x + op.y * u.y + op.z * u.z;
        op_v_scalaire = op.x * v.x + op.y * v.y;
        op_w_scalaire = op.x * w.x + op.y + w.y + op.z * w.z;
        //		printf("op u %f\n",op_u_scalaire);
        //		printf("op v %f\n",op_v_scalaire);
        //		printf("op w %f\n",op_w_scalaire);

        x_image = 640 + distance * op_v_scalaire / op_u_scalaire;
        y_image = 400 + distance * op_w_scalaire / op_u_scalaire;

        //		printf("%d,%d\n\n",x_image,y_image);
        if((x_image < 1280) && (x_image >= 0) && (y_image < 800)
                && (y_image >= 0) && op_u_scalaire > 0)
        {
            point = (colour_struct_t*) image->image[x_image][y_image];
            *point = figure.sequence[i].colour;
        }
    }
}
