/*&cam
 * main.c
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "interface.h"
#include "performance.h"
#include "presets.h"
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

    //interface_state_save();
    for(uint32_t preset = 0; preset < preset_get_total_count(); ++preset)
    {
        performance_t performance_preset
            = performance_init(preset_get_name(preset));
        performance_try_start(&performance_preset);
        preset_run(preset);
        performance_try_add(&performance_preset);
        performance_print(&performance_preset);
        performance_free(&performance_preset);
    }

    performance_try_add(&performance_total);
    performance_print(&performance_total);
    interface_deinit();
    performance_free(&performance_total);

    return EXIT_SUCCESS;
}
