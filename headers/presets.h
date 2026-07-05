/*
 * presets.h
 *
 *  Created on: 30 mai 2026
 *      Author: moliver
 */

#ifndef HEADERS_PRESETS_H_
#define HEADERS_PRESETS_H_

#include <stdint.h>

#include "image.h"

typedef struct preset_t preset_t;

typedef void (*model_init_f)();
typedef void (*model_update_f)();
typedef void (*image_render_f)(image_t* image_p, uint32_t frame);

int preset_add(const char* preset_p,
               model_init_f init_f,
               model_update_f update_f,
               image_render_f render_f,
               uint32_t frame_count);

//TODO utiliser la nouvelle architecture de preset

uint32_t preset_get_total_count();

const char* preset_get_name(uint32_t preset_index);
void        preset_run(uint32_t preset_index);

#endif /* HEADERS_PRESETS_H_ */
