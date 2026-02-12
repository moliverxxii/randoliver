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

typedef struct matrix_t
{
    uint32_t lines;
    uint32_t columns;
    matrix_data_t* array;
} matrix_t;

extern const matrix_t MATRIX_NULL;

matrix_t matrix_init(uint32_t lines, uint32_t columns);
//m_p = a_p
matrix_t matrix_copy(const matrix_t a);
void     matrix_free(matrix_t* matrix_p);
//nombre d'elements
uint32_t matrix_length(const matrix_t m);
//octets
size_t   matrix_size(const matrix_t m);
int      matrix_is_allocated(const matrix_t matrix);
int      matrix_is_dimensions_equal(const matrix_t a, const matrix_t b);

matrix_data_t matrix_value_get(const matrix_t matrix_p,
                               uint32_t line, uint32_t column);

void matrix_set(matrix_t m, const matrix_data_t* restrict data_array);

void matrix_value_set(const matrix_t m,
                      uint32_t line, uint32_t column,
                      matrix_data_t value);
//m = a + b
matrix_t matrix_add(const matrix_t a, const matrix_t b);

//m = a - b
matrix_t matrix_subtract(const matrix_t a, const matrix_t b);

//m = coefficient * a
matrix_t matrix_scale(const matrix_t a, matrix_data_t coefficient);

//m = a x b
matrix_t matrix_multiply(const matrix_t a, const matrix_t b);

//m = t_a
matrix_t matrix_transpose(const matrix_t a);

matrix_t matrix_slice_get(const matrix_t m,
                          uint32_t line_start, uint32_t line_end, uint32_t line_step,
                          uint32_t column_start, uint32_t column_end, uint32_t column_step);
//a[slice] = m
void matrix_slice_set(matrix_t a, const matrix_t m,
                      uint32_t line_start, uint32_t line_end, uint32_t line_step,
                      uint32_t column_start, uint32_t column_end, uint32_t column_step);

//m = { {m_0, m_1}, {m_2, m_3} }
matrix_t matrix_concatenate(const matrix_t* matrix_array);

void matrix_print(const matrix_t m);

#endif /* HEADERS_MATRIX_H_ */
