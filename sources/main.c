/*
 * main.c
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */
#include "main.h"

//#define OLI_3D
#define OLI_FIG

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
    int height = 720;

    char* nom_etendu;
    FILE* fichier;

    //Initialisation de l'image.
    image_t* image = initImage(width, height);

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

    renderFigure(image, test, camera);
    for(int i = 0; i < 5; ++i)
    {
        flou(image);
    }
#endif /* OLI_3D */
#ifdef OLI_BROWN
    brownien1(image, 30000, 1, width/2, height/2);
#endif /* OLI_BROWN */
#ifdef OLI_FIG
    unsigned int num_points = 20000000;
    figure_t fig = initFigure(num_points);
    for(int i=0; i<fig.nombre_Point;++i)
    {
         fig.sequence[i].x = image->width/2;
         fig.sequence[i].y = image->height/2;
         unsigned int color = rand();
         fig.sequence[i].color = *((color_struct_t*) &color);
    }

    for(int j=0; j<2000; ++j)
    {
        for(int i=0; i<fig.nombre_Point;++i)
        {
        randDeltaPoint(&fig.sequence[i],
                1, image->width,
                image->height);
        }
        nom_etendu = numExtension(nom, j);

        fichier = initImageFile(nom_etendu, image);
        drawFigure(image, &fig);
        writeImage(image, fichier);
        free(nom_etendu);
        setImage(image);
    }
#endif





    return EXIT_SUCCESS;
}
