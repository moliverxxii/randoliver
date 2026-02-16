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

typedef struct matrix_t
{
    uint32_t lines;
    uint32_t columns;
    matrix_data_t* array;
} matrix_t;

const matrix_t MATRIX_NULL =
{
        0,
        0,
        NULL
};

static void matrix_allocate(matrix_t* m_p);
static void matrix_deallocate(matrix_t* m_p);

matrix_t*
matrix_init(uint32_t lines, uint32_t columns)
{
    matrix_t m =
    {
            lines,
            columns,
            NULL
    };

    if(lines > 0 && columns > 0)
    {
        matrix_allocate(&m);
    }

    matrix_t* m_p = malloc(sizeof(matrix_t));

    if(m_p != NULL)
    {
        *m_p = m;
    }

    return m_p;
}

void
matrix_copy(matrix_t* m_p, const matrix_t* a_p)
{
    if(matrix_is_allocated(m_p))
    {
        matrix_deallocate(m_p);
    }

    *m_p = *a_p;
    m_p->array = NULL;

    matrix_allocate(m_p);

    memcpy(m_p->array, a_p->array, matrix_size(a_p));
}

void
matrix_free(matrix_t* m_p)
{
    if(matrix_is_allocated(m_p))
    {
        matrix_deallocate(m_p);
    }
    free(m_p);
}

uint32_t
matrix_length(const matrix_t* m_p)
{
    return m_p->lines * m_p->columns;
}

size_t
matrix_size(const matrix_t* m_p)
{
    return matrix_length(m_p) * sizeof(matrix_data_t);
}

int
matrix_is_allocated(const matrix_t* m_p)
{
    return m_p->array != NULL;
}

int
matrix_is_dimensions_equal(const matrix_t* a_p, const matrix_t* b_p)
{
    return a_p->columns == b_p->columns && a_p->lines == b_p->lines;
}

const matrix_data_t* matrix_data(const matrix_t* m_p)
{
    return m_p->array;
}

matrix_data_t
matrix_value_get(const matrix_t* m_p,
                 uint32_t line, uint32_t column)
{
    matrix_data_t value = 0;
    if(line < m_p->lines && column < m_p->columns)
    {
        value = m_p->array[line * m_p->columns + column];
    }
    return value;
}

void
matrix_set(matrix_t* m_p, const matrix_data_t* restrict data_array)
{
    memcpy(m_p->array, data_array, matrix_size(m_p));
}

void
matrix_value_set(matrix_t* m_p,
                 uint32_t line, uint32_t column,
                 matrix_data_t value)
{
    if(line < m_p->lines && column < m_p->columns)
    {
        m_p->array[line * m_p->columns + column] = value;
    }

}
//m = a + b
void
matrix_add(matrix_t* m_p,const matrix_t* a_p, const matrix_t* b_p)
{
    if(matrix_is_allocated(m_p))
    {
        matrix_deallocate(m_p);
    }

    if(matrix_is_dimensions_equal(a_p, b_p))
    {
        matrix_copy(m_p, a_p);
        for(uint32_t index = 0; index < matrix_length(m_p); ++index)
        {
            m_p->array[index] += b_p->array[index];
        }
    }
}

//m = a - b
void
matrix_subtract(matrix_t* m_p, const matrix_t* a_p, const matrix_t* b_p)
{
    matrix_t* b_m1_p = matrix_init(0, 0);

    matrix_scale(b_m1_p, b_p, -1);
    matrix_add(m_p, a_p, b_m1_p);
    matrix_free(b_m1_p);
}


//m = coefficient * a
void
matrix_scale(matrix_t* m_p, const matrix_t* a_p, matrix_data_t coefficient)
{
    matrix_copy(m_p, a_p);
    for(uint32_t index = 0; index < matrix_length(m_p); ++index)
    {
        m_p->array[index] *= coefficient;
    }
}


//m = a x b
void
matrix_multiply(matrix_t* m_p, const matrix_t* a_p, const matrix_t* b_p)
{
    if(matrix_is_allocated(m_p))
    {
        matrix_deallocate(m_p);
    }

    if(a_p->columns == b_p->lines)
    {
        m_p->lines = a_p->lines;
        m_p->columns = b_p->columns;
        matrix_allocate(m_p);
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
}

//m = t_a
void
matrix_transpose(matrix_t* m_p, const matrix_t* a_p)
{
    if(matrix_is_allocated(m_p))
    {
        matrix_deallocate(m_p);
    }

    m_p->lines = a_p->columns;
    m_p->columns = a_p->lines;
    matrix_allocate(m_p);

    for(uint32_t line = 0; line< m_p->lines; ++line)
    {
        for(uint32_t column = 0; column< m_p->columns; ++column)
        {
            matrix_data_t value = matrix_value_get(a_p, column, line);
            matrix_value_set(m_p, line, column, value);
        }
    }
}


void
matrix_slice_get(matrix_t* m_p, const matrix_t* a_p,
                 uint32_t line_start, uint32_t line_end, uint32_t line_step,
                 uint32_t column_start, uint32_t column_end, uint32_t column_step)
{
    OLI_UNUSED(m_p);
    OLI_UNUSED(a_p);
    OLI_UNUSED(line_start);
    OLI_UNUSED(line_end);
    OLI_UNUSED(line_step);
    OLI_UNUSED(column_start);
    OLI_UNUSED(column_end);
    OLI_UNUSED(column_step);
}

void
matrix_slice_set(matrix_t* m_p, const matrix_t* a_p,
                 uint32_t line_start, uint32_t line_end, uint32_t line_step,
                 uint32_t column_start, uint32_t column_end, uint32_t column_step)
{
    OLI_UNUSED(m_p);
    OLI_UNUSED(a_p);
    OLI_UNUSED(line_start);
    OLI_UNUSED(line_end);
    OLI_UNUSED(line_step);
    OLI_UNUSED(column_start);
    OLI_UNUSED(column_end);
    OLI_UNUSED(column_step);
}


//m = { {m_0, m_1}, {m_2, m_3} }
void
matrix_concatenate(matrix_t* m_p, const matrix_t* matrix_array)
{
    OLI_UNUSED(m_p);
    OLI_UNUSED(matrix_array);
}


void
matrix_print(const matrix_t* m_p)
{
    for(uint32_t line = 0; line < m_p->lines; ++line)
    {
        for(uint32_t column=0; column<m_p->columns; ++column)
        {
            printf("|%# 9.1e", matrix_value_get(m_p, line, column));
        }
        printf("|\n");
    }

}

static void
matrix_allocate(matrix_t* m_p)
{
    m_p->array = malloc(matrix_size(m_p));
}

static void
matrix_deallocate(matrix_t* m_p)
{
    free(m_p->array);
    *m_p = MATRIX_NULL;
}
