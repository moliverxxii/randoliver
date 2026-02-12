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

const matrix_t MATRIX_NULL =
{
        0,
        0,
        NULL
};

matrix_t
matrix_init(uint32_t lines, uint32_t columns)
{
    matrix_data_t* matrix_data_p = malloc(lines*columns*sizeof(matrix_data_t));
    matrix_t m =
    {
            lines,
            columns,
            matrix_data_p
    };

    return m;
}

matrix_t
matrix_copy(const matrix_t a)
{
    matrix_t m = matrix_init(a.lines, a.columns);
    memcpy(m.array, a.array, matrix_size(m));
    return m;
}

uint32_t matrix_length(const matrix_t m)
{
    return m.lines * m.columns;
}

size_t matrix_size(const matrix_t m)
{
    return matrix_length(m) * sizeof(matrix_data_t);
}

int
matrix_is_allocated(const matrix_t m)
{
    return m.array != NULL;
}

int
matrix_is_dimensions_equal(const matrix_t a, const matrix_t b)
{
    return a.columns == b.columns && a.lines == b.lines;
}


void
matrix_free(matrix_t* matrix_p)
{
    free(matrix_p->array);
    matrix_p->array = NULL;
    matrix_p->lines = 0;
    matrix_p->columns = 0;
}

matrix_data_t
matrix_value_get(const matrix_t m,
                 uint32_t line, uint32_t column)
{
    matrix_data_t value = 0;
    if(line < m.lines && column < m.columns)
    {
        value = m.array[line * m.columns + column];
    }
    return value;
}

void
matrix_set(matrix_t m, const matrix_data_t* restrict data_array)
{
    memcpy(m.array, data_array, matrix_size(m));
}

void
matrix_value_set(matrix_t m,
                 uint32_t line, uint32_t column,
                 matrix_data_t value)
{
    if(line < m.lines && column < m.columns)
    {
        m.array[line * m.columns + column] = value;
    }

}
//m = a + b
matrix_t
matrix_add(const matrix_t a, const matrix_t b)
{
    matrix_t m = matrix_copy(a);
    if(matrix_is_dimensions_equal(a, b))
    {
        for(uint32_t index = 0; index < matrix_length(m); ++index)
        {
            m.array[index] += b.array[index];
        }
    }
    return m;
}

//m = a - b
matrix_t
matrix_subtract(const matrix_t a, const matrix_t b)
{
    matrix_t b_m1 = matrix_scale(b, -1);
    matrix_t m = matrix_add(a, b_m1);
    matrix_free(&b_m1);
    return m;
}


//m = coefficient * a
matrix_t
matrix_scale(const matrix_t a, matrix_data_t coefficient)
{
    matrix_t m = matrix_copy(a);
    for(uint32_t index = 0; index < matrix_length(m); ++index)
    {
        m.array[index] *= coefficient;
    }
    return m;
}


//m = a x b
matrix_t matrix_multiply(const matrix_t a, const matrix_t b)
{
    matrix_t m = MATRIX_NULL;
    if(a.columns == b.lines)
    {
        m = matrix_init(a.lines, b.columns);
        for(uint32_t line = 0; line< m.lines; ++line)
        {
            for(uint32_t column = 0; column< m.columns; ++column)
            {
                matrix_data_t value = 0;
                for(uint32_t index = 0; index < a.columns; ++index)
                {
                    value += matrix_value_get(a, line, index)
                           * matrix_value_get(b, index, column);
                }
                matrix_value_set(m, line, column, value);
            }
        }
    }
    return m;
}

//m = t_a
matrix_t
matrix_transpose(const matrix_t a)
{
    matrix_t m = matrix_init(a.columns, a.lines);
    for(uint32_t line = 0; line< m.lines; ++line)
    {
        for(uint32_t column = 0; column< m.columns; ++column)
        {
            matrix_data_t value = matrix_value_get(a, column, line);
            matrix_value_set(m, line, column, value);
        }
    }
    return m;
}


matrix_t
matrix_slice_get(const matrix_t m,
                 uint32_t line_start, uint32_t line_end, uint32_t line_step,
                 uint32_t column_start, uint32_t column_end, uint32_t column_step)
{
    OLI_UNUSED(m);
    OLI_UNUSED(line_start);
    OLI_UNUSED(line_end);
    OLI_UNUSED(line_step);
    OLI_UNUSED(column_start);
    OLI_UNUSED(column_end);
    OLI_UNUSED(column_step);
    return MATRIX_NULL;
}

//a[slice] = m
void
matrix_slice_set(matrix_t m, const matrix_t m_p,
                 uint32_t line_start, uint32_t line_end, uint32_t line_step,
                 uint32_t column_start, uint32_t column_end, uint32_t column_step)
{
    OLI_UNUSED(m);
    OLI_UNUSED(m_p);
    OLI_UNUSED(line_start);
    OLI_UNUSED(line_end);
    OLI_UNUSED(line_step);
    OLI_UNUSED(column_start);
    OLI_UNUSED(column_end);
    OLI_UNUSED(column_step);
}


//m = { {m_0, m_1}, {m_2, m_3} }
matrix_t
matrix_concatenate(const matrix_t* matrix_array)
{
    OLI_UNUSED(matrix_array);
    return MATRIX_NULL;
}


void matrix_print(const matrix_t m)
{
    for(uint32_t line = 0; line < m.lines; ++line)
    {
        for(uint32_t column=0; column<m.columns; ++column)
        {
            printf("|%# 9.1e", matrix_value_get(m, line, column));
        }
        printf("|\n");
    }

}

