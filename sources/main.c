/*
 * main.c
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */
#include "main.h"

#define OLI_3D

int main(int argc, char* argv[])
{
    //Paramètres de base.
    char* nom = malloc(sizeof(char) * 40);
    printf("argc=%d\n", argc);
    if(1 < argc)
    {
        strcpy(nom, argv[1]);
    }
    else
    {
        strcpy(nom, "sans titre");
    }
    int width = 1280;
    int height = 800;

    //Initialisation de l'image.
    image_t* image = initImage(width, height);
    FILE* fichier = initImageFile(nom, image);

    //Initialisation des particules
    srand(time(NULL));



#ifdef OLI_3D
    uint nb = 400;
    int distX = 100;
    figure_t test = initFigure(nb);
    color_t color =
    {0x0, 0x0, 0xFF};
    camera_t camera;
    initCamera(&camera, 350, 0, 100, 350, 10, 100, 100);
    for(int i = 0; i < nb; ++i)
    {
        test.sequence[i].x = (i % 8) * distX;
        test.sequence[i].y = distX * (i - i % 8) / 8 - 100;
        test.sequence[i].z = 0;
        test.sequence[i].color = *((color_struct_t*) color);
        //		printf("x,y = %d,%d\n",test.sequence[i].x,test.sequence[i].y);
    }

    ok(0);
    renderFigure(image, test, camera);
    ok(1);
    for(int i = 0; i < 5; ++i)
        flou(image);
#else
    brownien1(image, 30000, 1, width/2, height/2);
#endif /* OLI_3D */

    writeImage(image, fichier);

    return EXIT_SUCCESS;
}
