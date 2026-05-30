/*
 * presets.h
 *
 *  Created on: 30 mai 2026
 *      Author: moliver
 */

#ifndef HEADERS_PRESETS_H_
#define HEADERS_PRESETS_H_

#include <stdint.h>

typedef struct preset_t preset_t;

uint32_t preset_get_total_count();

const char* preset_get_name();
void        preset_run(uint32_t preset_index);

#endif /* HEADERS_PRESETS_H_ */
