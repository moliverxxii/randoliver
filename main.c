/*
 * main.c
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */
#include "main.h"

int main()
{
    //Paramètres de base.
    char* nom = "Fantôme V7";
    int width = 1280;
    int height = 800;

    //Initialisation de l'image.
    image_t image = initImage(width,height);
    FILE* fichier = initImageFile(nom,width,height,image);

    //Initialisation des particules
    srand(time(NULL));

    uint nb = 400;
    int distX = 100;
    figure_t test = initFigure(nb);
    uchar color[3] = {0xFF,0xFF,0xFF};
    camera_t camera;
    camera.origine.x = 350;
    camera.origine.y = 0;
    camera.origine.z = 100;

    camera.direction.x = 350;
    camera.direction.y = 10;
    camera.direction.z = 100;

    camera.distance = 100;

    for(int i=0; i<nb;++i)
    {
        test.sequence[i].x = (i%8)*distX ;
        test.sequence[i].y = distX* (i - i%8)/8-100;
        test.sequence[i].z = 0;
        //		printf("x,y = %d,%d\n",test.sequence[i].x,test.sequence[i].y);
    }

    renderFigure(image,test,camera,color);

    for(int i=0; i<5;++i)flou(image,1280,800);
    writeImage(image,1280,800,fichier);


    return EXIT_SUCCESS;
}
