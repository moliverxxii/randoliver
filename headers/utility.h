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

/**
 * Un type pour la representation dans l'espace.
 */
typedef struct
{
    float x;
    float y;
    float z;
} vector_t;

extern const vector_t VECTOR_X;
extern const vector_t VECTOR_Y;
extern const vector_t VECTOR_Z;

extern char marker_g;

/**
 * ||vector_p||
 */
float norm_vector(vector_t vector_p);

/**
 * <vector_a|vector_b>
 */
float scalar_vector(vector_t vector_a, vector_t vector_b);

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
