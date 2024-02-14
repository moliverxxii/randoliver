/*
 * main.c
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */
#include "main.h"

//#define OLI_3D
//#define OLI_FIG

int
main(int argc, char* argv[])
{
    init_interface();
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
    image_t* image1 = init_image(width, height);
    image_t* image2 = init_image(width, height);

    //Initialisation des particules
    srand(time(NULL));
    random_image(image);
    random_image(image1);
    random_image(image2);
    flou(image,  3);
    flou(image1, 3);


    add_images(image, image1);

    for(int j=0; j<2000; ++j)
    {
        reset_line();
        printf("Image %u\n", j);
        file_name = num_extension(nom, j);
        random_colour_shift(image, 20);
        flou(image, 1);
        symetry(image);


        file = init_image_file(file_name, image);

        write_image(image, file);
        free(file_name);
        fclose(file);
    }

    file_name = "bloup";




#ifdef OLI_3D
    uint32_t nb = 400;
    int dist_x = 100;
    figure_t test = init_figure(nb);
    colour_t colour;
    *(colour_struct_t*) &colour = RED;
    camera_t camera;
    init_camera(&camera, 350, 0, 100, 350, 10, 100, 100);
    for(int i = 0; i < nb; ++i)
    {
        test.sequence[i].vector.x = (i % 8) * dist_x;
        test.sequence[i].vector.y = dist_x * (i - i % 8) / 8 - 100;
        test.sequence[i].vector.z = 0;
        test.sequence[i].colour = get_random_colour();
//        printf("x,y = %d,%d\n",test.sequence[i].x,test.sequence[i].y);
    }
    int frame_count = 2000;

    vector_t centre_grave = get_average_point(&test);
    vector_t centre_grave_z;
    add_vectors(&centre_grave_z, centre_grave, (vector_t) {0,0,1});
    int j=0;
    do
    {
        printf("Image %u\n", j);
        render_figure(image, test, camera);

        file_name = num_extension(nom, j);

        file = init_image_file(file_name, image);

        //OPERATION
        for(int point_n = 0; point_n< test.amount; point_n++)
        {
            rotate_vector(&test.sequence[point_n].vector, centre_grave, centre_grave_z, 2*M_PI/360);
        }


        free(file_name);
        fclose(file);
        set_image(image);
        ++j;
    } while (j<frame_count);


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
