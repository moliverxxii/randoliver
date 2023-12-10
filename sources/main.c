/*
 * main.c
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */
#include "main.h"

#define OLI_3D
//#define OLI_FIG

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

    char* file_name;
    FILE* file;

    //Initialisation de l'image.
    image_t* image = init_image(width, height);

    //Initialisation des particules
    srand(time(NULL));



#ifdef OLI_3D
    uint32_t nb = 400;
    int dist_x = 100;
    figure_t test = init_figure(nb);
    colour_t colour;
    *(colour_struct_t*) &colour = RED;
    camera_t camera;
    init_camera(&camera, 350, 0, 100, 350, 10, 100, 100);
    vector_t vector_z2 = {1, 0, 100};
    matrix_3x3_t rotation;
    get_rotation(rotation, VECTOR_0, vector_z2);
    for(int i = 0; i < nb; ++i)
    {
        test.sequence[i].x = (i % 8) * dist_x;
        test.sequence[i].y = dist_x * (i - i % 8) / 8 - 100;
        test.sequence[i].z = 0;
        test.sequence[i].colour = get_random_colour();
//        printf("x,y = %d,%d\n",test.sequence[i].x,test.sequence[i].y);
    }
    vector_t* vector_array = malloc(sizeof(vector_t)*nb);
    for(int point_n = 0; point_n< test.amount; point_n++)
    {
        point_to_vector(&vector_array[point_n], test.sequence[point_n]);
    }


    int j=0;
    do
    {
        printf("Image %u\n", j);
        render_figure(image, test, camera);
        file_name = num_extension(nom, j);

        file = init_image_file(file_name, image);

        space_operation(vector_array, rotation, vector_array, test.amount);
        for(int point_n = 0; point_n< test.amount; point_n++)
        {
            vector_to_point(&test.sequence[point_n], vector_array[point_n]);
        }

        free(file_name);
        fclose(file);
        set_image(image);
        ++j;
    } while (j<2000);


    strcpy(file_name, "a");
    file = init_image_file(file_name, image);
    fclose(file);
#endif /* OLI_3D */

#ifdef OLI_BROWN
    brownien1(image, 30000, 1, width/2, height/2);
#endif /* OLI_BROWN */
#ifdef OLI_FIG
    unsigned int num_points = 20000;
    figure_t fig = init_figure(num_points);
    for(int i=0; i<fig.amount; ++i)
    {
         fig.sequence[i].x = image->width/2;
         fig.sequence[i].y = image->height/2;
         fig.sequence[i].colour = get_random_colour();
    }

    for(int j=0; j<2000; ++j)
    {
    	printf("Image %u\n", j);
        for(int i=0; i<fig.amount;++i)
        {
			rand_delta_point(&fig.sequence[i],
							 1,
							 image->width,
							 image->height);
        }
        file_name = num_extension(nom, j);

        file = init_image_file(file_name, image);

        draw_figure(image, &fig);
        write_image(image, file);
        free(file_name);
    	fclose(file);
        set_image(image);
    }
#endif

    return EXIT_SUCCESS;
}
