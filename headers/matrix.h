/*
 * matrix.h
 *
 *  Created on: 8 févr. 2026
 *      Author: moliver
 */

#ifndef HEADERS_MATRIX_H_
#define HEADERS_MATRIX_H_

#include <stdint.h>

typedef float matrix_data_t;

typedef struct matrix_t
{
    uint32_t lines;
    uint32_t columns;
    matrix_data_t* array_p;
} matrix_t;

matrix_t* matrix_init(uint32_t lines, uint32_t columns);
//m_p = a_p
matrix_t* matrix_copy(const matrix_t* a_p);
void      matrix_free(matrix_t* matrix_p);
matrix_data_t matrix_value_get(const matrix_t* matrix_p,
                               uint32_t line, uint32_t column);

void matrix_set(matrix_t* matrix_p, const matrix_data_t* restrict data_array);

void          matrix_value_set(matrix_t* matrix_p,
                               uint32_t line, uint32_t column,
                               matrix_data_t value);
//m = a + b
matrix_t* matrix_add(const matrix_t* a_p, const matrix_t* b_p);

//m = a - b
matrix_t* matrix_subtract(const matrix_t* a_p, const matrix_t* b_p);

//m = coefficient * a
matrix_t* matrix_scale(const matrix_t* a_p, matrix_data_t coefficient);

//m = a x b
matrix_t* matrix_multiply(const matrix_t* a_p, const matrix_t* b_p);

//m = t_a
matrix_t* matrix_transpose(const matrix_t* a_p);

matrix_t* matrix_slice_get(const matrix_t* a_p,
                           uint32_t line_start, uint32_t line_end, uint32_t line_step,
                           uint32_t column_start, uint32_t column_end, uint32_t column_step);
//a[slice] = m
void matrix_slice_set(matrix_t* a_p, const matrix_t m_p,
                      uint32_t line_start, uint32_t line_end, uint32_t line_step,
                      uint32_t column_start, uint32_t column_end, uint32_t column_step);

//m = { {m_0, m_1}, {m_2, m_3} }
matrix_t* matrix_concatenate(const matrix_t* matrix_array);

void matrix_print(const matrix_t* matrix_p);

#endif /* HEADERS_MATRIX_H_ */
