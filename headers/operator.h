/*
 * operator.h
 *
 *  Created on: 16 févr. 2026
 *      Author: moliver
 */

#ifndef HEADERS_OPERATOR_H_
#define HEADERS_OPERATOR_H_

#include <stdint.h>
#include "vector.h"

typedef struct operator_t operator_t;

operator_t* operator_init_null();
operator_t* operator_init_id();
operator_t* operator_init_translation(vector_t t);
operator_t* operator_init_lines(vector_t x, vector_t y, vector_t z, vector_t t);
operator_t* operator_init_columns(vector_t x, vector_t y, vector_t z, vector_t t);
operator_t* operator_multiply(const operator_t* a_p, const operator_t* b_p);

void        operator_free(operator_t* operator_p);

const vector_axis_t* operator_data(const operator_t* operator_p);
vector_t operator_row_get(const operator_t* m_p, vector_axis_e row);
vector_t operator_column_get(const operator_t* m_p, vector_axis_e column);
void operator_column_set(operator_t* operator_p, vector_axis_e column, vector_t vector);

void operator_print(const operator_t* operator_p);
vector_t* operator_operation(const operator_t* m_p, const vector_t* vector_array_p, uint32_t vector_count);

/** t = t_m
 *
 */
void operator_transpose(operator_t* t_p, const operator_t* m_p);

#endif /* HEADERS_OPERATOR_H_ */
