/*
 * matrix.h
 *
 *  Created on: 8 févr. 2026
 *      Author: moliver
 */

#ifndef HEADERS_MATRIX_H_
#define HEADERS_MATRIX_H_

#include <stdint.h>
#include <stdlib.h>

typedef float matrix_data_t;

typedef struct matrix_t matrix_t;

matrix_t* matrix_init(uint32_t lines, uint32_t columns);
matrix_t* matrix_init_null();
//m_p = a_p
void     matrix_copy(matrix_t* m_p, const matrix_t* a_p);
void     matrix_free(matrix_t* m_p);

//nombre d'elements
uint32_t matrix_length(const matrix_t* m_p);
//octets
size_t   matrix_size(const matrix_t* m_p);
int      matrix_is_allocated(const matrix_t* m_p);
int      matrix_is_dimensions_equal(const matrix_t* a_p, const matrix_t* b_p);
const matrix_data_t* matrix_data(const matrix_t* m_p);

matrix_data_t matrix_value_get(const matrix_t* m_p,
                               uint32_t line, uint32_t column);

void matrix_set(matrix_t* m, const matrix_data_t* restrict data_array,
                uint32_t lines, uint32_t columns);

void matrix_value_set(matrix_t* m_p,
                      uint32_t line, uint32_t column,
                      matrix_data_t value);
//m = a + b
void matrix_add(matrix_t* m_p, const matrix_t* a_p, const matrix_t* b_p);

//m = a - b
void matrix_subtract(matrix_t* m_p, const matrix_t* a_p, const matrix_t* b_p);

//m = coefficient * a
void matrix_scale(matrix_t* m_p, const matrix_t* a_p, matrix_data_t coefficient);

//m = a x b
void matrix_multiply(matrix_t* m_p, const matrix_t* a_p, const matrix_t* b_p);

//m = t_a
void matrix_transpose(matrix_t* m_p, const matrix_t* a_p);

//m = a[slice]
void matrix_slice_get(matrix_t* m_p, const matrix_t* a_p,
                      uint32_t line_start, uint32_t line_end, uint32_t line_step,
                      uint32_t column_start, uint32_t column_end, uint32_t column_step);
//m[slice] = a
void matrix_slice_set(matrix_t* m_p, const matrix_t* a_p,
                      uint32_t line_start, uint32_t line_end, uint32_t line_step,
                      uint32_t column_start, uint32_t column_end, uint32_t column_step);

//m = { {m_0, m_1}, {m_2, m_3} }
void matrix_concatenate(matrix_t* m_p, const matrix_t* matrix_array);

void matrix_print(const matrix_t* m);

#endif /* HEADERS_MATRIX_H_ */
