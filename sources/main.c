/*&cam
 * main.c
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

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
    image_t* frame_p = image_init(320,240);
    for(uint32_t preset = 0; preset < preset_get_total_count(); ++preset)
    {
        //TODO utiliser la nouvelle architecture de preset
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
            preset_frame_render(preset, frame_p);
            performance_try_add(&perf_render);

            performance_try_start(&perf_file);
            image_file_write(preset_get_name(preset), frame_p, NULL);
            performance_try_add(&perf_file);

            performance_try_add(&perf_frame);
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
    }
    image_free(frame_p);

    performance_try_add(&performance_total);
    performance_print(&performance_total);
    interface_deinit();
    performance_free(&performance_total);

    return EXIT_SUCCESS;
}
