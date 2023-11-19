/*
 * main.c
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */
#include "main.h"

//#define OLI_3D
#define OLI_FIG

int
main(int argc, char* argv[])
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
    image_t* image = init_image(width, height);

    //Initialisation des particules
    srand(time(NULL));



#ifdef OLI_3D
    uint nb = 400;
    int distX = 100;
    figure_t test = init_figure(nb);
    color_t color =
    {0x0, 0x0, 0xFF};
    camera_t camera;
    initCamera(&camera, 350, 0, 100, 350, 10, 100, 100);
    for(int i = 0; i < nb; ++i)
    {
        test.sequence[i].x = (i % 8) * distX;
        test.sequence[i].y = distX * (i - i % 8) / 8 - 100;
        test.sequence[i].z = 0;
        test.sequence[i].color = *((colour_struct_t*) color);
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
    figure_t fig = init_figure(1000);
    for(int i=0; i<fig.amount;++i)
    {
         rand_coord(&fig.sequence[i], image->height, image->width);
         fig.sequence[i].colour = (colour_struct_t) {0xFF, 0xFF, 0xFF};
    }

    for(int j=0; j<2000; ++j)
    {
        for(int i=0; i<fig.amount;++i)
        {
        rand_delta_point(&fig.sequence[i],
                1, image->width,
                image->height);
        }
        nom_etendu = num_extension(nom, j);

        fichier = init_image_file(nom_etendu, image);
        draw_figure(image, &fig);
        write_image(image, fichier);
        free(nom_etendu);
        set_image(image);
    }
#endif

    return EXIT_SUCCESS;
}
