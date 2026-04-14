/*&cam
 * main.c
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "camera.h"
#include "engine.h"
#include "figure.h"
#include "image.h"
#include "image_drawing.h"
#include "image_file.h"
#include "interface.h"
#include "performance.h"
#include "solid.h"
#include "solid_file.h"
#include "triangle.h"
#include "utility.h"

//#define OLI_BROWN
//#define OLI_TEST_PATTERN
#define OLI_TEST_PATTERN_SCAN

//#define OLI_3D
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
    int width = 320;
    int height = 240;

    float scale = 1.0f;
    //Initialisation de l'image.
    image_t* image_p  = image_init(width, height);
    image_set(image_p);
    image_scale(image_p, scale, SCALE_ALGORITHM_LINEAR);
    //Initialisation des particules
    srand(time(NULL));

    //interface_state_save();


    uint32_t point_count = 6;

#ifdef OLI_BROWN
    brownien1(image_p, point_count, 1, width/2, height/2);
    image_file_write(file_name_prefix_p, image_p);
#endif /* OLI_BROWN */


#ifdef OLI_TEST_PATTERN
    image_scale(image_p, 1.0f/height, SCALE_ALGORITHM_LINEAR);
    test_pattern_squares(image_p, 1);
    image_scale(image_p, 960, SCALE_ALGORITHM_LINEAR);
    image_scale(image_p, 10.0f/960, SCALE_ALGORITHM_LINEAR);
    image_scale(image_p, 96, SCALE_ALGORITHM_LINEAR);
    image_file_write(file_name_prefix_p, image_p);
#endif //OLI_TEST_PATTERN

#ifdef OLI_TEST_PATTERN_SCAN
    test_pattern_scan(image_p);
    image_reduce_bit_depth(image_p, 5, 1);
    image_file_write("oli test pattern scan", image_p);

    test_pattern_scan(image_p);
    image_reduce_bit_depth(image_p, 5, 0);
    image_file_write("oli test pattern scan2", image_p);
#endif //OLI_TEST_PATTERN_SCAN

    //Animation
    int frame_count = 1;
    #ifdef OLI_3D
    //3
    performance_t render_performance = performance_init("rendu");
    performance_t process_performance = performance_init("processus");
    performance_t frame_performance = performance_init("image");

    solid_init_const();
    solid_t* solid_p = solid_file_open("scene.txt");


    camera_t camera = camera_init(-5, -1.5, -1,
                                  0, 0, 0,
                                  45);

    for(int frame=0; frame<frame_count; ++frame)
    {
        performance_try_start(&frame_performance);
        //interface_state_restore();
        printf("Image %u\n", frame);
        performance_print(&render_performance);
        performance_print(&process_performance);
        performance_print(&frame_performance);

        performance_try_start(&render_performance);
        renderable_cache_clear();

        solid_render(solid_p, image_p, &camera);
        performance_try_add(&render_performance);

        image_scale(image_p, 1/scale, SCALE_ALGORITHM_LINEAR);
        image_reduce_bit_depth(image_p, 5, 1);

        char* file_name_p = file_name_extension_number(file_name_prefix_p, frame);
        image_file_write(file_name_p, image_p);

        image_scale(image_p, scale, SCALE_ALGORITHM_LINEAR);
        free(file_name_p);
        image_set(image_p);

        //OPERATION
        performance_try_start(&process_performance);
        const vector_t rotation_axis = vector_add(VECTOR_X, VECTOR_Z);
//        for(uint32_t point = 0; point < solid_length(figure_bis_p); point++)
//        {
//            float angle = (float) frame * 2.*M_PI/frame_count;
//            vector_rotate_axial(point_vector(figure_point(figure_bis_p, point)),
//                                VECTOR_0,
//                                rotation_axis,
//                                angle);
//        }
        performance_try_add(&process_performance);

        performance_try_add(&frame_performance);
    }
    image_free(image_p);
    solid_free(solid_p);
    performance_free(&render_performance);
    performance_free(&process_performance);
    performance_free(&frame_performance);
#endif /* OLI_3D */

#ifdef OLI_FIG
    figure_t* figure_p = figure_init(point_count);
    for(uint32_t point = 0; point < figure_length(figure_p); ++point)
    {
        *point_vector(figure_point(figure_p, point))
                = vector_init(image_width(image_p)/2,
                              image_height(image_p)/2, 0);
        *point_colour(figure_point(figure_p, point))
                = colour_get_random();
    }
    

    for(int frame = 0; frame < frame_count; ++frame)
    {
        interface_state_restore();
        printf("Image %u\n", frame);
        for(uint32_t point = 0; point < figure_length(figure_p); ++point)
        {
           vector_random_delta(point_vector(figure_point(figure_p, point)),
                               point,
                               image_width(image_p),
                               image_height(image_p));
        }
        figure_draw(figure_p, image_p);

        char* file_name_p = num_extension(file_name_prefix_p, frame);
        image_file_write(file_name_p, image_p);
        free(file_name_p);

        image_set(image_p);
    }
#endif

#ifdef OLI_FIG_2
    test_pattern_squares(image_p, 1);

    for(int frame = 0; frame < frame_count; ++frame)
    {
        interface_state_restore();
        printf("Image %u\n", frame);
        
        figure_t* figure_p = figure_from_image(image_p);
        for(uint32_t point = 0; point<figure_length(figure_p); ++point)
        {
           vector_random_delta(point_vector(figure_point(figure_p, point)),
                             8,
                             image_width(image_p),
                             image_height(image_p));
        }

        figure_draw(figure_p, image_p);
        figure_free(figure_p);
        image_scale(image_p, 1.0f/8, SCALE_ALGORITHM_DUMB);
        image_scale(image_p, 8, SCALE_ALGORITHM_DUMB);

        char* file_name_p = num_extension(file_name_prefix_p, frame);
        image_file_write(file_name_p, image_p);
        free(file_name_p);
    }
#endif

    free(file_name_prefix_p);
    interface_deinit();

    return EXIT_SUCCESS;
}
