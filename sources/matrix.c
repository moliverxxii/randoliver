/*
 * matrix.c
 *
 *  Created on: 8 févr. 2026
 *      Author: moliver
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "matrix.h"
#include "utility.h"

matrix_t*
matrix_init(uint32_t lines, uint32_t columns)
{
    matrix_data_t* matrix_data_p = malloc(lines*columns*sizeof(matrix_data_t));
    matrix_t m =
    {
            lines,
            columns,
            matrix_data_p
    };

    matrix_t* m_p = malloc(sizeof(matrix_t));
    *m_p = m;
    return m_p;
}

matrix_t*
matrix_copy(const matrix_t* a_p)
{
    matrix_t* m_p = matrix_init(a_p->lines, a_p->lines);
    memcpy(m_p->array, a_p->array, m_p->lines * m_p->columns * sizeof(matrix_data_t));
    return m_p;
}

void
matrix_free(matrix_t* matrix_p)
{
    free(matrix_p->array);
    free(matrix_p);
}

matrix_data_t
matrix_value_get(const matrix_t* matrix_p,
                 uint32_t line, uint32_t column)
{
    matrix_data_t value = 0;
    if(line < matrix_p->lines && column < matrix_p->columns)
    {
        value = matrix_p->array[line * matrix_p->columns + column];
    }
    return value;
}

void
matrix_set(matrix_t* matrix_p, const matrix_data_t* restrict data_array)
{
    memcpy(matrix_p->array, data_array, matrix_p->lines * matrix_p->columns * sizeof(matrix_data_t));
}

void
matrix_value_set(matrix_t* matrix_p,
                 uint32_t line, uint32_t column,
                 matrix_data_t value)
{
    if(line < matrix_p->lines && column < matrix_p->columns)
    {
        matrix_p->array[line * matrix_p->columns + column] = value;
    }

}
//m = a + b
matrix_t* matrix_add(const matrix_t* a_p, const matrix_t* b_p)
{
    matrix_t* m_p = NULL;
    if(a_p->columns == b_p->columns && a_p->lines == b_p->lines)
    {
        m_p = matrix_copy(a_p);
        for(uint32_t index = 0; index < m_p->lines*m_p->columns; ++index)
        {
            m_p->array[index] += b_p->array[index];
        }
    }
    return m_p;
}

//m = a - b
matrix_t* matrix_subtract(const matrix_t* a_p, const matrix_t* b_p)
{
    matrix_t* m_p = NULL;
    if(a_p->columns == b_p->columns && a_p->lines == b_p->lines)
    {
        m_p = matrix_copy(a_p);
        for(uint32_t index = 0; index < m_p->lines*m_p->columns; ++index)
        {
            m_p->array[index] -= b_p->array[index];
        }
    }
    return m_p;
}


//m = coefficient * a
matrix_t* matrix_scale(const matrix_t* a_p, matrix_data_t coefficient)
{
    matrix_t* m_p = NULL;
    m_p = matrix_copy(a_p);
    for(uint32_t index = 0; index < m_p->lines*m_p->columns; ++index)
    {
        m_p->array[index] *= coefficient;
    }
    return m_p;
}


//m = a x b
matrix_t* matrix_multiply(const matrix_t* a_p, const matrix_t* b_p)
{
    matrix_t* m_p = NULL;
    if(a_p->columns == b_p->lines)
    {
        m_p = matrix_init(a_p->lines, b_p->columns);
        for(uint32_t line = 0; line< m_p->lines; ++line)
        {

            for(uint32_t column = 0; column< m_p->columns; ++column)
            {
                matrix_data_t value = 0;
                for(uint32_t index = 0; index < a_p->columns; ++index)
                {
                    value += matrix_value_get(a_p, line, index)
                           * matrix_value_get(b_p, index, column);
                }
                matrix_value_set(m_p, line, column, value);
            }
        }
    }
    return m_p;
}

//m = t_a
matrix_t* matrix_transpose(const matrix_t* a_p)
{
    matrix_t* m_p = matrix_init(a_p->columns, a_p->lines);
    for(uint32_t line = 0; line< m_p->lines; ++line)
    {

        for(uint32_t column = 0; column< m_p->columns; ++column)
        {
            matrix_data_t value = matrix_value_get(a_p, column, line);
            matrix_value_set(m_p, line, column, value);
        }
    }
    return m_p;
}


matrix_t* matrix_slice_get(const matrix_t* a_p,
                           uint32_t line_start, uint32_t line_end, uint32_t line_step,
                           uint32_t column_start, uint32_t column_end, uint32_t column_step)
{
    OLI_UNUSED(a_p);
    OLI_UNUSED(line_start);
    OLI_UNUSED(line_end);
    OLI_UNUSED(line_step);
    OLI_UNUSED(column_start);
    OLI_UNUSED(column_end);
    OLI_UNUSED(column_step);
    return NULL;
}

//a[slice] = m
void matrix_slice_set(matrix_t* a_p, const matrix_t m_p,
                      uint32_t line_start, uint32_t line_end, uint32_t line_step,
                      uint32_t column_start, uint32_t column_end, uint32_t column_step)
{
    OLI_UNUSED(a_p);
    OLI_UNUSED(m_p);
    OLI_UNUSED(line_start);
    OLI_UNUSED(line_end);
    OLI_UNUSED(line_step);
    OLI_UNUSED(column_start);
    OLI_UNUSED(column_end);
    OLI_UNUSED(column_step);
}


//m = { {m_0, m_1}, {m_2, m_3} }
matrix_t* matrix_concatenate(const matrix_t* matrix_array)
{
    OLI_UNUSED(matrix_array);
    return NULL;
}


void matrix_print(const matrix_t* matrix_p)
{
    for(uint32_t line = 0; line < matrix_p->lines; ++line)
    {
        for(uint32_t column=0; column<matrix_p->columns; ++column)
        {
            printf("|%# 9.1e", matrix_value_get(matrix_p, line, column));
        }
        printf("|\n");
    }

}

