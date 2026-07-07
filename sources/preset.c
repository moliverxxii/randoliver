/*
 * presets.c
 *
 *  Created on: 30 mai 2026
 *      Author: moliver
 */

#include "../headers/preset.h"

#include <stdlib.h>
#include <string.h>

#include "image.h"

static const char* const PRESET_NAME_NONE = "Not a preset";

typedef struct preset_t
{
    const char* name_p;
    model_init_f   init_f;
    model_update_f update_f;
    image_render_f render_f;
    model_free_f   free_f;
    uint32_t frame_count;
} preset_t;

static uint32_t PRESET_LIST_LENGTH = 0;
static preset_t* PRESET_LIST = NULL;

int
preset_add(const char* preset_p,
           model_init_f init_f,
           model_update_f update_f,
           image_render_f render_f,
           model_free_f free_f,
           uint32_t frame_count)
{
    uint32_t new_count = PRESET_LIST_LENGTH + 1;
    preset_t* list_p = realloc(PRESET_LIST, new_count * sizeof(preset_t));
    char* name_p = malloc(strlen(preset_p)+1);
    strcpy(name_p, preset_p);
    list_p[new_count-1] = (preset_t) {name_p, init_f, update_f, render_f, free_f, frame_count};
    PRESET_LIST = list_p;
    PRESET_LIST_LENGTH = new_count;
    return 0;
}

void
preset_clear()
{
    free(PRESET_LIST);
    PRESET_LIST_LENGTH = 0;
}

uint32_t
preset_get_total_count()
{
    return PRESET_LIST_LENGTH;
}

const char*
preset_get_name(uint32_t preset_index)
{
    if(preset_index > preset_get_total_count())
    {
        return PRESET_NAME_NONE;
    }
    else
    {
        return PRESET_LIST[preset_index].name_p;
    }

}

uint32_t
preset_frame_count(uint32_t preset_index)
{
    uint32_t frame_count = 0;
    if(preset_index < preset_get_total_count())
    {
        frame_count = PRESET_LIST[preset_index].frame_count;
    }
    return frame_count;
}


void
preset_model_init(uint32_t preset_index)
{
    if(preset_index < preset_get_total_count())
    {
        if(PRESET_LIST[preset_index].init_f != NULL)
        {
            (*PRESET_LIST[preset_index].init_f)();
        }
    }
}

void
preset_model_free(uint32_t preset_index)
{
    if(preset_index < preset_get_total_count())
    {
        if(PRESET_LIST[preset_index].free_f != NULL)
        {
            (*PRESET_LIST[preset_index].free_f)();
        }
    }

}

void
preset_model_update(uint32_t preset_index)
{
    if(preset_index < preset_get_total_count())
    {
        if(PRESET_LIST[preset_index].update_f != NULL)
        {
            (*PRESET_LIST[preset_index].update_f)();
        }
    }

}

int
preset_frame_render(uint32_t preset_index, image_t* image_p)
{
    int exists = 0;
    if(preset_index < preset_get_total_count())
    {
        if(PRESET_LIST[preset_index].render_f != NULL)
        {
            (*PRESET_LIST[preset_index].render_f)(image_p, 0);
            exists = 1;
        }
    }
    return exists;

}


