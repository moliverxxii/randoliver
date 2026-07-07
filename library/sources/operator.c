/*
 * operator.c
 *
 *  Created on: 16 févr. 2026
 *      Author: moliver
 */
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "operator.h"

typedef vector_axis_t matrix_4x4_t [VECTOR_AXIS_COUNT][VECTOR_AXIS_COUNT];
typedef vector_axis_t matrix_4x1_t [VECTOR_AXIS_COUNT];

typedef struct operator_t
{
    matrix_4x4_t matrix;
} operator_t;

operator_t*
operator_init_null()
{
    return operator_init_lines(VECTOR_0, VECTOR_0, VECTOR_0, VECTOR_0);
}

operator_t*
operator_init_id()
{
    return operator_init_lines(VECTOR_X, VECTOR_Y, VECTOR_Z, VECTOR_0);
}

operator_t*
operator_init_translation(vector_t t)
{
    return operator_init_lines(VECTOR_X, VECTOR_Y, VECTOR_Z, t);
}

operator_t*
operator_init_lines(vector_t x, vector_t y, vector_t z, vector_t t)
{
    operator_t operator =
    {
        {
            {x.x, x.y, x.z, t.x},
            {y.x, y.y, y.z, t.y},
            {z.x, z.y, z.z, t.z},
            {  0,   0,   0,   1}
        }
    };
    operator_t* operator_p = malloc(sizeof(operator));
    if(operator_p != NULL)
    {
        *operator_p = operator;
    }
    return operator_p;
}

operator_t*
operator_init_columns(vector_t x, vector_t y, vector_t z, vector_t t)
{
    operator_t operator =
    {
        {
            {x.x, y.x, z.x, t.x},
            {x.y, y.y, z.y, t.y},
            {x.z, y.z, z.z, t.z},
            {  0,   0,   0,   1}
        }
    };

    operator_t* operator_p = malloc(sizeof(operator));
    if(operator_p != NULL)
    {
        *operator_p = operator;
    }
    return operator_p;
}


operator_t*
operator_multiply(const operator_t* a_p, const operator_t* b_p)
{
    operator_t* operator_p = operator_init_null();
    for(vector_axis_e column = 0; column < VECTOR_AXIS_COUNT; ++column)
    {
        vector_t vector_column = operator_column_get(b_p, column);
        vector_t* result_p = operator_operation(a_p, &vector_column, 1);
        operator_column_set(operator_p, column, *result_p);
        free(result_p);
    }
    return operator_p;
}

void
operator_free(operator_t* operator_p)
{
    free(operator_p);
}

const vector_axis_t*
operator_data(const operator_t* operator_p)
{
    return &operator_p->matrix[0][0];
}

vector_t
operator_row_get(const operator_t* m_p, vector_axis_e row)
{
    vector_t vector_row = VECTOR_0;
    if(VECTOR_AXIS_X <= row && row < VECTOR_AXIS_COUNT)
    {
        memcpy(vector_row.array, m_p->matrix[row], sizeof(vector_row));
    }
    return vector_row;
}

vector_t
operator_column_get(const operator_t* m_p, vector_axis_e column)
{
    vector_t vector_column = VECTOR_0;
    if(VECTOR_AXIS_X <= column && column < VECTOR_AXIS_COUNT)
    {
        for(vector_axis_e row = VECTOR_AXIS_X; row < VECTOR_AXIS_COUNT; ++row)
        {
            vector_column.array[row] = m_p->matrix[row][column];
        }
    }
    return vector_column;
}

void
operator_column_set(operator_t* operator_p, vector_axis_e column, vector_t vector)
{
    for(vector_axis_e row = VECTOR_AXIS_X; row < VECTOR_AXIS_COUNT; ++row)
    {
        operator_p->matrix[row][column] = vector.array[row];
    }
}

void
operator_print(const operator_t* operator_p)
{
    for(int row = 0; row < VECTOR_AXIS_COUNT; ++row)
    {
        for(int column = 0; column < VECTOR_AXIS_COUNT; ++column)
        {
            printf("% 2.3f |", operator_p->matrix[row][column]);
        }
        printf("\n");
    }
    printf("\n");
}

vector_t*
operator_operation(const operator_t* m_p, const vector_t* vector_array_p, uint32_t vector_count)
{
    vector_t* result_array_p = malloc(vector_count * sizeof(vector_t));
    for(uint32_t vector = 0; vector < vector_count; ++vector)
    {
        for(vector_axis_e row = VECTOR_AXIS_X; row < VECTOR_AXIS_COUNT; ++row)
        {
            result_array_p[vector].array[row] = vector_scalar_full(operator_row_get(m_p, row), vector_array_p[vector]);
        }
    }
    return result_array_p;
}


void
operator_transpose(operator_t* t_p, const operator_t* m_p)
{
    for(int column = 0; column <= VECTOR_AXIS_Z; ++column)
    {
        operator_column_set(t_p, column, operator_row_get(m_p, column));
    }
}

