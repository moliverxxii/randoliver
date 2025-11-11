/*
 * util.h
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <stdlib.h>

#define OLI_UNUSED(PARAMETER) (void) PARAMETER

#define SAT_MIN 0
#define SAT_MAX 255


extern char marker_g;

/* Ecrête le signal entre "min" et "max"
 *
 */
int saturator(int input, int satMin, int satMax);

/** Renvoie le VRAI reste de la division euclidienne de "input" par "modulo".
 *
 */
int modulo(int input, int modulo);
int dither(float value);

//Affiche un signal ok numéroté.
void ok(int num);

/*Affiche un signal ok numéroté, tout en vérifiant la disponibilité en mémoire
 * du pointeur.
 */
void ok_p(int num, void* pointer);
void print_memory(void* pointer, size_t size);
#endif /* UTIL_H_ */
