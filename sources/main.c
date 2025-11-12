/*
 * main.c
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "interface.h"
#include "image.h"
#include "image_file.h"
#include "image_drawing.h"
#include "camera.h"
#include "utility.h"

//#define OLI_TEST_PATTERN
//#define OLI_3D
//#define OLI_BROWN
//#define OLI_FIG
#define OLI_FIG_2

int
main(int argc, char* argv[])
{
    interface_init();
    //Paramètres de base.
    char* file_name_prefix_p = malloc(sizeof(char) * 40);
    if(1 < argc)
    {
        strcpy(file_name_prefix_p, argv[1]);
    }
    else
    {
        strcpy(file_name_prefix_p, "sans titre");
    }
    int width = 1920;
    int height = 960;

    image_file_t* image_file_p;

    //Initialisation de l'image.
    image_t* image_p  = image_init(width, height);

    //Initialisation des particules
    srand(time(NULL));

    interface_state_save();

#ifdef OLI_TEST_PATTERN
    image_scale(&image_p, 1.0f/height, SCALE_ALGORITHM_LINEAR);
    test_pattern_squares(image_p, 1);
    image_scale(&image_p, 960, SCALE_ALGORITHM_LINEAR);
    image_scale(&image_p, 10.0f/960, SCALE_ALGORITHM_LINEAR);
    image_scale(&image_p, 96, SCALE_ALGORITHM_LINEAR);
    image_file_p = image_file_init(file_name_prefix_p, image_p);
    image_file_free(image_file_p);
#endif //OLI_TEST_PATTERN


#ifdef OLI_3D
    uint32_t nb = 400;
    int dist_x = 100;
    figure_t test = figure_init(nb);
    colour_t colour;
    *(colour_struct_t*) &colour = RED;
    camera_t camera = camera_init(350, 0, 100, 350, 10, 100, 100);
    for(uint32_t point_n = 0; point_n < nb; ++point_n)
    {
        test.sequence[point_n].vector.x = (point_n % 8) * dist_x;
        test.sequence[point_n].vector.y = (dist_x * (float) (point_n - point_n % 8)) / 8 - 100;
        test.sequence[point_n].vector.z = 0;
        test.sequence[point_n].colour = colour_get_random();
//        printf("x,y = %d,%d\n",test.sequence[i].x,test.sequence[i].y);
    }
    int frame_count = 360;

    vector_t centre_grave = figure_get_average_point(&test);
    vector_t centre_grave_z;
    add_vectors(&centre_grave_z, centre_grave, (vector_t) {0,0,1});
    int frame=0;
    interface_state_save();
    do
    {
        interface_state_restore();
        printf("Image %u\n", frame);
        camera_render_figure(&camera, image_p, test);

        char* file_name_p = num_extension(file_name_prefix_p, frame);

        image_file_p = image_file_init(file_name_p, image_p);

        //OPERATION
        for(uint32_t point_n = 0; point_n< test.amount; point_n++)
        {
            rotate_vector(&test.sequence[point_n].vector,
                          centre_grave,
                          centre_grave_z,
                          2.*M_PI/360.);
        }

        free(file_name_p);
        image_file_free(image_file_p);
        image_file_p = NULL;
        image_set(image_p);
        ++frame;
    } while (frame<frame_count);

#endif /* OLI_3D */

#ifdef OLI_BROWN
    brownien1(image_p, 30000, 1, width/2, height/2);
    image_file_p = image_file_init(file_name_prefix_p, image_p);

    image_file_write(image_file_p, image_p);
    image_file_free(image_file_p);
    image_set(image_p);
#endif /* OLI_BROWN */

#ifdef OLI_FIG
    unsigned int num_points = 20000;
    figure_t fig = figure_init(num_points);
    for(uint32_t i=0; i<fig.amount; ++i)
    {
         fig.sequence[i].vector.x = image_p->width/2;
         fig.sequence[i].vector.y = image_p->height/2;
         fig.sequence[i].colour = colour_get_random();
    }
    

    for(int frame=0; frame<200; ++frame)
    {
        interface_state_restore();
        printf("Image %u\n", frame);
        for(uint32_t i=0; i<fig.amount;++i)
        {
           rand_delta_vector(&fig.sequence[i].vector,
                             1,
                             image_p->width,
                             image_p->height);
        }
        char* file_name_p = num_extension(file_name_prefix_p, frame);

        image_file_p = image_file_init(file_name_p, image_p);

        image_draw_figure(image_p, &fig);
        image_file_write(image_file_p, image_p);
        free(file_name_p);
        image_file_free(image_file_p);
        image_set(image_p);
    }
#endif
#ifdef OLI_FIG_2
    test_pattern_squares(image_p, 1);

    for(uint32_t frame = 0; frame < 2000; ++frame)
    {
        interface_state_restore();
        printf("Image %u\n", frame);
        
        figure_t figure = figure_from_image(image_p);
        for(uint32_t point=0; point<figure.amount;++point)
        {
           rand_delta_vector(&figure.sequence[point].vector,
                             8,
                             image_p->width,
                             image_p->height);
        }

        image_draw_figure(image_p, &figure);
        figure_free(&figure);
        image_scale(&image_p, 1.0f/8, SCALE_ALGORITHM_DUMB);
        image_scale(&image_p, 8, SCALE_ALGORITHM_DUMB);

        char* file_name_p = num_extension(file_name_prefix_p, frame);
        image_file_p = image_file_init(file_name_p, image_p);
        free(file_name_p);
        image_file_free(image_file_p);
    }
#endif

    free(file_name_prefix_p);
    interface_deinit();

    return EXIT_SUCCESS;
}
