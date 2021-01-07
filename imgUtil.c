/*
 * imgUtil.c
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */
#include "main.h"

image_t initImage(int width, int height)
{
    //Returns a pointer to an image.

    int x;
    int y;
    image_t image = (image_t) malloc(width*sizeof(uchar**));

    for(x=0; x<width; ++x)
    {
        image[x] = (uchar**) malloc(height*
                sizeof(uchar*));
        if(image[x] == NULL) exit(1);
        for(y=0;y<height; ++y)
        {
            image[x][y] = (uchar*) malloc(3*sizeof(uchar));
            if(image[x][y] == NULL) exit(1);
        }
    }
    printf("image à : %p\n",image);
    return image;
}

void setImage(image_t image, int width, int height)
{
    //Turns the image black.
    int x;
    int y;
    int color;

    for(x=0; x<width; ++x)
    {
        for(y=0; y<height;++y)
        {
            for(color=0; color<3; ++color)
            {
                image[x][y][color] = 0x0;
            }
        }
    }
}

void dispImage(image_t image, int width, int height)
{
    //Displays the values of the pixels. Line by line.
    int x;
    int y;
    for(x = 0; x<width;++x)
    {
        for(y = 0;y<height;++y)
        {
            printf("Point[%d,%d] = (%x,%x,%x)\n",
                    x+1,y+1,
                    image[x][y][0],
                    image[x][y][1],
                    image[x][y][2]);
        }
    }
}

void drawRect(uchar* color,
        int botLeftX,
        int botLeftY,
        int topRightX,
        int topRightY,
        image_t image)
{
    //Draws a colored rectangle in an image.
    int x;
    int y;
    int k;
    for(x=botLeftX; x <= topRightX; ++x)
    {
        for(y=botLeftY; y <= topRightY; ++y)
        {
            for(k=0;k<3;++k)
            {
                image[x][y][k] = color[k];
            }
        }
    }
}

void drawPoints(point_t** seq,int nPoints, image_t image)
{
    // Draws a sequence of points into an image.
    int i;
    int x;
    int y;
    for(i=0;i<nPoints;++i)
    {
        x = seq[i] -> x;
        y = seq[i] -> y;
        image[x][y][0] = seq[i] -> blue;
        image[x][y][1] = seq[i] -> green;
        image[x][y][2] = seq[i] -> red;
    }

}


point_t initPoint()
{
    point_t point;
    point.blue = 0;
    point.green = 0;
    point.red = 0;
    point.x = 0;
    point.y = 0;
    point.z = 0;
    return point;
}

figure_t initFigure(uint nombre_Point)
{
    //Initialise une sequence de nombre_Point point_t.
    figure_t figure;

    figure.nombre_Point = nombre_Point;
    figure.sequence =  (point_t*) malloc(nombre_Point * sizeof(point_t));

    int i;
    for(i=0;i<nombre_Point;++i)
    {
        figure.sequence[i] = initPoint();
    }
    return figure;
}


void freeImage(image_t image, int width, int height)
{
    //Frees the memory the image occupies in memory.
    int x;
    int y;
    for(x=0;x<width;++x)
    {
        for(y=0;y<height;++y)
        {
            free(image[x][y]);
        }
        free(image[x]);
    }
    free(image);
}

void brownien2(image_t image,int width,int height,int iterations, int spread,int x0, int y0)
{
    /*Ajoute un mouvement brownien à l'image de longueur "iterations" et de point
     *de départ (x0,y0).
     */
    uchar pixels[1][3] = { {0,0xFF,0} };
    int y =  y0;
    int x = x0;
    int color = 0;
    int delta = 0;
    int i;
    iterations = 1000000;
    //Début du procesus.
    printf("Début du processus\n");
    for(i=0;i<iterations;++i)
    {
        for(color=0;color<3; ++color)
        {

            image[x][y][color] = pixels[0][color];
            if(color == 1)
            {
                delta = rand()%3-1;
                pixels[0][color] = saturator(pixels[0][color] + delta ,SAT_MIN, SAT_MAX);
            }
        }
        delta = rand()%(2*spread + 1) - spread;
        y += delta;
        y = modulo(y,height);

        delta = rand()%(2*spread + 1) - spread;
        x += delta;
        x = modulo(x,width);


        if( (i%10000) == 0)
        {
            printf("point (%d,%d)",x,y);
            ok(i);
        }

    }
    printf("Processus terminé\n");
}

void barres1(image_t image, int width, int height, int spread)
{
    /*Ajoutes des barres blanches brouillées verticales à l'image. Elles sont espacées centre-à-centre
     *de la distance "spread".
     */
    printf("Début du processus\n");

    uchar pixel[3] = {0xFF,0xFF,0xFF};
    int i = 0;
    int y;
    int x;
    int xPrime;
    char bool=1;

    for(y = 0; y<height; ++y)
    {
        x = 2;
        xPrime = x;
        bool=1;
        while(bool)
        {

            for(i=0;i<3;++i)
            {
                image[xPrime][y][i] = pixel[i];
            }
            x += spread;
            xPrime = x + (rand()%5 - 2);
            bool = xPrime<width;
        }
    }
    printf("Processus terminé\n");

}

void barres2(image_t image, int width, int height, int spread)
{
    /*Ajoutes des barres verticales brouillées à l'image. Elles sont espacées centre-à-centre
     *de la distance "spread".
     */
    printf("Début du processus\n");

    uchar pixel[3] = {0xFF,0xFF,0xFF};
    int i = 0;
    int y;
    int x;
    int* xPrime = calloc(3,sizeof(int));
    char bool=1;

    for(y = 0; y<height; ++y){
        x = 2;
        for(i=0;i<3;++i) xPrime[i] = x;
        bool=1;
        while(bool)
        {

            for(i=0;i<3;++i)
            {
                image[xPrime[i]][y][i] = pixel[i];
            }
            x += spread;
            for(i=0;i<3;++i) xPrime[i] = x + (rand()%5 - 2);
            bool = (xPrime[0]<width) & (xPrime[1]<width) & (xPrime[2]<width);
        }
    }
    printf("Processus terminé\n");

}


void randCoord(point_t* m,int height, int width)
{
    int randX = rand()%width;
    int randY = rand()%height;
    m -> x = randX;
    m -> y = randY;
}

void randDeltaPoint(point_t* m, int amplitude,int width, int height)
{
    //Randomly moves a point.
    m -> x += rand()%( 2*amplitude + 1 ) - amplitude;
    modulo(m -> x,width);
    m -> y += rand()%( 2*amplitude + 1 ) - amplitude;
    modulo(m -> y,height);
}

void freePoints(point_t** seq, int n)
{
    int i;
    for(i=0;i<n;++i)
    {
        free(seq[i]);
    }
    free(seq);
}

void renderFigure(image_t image, figure_t figure ,camera_t camera, uchar* color)
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

    float distance = camera.distance;

    o.x = camera.origine.x;
    o.y = camera.origine.y;
    o.z = camera.origine.z;

    f.x = camera.direction.x;
    f.y = camera.direction.y;
    f.z = camera.direction.z;

    float norme_of = sqrt(pow(f.x-o.x, 2)
            + pow(f.y-o.y, 2)
            + pow(f.z-o.z, 2));
    u.x = (f.x - o.x)/norme_of;
    u.y = (f.y - o.y)/norme_of;
    u.z = (f.z - o.z)/norme_of;

    v.x = -u.y/sqrt(pow(u.x,2) + pow(u.y,2));
    v.y = u.x/sqrt(pow(u.x,2) + pow(u.y,2));
    v.z=0;

    w.x = -u.z * u.x / sqrt(pow(u.x,2) + pow(u.y,2));
    w.y = u.z * u.y / sqrt(pow(u.x,2) + pow(u.y,2));
    w.z = sqrt(pow(u.x,2) + pow(u.y,2));

    //	printf("u %f,%f,%f\n",u.x,u.y,u.z);
    //	printf("v %f,%f,%f\n",v.x,v.y,v.z);
    //	printf("w %f,%f,%f\n",w.x,w.y,w.z);

    float op_u_scalaire;
    float op_v_scalaire;
    float op_w_scalaire;

    for(int i=0;i<figure.nombre_Point;++i)
    {
        p.x = figure.sequence[i].x;
        p.y = figure.sequence[i].y;
        p.z = figure.sequence[i].z;

        op.x = p.x - o.x;
        op.y = p.y - o.y;
        op.z = p.z - o.z;
        //		printf("op %f , %f , %f\n",op.x,op.y, op.z);

        op_u_scalaire = op.x*u.x + op.y*u.y + op.z*u.z;
        op_v_scalaire = op.x*v.x + op.y*v.y;
        op_w_scalaire = op.x*w.x + op.y+w.y + op.z*w.z;
        //		printf("op u %f\n",op_u_scalaire);
        //		printf("op v %f\n",op_v_scalaire);
        //		printf("op w %f\n",op_w_scalaire);

        x_image = 640 + distance*op_v_scalaire/op_u_scalaire;
        y_image = 400 + distance*op_w_scalaire/op_u_scalaire;

        //		printf("%d,%d\n\n",x_image,y_image);
        if( (x_image<1280) && (x_image>=0) && (y_image < 800) && (y_image >= 0) && op_u_scalaire>0)
        {
            image[x_image][y_image][0] = color[0];
            image[x_image][y_image][1] = color[1];
            image[x_image][y_image][2] = color[2];
        }
    }
}
