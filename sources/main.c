/*
 * main.c
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "image_file.h"
#include "interface.h"
#include "performance.h"
#include "preset.h"
#include "preset_list.h"
#include "utility.h"

int
main(int argc, char* argv[])
{
    OLI_UNUSED(argc);
    OLI_UNUSED(argv);

    interface_init("OAM IMAGE SYNTHESIZER (2019-2026)");

    performance_t performance_total = performance_init("total");
    performance_try_start(&performance_total);

    //Initialisation des particules
    srand(time(NULL));

    preset_list_init();
    //interface_state_save();
    for(uint32_t preset = 0; preset < preset_get_total_count(); ++preset)
    {
        image_t* frame_p = image_init(320,180);

        performance_t performance_preset
          = performance_init(preset_get_name(preset));
        performance_try_start(&performance_preset);

        performance_t perf_init = performance_init("  init");
        performance_try_start(&perf_init);
        preset_model_init(preset);
        performance_try_add(&perf_init);

        uint32_t frame_count = preset_frame_count(preset);

        performance_t perf_frame  = performance_init("  frame:");
        performance_t perf_update = performance_init("  update");
        performance_t perf_render = performance_init("  render");
        performance_t perf_file   = performance_init("  file");

        for(uint32_t frame = 0; frame < frame_count; ++frame)
        {
            performance_try_start(&perf_frame);

            performance_try_start(&perf_update);
            preset_model_update(preset);
            performance_try_add(&perf_update);

            image_set(frame_p);
            performance_try_start(&perf_render);
            int exists_b = preset_frame_render(preset, frame_p);
            performance_try_add(&perf_render);


            performance_try_start(&perf_file);
            if(exists_b)
            {
                char* name_p = (char*) preset_get_name(preset);
                if(frame_count > 1)
                {
                    name_p = file_name_extension_number(name_p, frame);
                }
                image_file_write(name_p, frame_p, NULL);
                if(frame_count > 1)
                {
                    free(name_p);
                }
            }
            performance_try_add(&perf_file);

            performance_try_add(&perf_frame);
        }
        if(frame_count > 1)
        {
            const char* preset_name_p = preset_get_name(preset);

            const char* const input_suffix_p  = " %d.bmp";
            char* name_input_p = malloc((strlen(preset_name_p) + strlen(input_suffix_p) + 1) * sizeof(char));
            strcpy(name_input_p, preset_name_p);
            strcat(name_input_p, input_suffix_p);

            const char* const output_suffix_p = ".mov";
            char* name_output_p = malloc((strlen(preset_name_p) + strlen(output_suffix_p) + 1) * sizeof(char));
            strcpy(name_output_p, preset_name_p);
            strcat(name_output_p, output_suffix_p);

            char frame_count_text[11];
            sprintf(frame_count_text, "%u", frame_count);

            pid_t process = fork();
            if(0 == process)
            {
                int conversion_process = execlp("ffmpeg", "ffmpeg",
                                                "-y",
                                                "-v",       "warning",
                                                "-f",       "image2",
                                                "-r",       "10",
                                                "-i",       name_input_p,
                                                "-frames",  frame_count_text,
                                                "-pix_fmt", "yuv420p",
                                                name_output_p,
                                                NULL);
                if(conversion_process != 0)
                {
                    //pas de ffmpeg chez vous monsieur/madame!
                    free(name_input_p);
                    free(name_output_p);
                    exit(0);
                }

            }
            else
            {
                free(name_input_p);
                free(name_output_p);
            }
        }

        preset_model_free(preset);

        performance_try_add(&performance_preset);
        performance_print(&performance_preset);
        performance_print(&perf_init);
        performance_print(&perf_frame);
        performance_print(&perf_update);
        performance_print(&perf_render);
        performance_print(&perf_file);

        performance_free(&performance_preset);
        performance_free(&perf_init);
        performance_free(&perf_frame);
        performance_free(&perf_update);
        performance_free(&perf_render);
        performance_free(&perf_file);
        image_free(frame_p);
    }

    performance_try_add(&performance_total);
    performance_print(&performance_total);
    interface_deinit();
    performance_free(&performance_total);

    return EXIT_SUCCESS;
}
