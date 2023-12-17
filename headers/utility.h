/*
 * util.h
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */

#ifndef UTIL_H_
#define UTIL_H_

#define SAT_MIN 0
#define SAT_MAX 255


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

typedef float matrix_3x3_t [3][3];
typedef float matrix_3x1_t [3];

extern const vector_t VECTOR_X;
extern const vector_t VECTOR_Y;
extern const vector_t VECTOR_Z;
extern const vector_t VECTOR_0;

extern char marker_g;



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
void space_operation(vector_t* result_p, const matrix_3x3_t operator, const vector_t* vectors_p, size_t vector_count);

/** matrix_trans = matrix^T
 *
 */
void transpose_operator(matrix_3x3_t matrix_trans, const matrix_3x3_t matrix);

/**
 * Calculer la matrice de rotation pour obtenir l'opération qui a donné z0 -> z1 = AB/||AB||
 */
void get_rotation(matrix_3x3_t rotation, vector_t vector_a, vector_t vector_b);

/* Ecrête le signal entre "min" et "max"
 *
 */
int saturator(int input, int satMin, int satMax);

// Renvoie le VRAI reste de la division euclidienne de "input" par "modulo".
//modulo must be different to 0;
int modulo(int input, int modulo);

//Affiche un signal ok numéroté.
void ok(int num);

/*Affiche un signal ok numéroté, tout en vérifiant la disponibilité en mémoire
 * du pointeur.
 */
void ok_p(int num, void* pointer);

void print_memory(void* pointer, size_t size);
#endif /* UTIL_H_ */
