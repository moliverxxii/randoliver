/*
 * transform.h
 *
 *  Created on: 6 déc. 2023
 *      Author: moliver
 */

#ifndef HEADERS_TRANSFORM_H_
#define HEADERS_TRANSFORM_H_

#include <stdlib.h>
#include <stdint.h>

#include "transform.h"

typedef float vector_axis_t;

/**
 * Un type pour la representation dans l'espace.
 */
typedef struct
{
    vector_axis_t x;
    vector_axis_t y;
    vector_axis_t z;
} vector_t;

typedef vector_axis_t matrix_3x3_t [3][3];
typedef vector_axis_t matrix_3x1_t [3];

extern const vector_t VECTOR_X;
extern const vector_t VECTOR_Y;
extern const vector_t VECTOR_Z;
extern const vector_t VECTOR_0;

/**
 * ||vector_p||
 */
float norm_vector(vector_t vector_p);

/**
 * <vector_a|vector_b>
 */
float scalar_vector(vector_t vector_a, vector_t vector_b);


/** *vector_r_p = vector_a + vector_b
 *
 */
vector_t* add_vectors(vector_t* vector_r_p, vector_t vector_a, vector_t vector_b);

/** *vector_r_p = vector_a - vector_b
 *
 */
vector_t* subtract_vectors(vector_t* vector_r_p, vector_t vector_a, vector_t vector_b);

/** *vector_r_p = scale * vector
 *
 */
vector_t* scale_vector(vector_t* vector_r_p, vector_t vector, float scale);

/** *vector_r_p = scale - vector
 *
 */
vector_t* negative_vector(vector_t* vector_r_p, vector_t vector);

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


void translate_vector(vector_t* vector_p, vector_t direction);
void rotate_vector(vector_t* vector_p, vector_t axis_a, vector_t axis_b, float angle);

void project_vector(vector_t* vector_p, vector_t axis_a, vector_t axis_b);

void radial_scale_vector(vector_t* vector_p, vector_t reference, float scale);
void axial_scale_vector(vector_t* vector_p, vector_t axis_a, vector_t axis_b, float scale);
void planar_scale_vector(vector_t* vector_p, vector_t plane_a, vector_t normal_b, float scale);

void rand_coord_vector(vector_t* vector_p, int height, int width);

/**
 * Randomly moves a vector.
 */
void rand_delta_vector(vector_t* vector_p, int amplitude, int width, int height);





#endif /* HEADERS_TRANSFORM_H_ */
