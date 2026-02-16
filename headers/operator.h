/*
 * operator.h
 *
 *  Created on: 16 févr. 2026
 *      Author: moliver
 */

#ifndef HEADERS_OPERATOR_H_
#define HEADERS_OPERATOR_H_

#include "vector.h"

typedef struct operator_t operator_t;

operator_t* operator_init_lines(vector_t x, vector_t y, vector_t z);
operator_t* operator_init_columns(vector_t x, vector_t y, vector_t z);
void        operator_free(operator_t* operator_p);

const vector_axis_t* operator_data(const operator_t* operator_p);

void operator_print(const operator_t* operator_p);

/** t = t_m
 *
 */
void operator_transpose(operator_t* t_p, const operator_t* m_p);

#endif /* HEADERS_OPERATOR_H_ */
