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

typedef vector_axis_t matrix_3x3_t [VECTOR_AXIS_COUNT][VECTOR_AXIS_COUNT];
typedef vector_axis_t matrix_3x1_t [VECTOR_AXIS_COUNT];

typedef struct operator_t
{
    matrix_3x3_t matrix;
} operator_t;



const vector_axis_t*
operator_data(const operator_t* operator_p)
{
    return &operator_p->matrix[0][0];
}

operator_t*
operator_init_lines(vector_t x, vector_t y, vector_t z)
{
    operator_t operator =
    {
        {
            {x.x, x.y, x.z},
            {y.x, y.y, y.z},
            {z.x, z.y, z.z}
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
operator_init_columns(vector_t x, vector_t y, vector_t z)
{
    operator_t operator =
    {
        {
            {x.x, y.x, z.x},
            {x.y, y.y, z.y},
            {x.z, y.z, z.z}
        }
    };

    operator_t* operator_p = malloc(sizeof(operator));
    if(operator_p != NULL)
    {
        *operator_p = operator;
    }
    return operator_p;
}


void
operator_free(operator_t* operator_p)
{
    free(operator_p);
}



void
operator_print(const operator_t* operator)
{
    for(int n=0; n<VECTOR_AXIS_COUNT; ++n)
    {
        for(int m=0; m<VECTOR_AXIS_COUNT; ++m)
        {
            printf("% 2.3f |", operator->matrix[n][m]);
        }
        printf("\n");
    }
    printf("\n");
}


void
operator_transpose(operator_t* t_p, const operator_t* m_p)
{
    for(int row = 0; row <VECTOR_AXIS_COUNT; ++row)
    {
        for(int column = 0; column<VECTOR_AXIS_COUNT; ++column)
        {
            t_p->matrix[row][column] = m_p->matrix[column][row];
        }
    }
}

