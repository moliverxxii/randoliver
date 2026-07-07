/*
 * presets.h
 *
 *  Created on: 30 mai 2026
 *      Author: moliver
 */

#ifndef HEADERS_PRESET_H_
#define HEADERS_PRESET_H_

#include <stdint.h>

#include "image.h"

typedef struct preset_t preset_t;

typedef void (*model_init_f)();
typedef void (*model_update_f)();
typedef void (*image_render_f)(image_t* image_p, uint32_t frame);
typedef void (*model_free_f)();

int preset_add(const char* preset_p,
               model_init_f init_f,
               model_update_f update_f,
               image_render_f render_f,
               model_free_f free_f,
               uint32_t frame_count);

uint32_t preset_get_total_count();

const char* preset_get_name(uint32_t preset_index);
uint32_t    preset_frame_count(uint32_t preset_index);
void        preset_model_init(uint32_t preset_index);
void        preset_model_free(uint32_t preset_index);
void        preset_model_update(uint32_t preset_index);
int         preset_frame_render(uint32_t preset_index, image_t* image_p);

#endif /* HEADERS_PRESET_H_ */
