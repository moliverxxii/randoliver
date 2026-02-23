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
    image_set(image_p);
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
    int frame_count = 20;
    figure_t* figure_p = figure_init(point_count);
    #ifdef OLI_3D
    //3
    performance_t render_performance = performance_init("rendu");
    performance_t process_performance = performance_init("processus");
    performance_t frame_performance = performance_init("image");
    *point_vector(figure_point(figure_p, 0)) = vector_init( 1,  0,  0);
    *point_vector(figure_point(figure_p, 1)) = vector_init( 0,  1,  0);
    *point_vector(figure_point(figure_p, 2)) = vector_init(-1,  0,  0);
    *point_vector(figure_point(figure_p, 3)) = vector_init( 0, -1,  0);
    *point_vector(figure_point(figure_p, 4)) = vector_init( 0,  0,  1);
    *point_vector(figure_point(figure_p, 5)) = vector_init( 0,  0, -1);

    for(uint32_t point = 0; point < point_count; ++point)
    {
        *point_colour(figure_point(figure_p, point)) = WHITE;
    }

    vector_t centre_grave = figure_get_average_point(figure_p);
    vector_print(centre_grave);

    camera_t camera = camera_init(centre_grave.x, -10, 1, centre_grave.x, centre_grave.y, centre_grave.z, M_PI_2/2);

    figure_t* figure_bis_p = figure_copy(figure_p);
    for(int frame=0; frame<frame_count; ++frame)
    {
        edge_t* edge_array[] =
        {
            edge_init(point_vector(figure_point(figure_bis_p, 0)), point_vector(figure_point(figure_bis_p, 1)), RED),
            edge_init(point_vector(figure_point(figure_bis_p, 1)), point_vector(figure_point(figure_bis_p, 2)), RED),
            edge_init(point_vector(figure_point(figure_bis_p, 2)), point_vector(figure_point(figure_bis_p, 3)), RED),
            edge_init(point_vector(figure_point(figure_bis_p, 3)), point_vector(figure_point(figure_bis_p, 0)), RED),
            edge_init(point_vector(figure_point(figure_bis_p, 0)), point_vector(figure_point(figure_bis_p, 4)), BLUE),
            edge_init(point_vector(figure_point(figure_bis_p, 4)), point_vector(figure_point(figure_bis_p, 2)), BLUE),
            edge_init(point_vector(figure_point(figure_bis_p, 2)), point_vector(figure_point(figure_bis_p, 5)), BLUE),
            edge_init(point_vector(figure_point(figure_bis_p, 5)), point_vector(figure_point(figure_bis_p, 0)), BLUE),
            edge_init(point_vector(figure_point(figure_bis_p, 1)), point_vector(figure_point(figure_bis_p, 4)), GREEN),
            edge_init(point_vector(figure_point(figure_bis_p, 4)), point_vector(figure_point(figure_bis_p, 3)), GREEN),
            edge_init(point_vector(figure_point(figure_bis_p, 3)), point_vector(figure_point(figure_bis_p, 5)), GREEN),
            edge_init(point_vector(figure_point(figure_bis_p, 5)), point_vector(figure_point(figure_bis_p, 1)), GREEN),
        };

        triangle_t* solid[] =
        {
            triangle_init(point_vector(figure_point(figure_bis_p, 0)),
                          point_vector(figure_point(figure_bis_p, 1)),
                          point_vector(figure_point(figure_bis_p, 4)), GREEN),
            triangle_init(point_vector(figure_point(figure_bis_p, 2)),
                          point_vector(figure_point(figure_bis_p, 1)),
                          point_vector(figure_point(figure_bis_p, 4)), MAGENTA),
            triangle_init(point_vector(figure_point(figure_bis_p, 2)),
                          point_vector(figure_point(figure_bis_p, 3)),
                          point_vector(figure_point(figure_bis_p, 4)), BLUE),
            triangle_init(point_vector(figure_point(figure_bis_p, 0)),
                          point_vector(figure_point(figure_bis_p, 3)),
                          point_vector(figure_point(figure_bis_p, 4)), RED),
            triangle_init(point_vector(figure_point(figure_bis_p, 0)),
                          point_vector(figure_point(figure_bis_p, 1)),
                          point_vector(figure_point(figure_bis_p, 5)), BLUE),
            triangle_init(point_vector(figure_point(figure_bis_p, 2)),
                          point_vector(figure_point(figure_bis_p, 1)),
                          point_vector(figure_point(figure_bis_p, 5)), RED),
            triangle_init(point_vector(figure_point(figure_bis_p, 2)),
                          point_vector(figure_point(figure_bis_p, 3)),
                          point_vector(figure_point(figure_bis_p, 5)), GREEN),
            triangle_init(point_vector(figure_point(figure_bis_p, 0)),
                          point_vector(figure_point(figure_bis_p, 3)),
                          point_vector(figure_point(figure_bis_p, 5)), MAGENTA),
        };
        performance_try_start(&frame_performance);
        interface_state_restore();
        printf("Image %u\n", frame);
        performance_print(&render_performance);
        performance_print(&process_performance);
        performance_print(&frame_performance);

        performance_try_start(&render_performance);
        renderable_cache_clear();
        figure_render(figure_bis_p, image_p, &camera);
        for(uint32_t edge=0; edge<sizeof(edge_array)/sizeof(edge_array[0]); ++edge)
        {
            edge_render(edge_array[edge], image_p, &camera);
        }
        for(uint32_t triangle=0; triangle<sizeof(solid)/sizeof(solid[0]); ++triangle)
        {
            triangle_render(solid[triangle], image_p, &camera);
        }
        figure_free(figure_bis_p);
        figure_bis_p = NULL;
        performance_try_add(&render_performance);

        char* file_name_p = num_extension(file_name_prefix_p, frame);

        image_file_p = image_file_init(file_name_p, image_p);

        //OPERATION
        performance_try_start(&process_performance);
        figure_bis_p = figure_copy(figure_p);
        const vector_t rotation_axis = vector_add(VECTOR_X, VECTOR_Z);
        for(uint32_t point = 0; point < figure_length(figure_bis_p); point++)
        {
             vector_rotate_axial(point_vector(figure_point(figure_bis_p, point)),
                                 VECTOR_0,
                                 rotation_axis,
                                 (float) frame * 2.*M_PI/frame_count);
        }
        performance_try_add(&process_performance);

        free(file_name_p);
        image_file_free(image_file_p);
        image_file_p = NULL;
        image_set(image_p);
        performance_try_add(&frame_performance);
    }
    figure_free(figure_bis_p);
    performance_free(&render_performance);
    performance_free(&process_performance);
    performance_free(&frame_performance);
#endif /* OLI_3D */

#ifdef OLI_FIG
    for(uint32_t point_n=0; point_n<figure_p.amount; ++point_n)
    {
        *point_vector(figure_p.array[point_n]) = vector_init(image_width(image_p)/2, image_height(image_p)/2, 0);
        *point_colour(figure_p.array[point_n]) = colour_get_random();
    }
    

    for(int frame=0; frame<frame_count; ++frame)
    {
        interface_state_restore();
        printf("Image %u\n", frame);
        for(uint32_t point_n=0; point_n<figure_p.amount;++point_n)
        {
           vector_random_delta(point_vector(figure_p.array[point_n]),
                             point_n,
                             image_width(image_p),
                             image_height(image_p));
        }
        char* file_name_p = num_extension(file_name_prefix_p, frame);

        image_file_p = image_file_init(file_name_p, image_p);

        figure_draw(&figure_p, image_p);
        image_file_write(image_file_p, image_p);
        free(file_name_p);
        image_file_free(image_file_p);
        image_set(image_p);
    }
#endif
    figure_free(figure_p);

#ifdef OLI_FIG_2
    test_pattern_squares(image_p, 1);

    for(int frame = 0; frame < frame_count; ++frame)
    {
        interface_state_restore();
        printf("Image %u\n", frame);
        
        figure_t figure_p = figure_from_image(image_p);
        for(uint32_t point=0; point<figure_p.amount;++point)
        {
           vector_random_delta(point_vector(figure_p.array[point]),
                             8,
                             image_width(image_p),
                             image_height(image_p));
        }

        figure_draw(&figure_p, image_p);
        figure_free(&figure_p);
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
