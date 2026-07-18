/*
 * bitarray.h
 *
 *  Created on: 12 juil. 2026
 *      Author: moliver
 */

#ifndef HEADERS_BITARRAY_H_
#define HEADERS_BITARRAY_H_

#include <stdint.h>

typedef struct bitarray_t bitarray_t;

bitarray_t* bitarray_init(uint32_t width, uint32_t height);
void bitarray_free(bitarray_t* bitarray_p);
void bitarray_extract(bitarray_t* destination_p, const bitarray_t* source_p,
                      int32_t position_x, int32_t position_y);
void bitarray_bit_set(bitarray_t* array_p, uint32_t x, uint32_t y);
void bitarray_bit_reset(bitarray_t* array_p, uint32_t x, uint32_t y);
int  bitarray_bit_get(const bitarray_t* array_p, uint32_t x, uint32_t y);
void bitarray_and(bitarray_t* a_p, const bitarray_t* b_p);
uint32_t bitarray_on_get(const bitarray_t* array_p);
void bitarray_print(const bitarray_t* bitarray_p);
#endif /* HEADERS_BITARRAY_H_ */
