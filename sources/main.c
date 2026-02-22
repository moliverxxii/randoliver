/*&cam
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
#include "performance.h"
#include "edge.h"
#include "figure.h"
#include "triangle.h"

//#define OLI_BROWN
//#define OLI_TEST_PATTERN

#define OLI_3D
//#define OLI_FIG
//#define OLI_FIG_2

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
    int width = 1280;
    int height = 720;

    image_file_t* image_file_p;

    //Initialisation de l'image.
    image_t* image_p  = image_init(width, height);

    //Initialisation des particules
    srand(time(NULL));

    interface_state_save();


    uint32_t point_count = 6;

#ifdef OLI_BROWN
    brownien1(image_p, point_count, 1, width/2, height/2);
    image_file_p = image_file_init(file_name_prefix_p, image_p);

    image_file_write(image_file_p, image_p);
    image_file_free(image_file_p);
    image_set(image_p);
#endif /* OLI_BROWN */


#ifdef OLI_TEST_PATTERN
    image_scale(&image_p, 1.0f/height, SCALE_ALGORITHM_LINEAR);
    test_pattern_squares(image_p, 1);
    image_scale(&image_p, 960, SCALE_ALGORITHM_LINEAR);
    image_scale(&image_p, 10.0f/960, SCALE_ALGORITHM_LINEAR);
    image_scale(&image_p, 96, SCALE_ALGORITHM_LINEAR);
    image_file_p = image_file_init(file_name_prefix_p, image_p);
    image_file_free(image_file_p);
#endif //OLI_TEST_PATTERN

//Animation
    int frame_count = 360;
    figure_t figure = figure_init(point_count);
    performance_t render_performance = performance_init("rendu");
    performance_t process_performance = performance_init("processus");
    performance_t frame_performance = performance_init("image");
    #ifdef OLI_3D
    //3
    figure.array[0].vector = vector_init( 1,  0,  0);
    figure.array[1].vector = vector_init( 0,  1,  0);
    figure.array[2].vector = vector_init(-1,  0,  0);
    figure.array[3].vector = vector_init( 0, -1,  0);
    figure.array[4].vector = vector_init( 0,  0,  1);
    figure.array[5].vector = vector_init( 0,  0, -1);

    for(uint32_t point = 0; point < point_count; ++point)
    {
        figure.array[point].colour = WHITE;
    }

    vector_t centre_grave = figure_get_average_point(&figure);
    vector_print(centre_grave);

    camera_t camera = camera_init(centre_grave.x, -10, 1, centre_grave.x, centre_grave.y, centre_grave.z, M_PI_2/2);

    figure_t figure_bis = figure_copy(figure);
    for(int frame=0; frame<frame_count; ++frame)
    {
        edge_t edge_array[] =
        {
            edge_init(&figure_bis.array[0].vector, &figure_bis.array[1].vector, RED),
            edge_init(&figure_bis.array[1].vector, &figure_bis.array[2].vector, RED),
            edge_init(&figure_bis.array[2].vector, &figure_bis.array[3].vector, RED),
            edge_init(&figure_bis.array[3].vector, &figure_bis.array[0].vector, RED),

            edge_init(&figure_bis.array[0].vector, &figure_bis.array[4].vector, BLUE),
            edge_init(&figure_bis.array[4].vector, &figure_bis.array[2].vector, BLUE),
            edge_init(&figure_bis.array[2].vector, &figure_bis.array[5].vector, BLUE),
            edge_init(&figure_bis.array[5].vector, &figure_bis.array[0].vector, BLUE),

            edge_init(&figure_bis.array[1].vector, &figure_bis.array[4].vector, GREEN),
            edge_init(&figure_bis.array[4].vector, &figure_bis.array[3].vector, GREEN),
            edge_init(&figure_bis.array[3].vector, &figure_bis.array[5].vector, GREEN),
            edge_init(&figure_bis.array[5].vector, &figure_bis.array[1].vector, GREEN),
        };

        triangle_t solid[] =
        {
            triangle_init(&figure_bis.array[0].vector, &figure_bis.array[1].vector, &figure_bis.array[4].vector, GREEN),
            triangle_init(&figure_bis.array[2].vector, &figure_bis.array[1].vector, &figure_bis.array[4].vector, MAGENTA),
            triangle_init(&figure_bis.array[2].vector, &figure_bis.array[3].vector, &figure_bis.array[4].vector, BLUE),
            triangle_init(&figure_bis.array[0].vector, &figure_bis.array[3].vector, &figure_bis.array[4].vector, RED),

            triangle_init(&figure_bis.array[0].vector, &figure_bis.array[1].vector, &figure_bis.array[5].vector, BLUE),
            triangle_init(&figure_bis.array[2].vector, &figure_bis.array[1].vector, &figure_bis.array[5].vector, RED),
            triangle_init(&figure_bis.array[2].vector, &figure_bis.array[3].vector, &figure_bis.array[5].vector, GREEN),
            triangle_init(&figure_bis.array[0].vector, &figure_bis.array[3].vector, &figure_bis.array[5].vector, MAGENTA),
        };
        performance_try_start(&frame_performance);
        interface_state_restore();
        printf("Image %u\n", frame);
        performance_print(&render_performance);
        performance_print(&process_performance);
        performance_print(&frame_performance);

        performance_try_start(&render_performance);
        camera_cache_clear();
//        camera_render_figure(&camera, image_p, figure_bis);
        for(uint32_t edge=0; edge<sizeof(edge_array)/sizeof(edge_array[0]); ++edge)
        {
//            camera_render_edge(&camera, image_p, edge_array[edge]);
        }
        for(uint32_t triangle=0; triangle<sizeof(solid)/sizeof(solid[0]); ++triangle)
        {
            camera_render_triangle(&camera, image_p, solid[triangle]);
        }
        figure_free(&figure_bis);
        performance_try_add(&render_performance);

        char* file_name_p = num_extension(file_name_prefix_p, frame);

        image_file_p = image_file_init(file_name_p, image_p);

        //OPERATION
        performance_try_start(&process_performance);
        figure_bis = figure_copy(figure);
        const vector_t rotation_axis = vector_add(VECTOR_X, VECTOR_Z);
        for(uint32_t point_n = 0; point_n< figure.amount; point_n++)
        {
//            figure_bis.array[point_n].vector =
//            vector_rotate(figure.array[point_n].vector,
//                          rotation_axis,
//                          (float) frame * 2. * M_PI/frame_count);

             vector_rotate_axial(&figure_bis.array[point_n].vector,
                                 VECTOR_0,
                                 rotation_axis,
                                 (float) frame * 2.*M_PI/frame_count);
        }
//        break;
        performance_try_add(&process_performance);

        free(file_name_p);
        image_file_free(image_file_p);
        image_file_p = NULL;
        image_set(image_p);
        performance_try_add(&frame_performance);
    }
#endif /* OLI_3D */

#ifdef OLI_FIG
    for(uint32_t point_n=0; point_n<figure.amount; ++point_n)
    {
        figure.array[point_n].vector.x = image_width(image_p)/2;
        figure.array[point_n].vector.y = image_height(image_p)/2;
        figure.array[point_n].colour = colour_get_random();
    }
    

    for(int frame=0; frame<frame_count; ++frame)
    {
        interface_state_restore();
        printf("Image %u\n", frame);
        for(uint32_t point_n=0; point_n<figure.amount;++point_n)
        {
           vector_random_delta(&figure.array[point_n].vector,
                             point_n,
                             image_width(image_p),
                             image_height(image_p));
        }
        char* file_name_p = num_extension(file_name_prefix_p, frame);

        image_file_p = image_file_init(file_name_p, image_p);

        image_draw_figure(image_p, &figure);
        image_file_write(image_file_p, image_p);
        free(file_name_p);
        image_file_free(image_file_p);
        image_set(image_p);
    }
#endif
    figure_free(&figure);

#ifdef OLI_FIG_2
    test_pattern_squares(image_p, 1);

    for(int frame = 0; frame < frame_count; ++frame)
    {
        interface_state_restore();
        printf("Image %u\n", frame);
        
        figure_t figure = figure_from_image(image_p);
        for(uint32_t point=0; point<figure.amount;++point)
        {
           vector_random_delta(&figure.array[point].vector,
                             8,
                             image_width(image_p),
                             image_height(image_p));
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
