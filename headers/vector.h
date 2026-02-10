/*
 * transform.h
 *
 *  Created on: 6 déc. 2023
 *      Author: moliver
 */

#ifndef HEADERS_VECTOR_H_
#define HEADERS_VECTOR_H_

#include <stdlib.h>
#include <stdint.h>
#include "matrix.h"
typedef matrix_data_t vector_axis_t;

typedef enum
{
    VECTOR_AXIS_X = 0,
    VECTOR_AXIS_Y,
    VECTOR_AXIS_Z,
    VECTOR_AXIS_COUNT
} vector_axis_e;

/**
 * Un type pour la representation dans l'espace.
 */
typedef union
{
    struct
    {
        vector_axis_t x;
        vector_axis_t y;
        vector_axis_t z;
    };
    vector_axis_t array[VECTOR_AXIS_COUNT];
} vector_t;


#define VECTOR_AXIS_MASK(AXIS) VECTOR_AXIS_MASK_##AXIS = 1 << VECTOR_AXIS_##AXIS

typedef enum
{
    VECTOR_AXIS_MASK(X),
    VECTOR_AXIS_MASK(Y),
    VECTOR_AXIS_MASK(Z),
    VECTOR_AXIS_MASK_ALL = VECTOR_AXIS_MASK_X | VECTOR_AXIS_MASK_Y | VECTOR_AXIS_MASK_Z
} vector_axis_mask_e;

typedef vector_axis_t matrix_3x3_t [VECTOR_AXIS_COUNT][VECTOR_AXIS_COUNT];
typedef vector_axis_t matrix_3x1_t [VECTOR_AXIS_COUNT];

extern const vector_t VECTOR_X;
extern const vector_t VECTOR_Y;
extern const vector_t VECTOR_Z;
extern const vector_t VECTOR_0;

vector_t vector_init(vector_axis_t x, vector_axis_t y, vector_axis_t z);
vector_t vector_init_array(const vector_axis_t* array);

void vector_print(vector_t vector);

/**
 * ||vector_p||
 */
float vector_norm(vector_t vector_p);

/**
 * <vector_a|vector_b>
 */
float vector_scalar(vector_t vector_a, vector_t vector_b);

/**
 * vector_a ^ vector_b
 */
vector_t vector_product(vector_t vector_a, vector_t vector_b);

/** *vector_r_p = vector_a + vector_b
 *
 */
vector_t* vector_add(vector_t* vector_r_p, vector_t vector_a, vector_t vector_b);

/** *vector_r_p = vector_a - vector_b
 *
 */
vector_t* vector_subtract(vector_t* vector_r_p, vector_t vector_a, vector_t vector_b);

/** *vector_r_p = scale * vector
 *
 */
vector_t* vector_scale(vector_t* vector_r_p, vector_t vector, float scale);

/** *vector_r_p = scale - vector
 *
 */
vector_t* vector_negative(vector_t* vector_r_p, vector_t vector);

void print_operator(const matrix_3x3_t operator);

/** result_p[i] = operator X vectors_p[i], int i, 0<=i <vector_count
 *
 */
void space_operation(vector_t* result_p, const matrix_3x3_t operator, const vector_t* vectors_p, uint32_t vector_count);

/** matrix_trans = matrix^T
 *
 */
void transpose_operator(matrix_3x3_t matrix_trans, const matrix_3x3_t matrix);

/**
 * Calculer la matrice de rotation pour obtenir l'opération qui a donné z0 -> z1 = AB/||AB||
 */
void get_rotation(matrix_3x3_t rotation, vector_t vector_a, vector_t vector_b);


void vector_translate(vector_t* vector_p, vector_t direction);
void vector_rotate(vector_t* vector_p, vector_t axis_a, vector_t axis_b, float angle);

void vector_project(vector_t* vector_p, vector_t axis_a, vector_t axis_b);

void vector_scale_radial(vector_t* vector_p, vector_t reference, float scale);
void vector_scale_axial(vector_t* vector_p, vector_t axis_a, vector_t axis_b, float scale);
void vector_scale_planar(vector_t* vector_p, vector_t plane_a, vector_t normal_b, float scale);

void vector_random(vector_t* vector_p, int height, int width);

/**
 * Randomly moves a vector.
 */
void vector_random_delta(vector_t* vector_p, int amplitude, int width, int height);





#endif /* HEADERS_VECTOR_H_ */
