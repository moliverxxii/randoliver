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


typedef struct
{
    float x;
    float y;
    float z;
} vector_t;

extern char marker_g;

/**
 * ||point_p||
 */
float norm_vector(vector_t vector_p);

/**
 * <point_a|point_b>
 */
float scalar_vector(vector_t vector_a, vector_t vector_b);


int saturator(int input, int satMin, int satMax);
int modulo(int input, int modulo);
void ok(int num);
void ok_p(int num, void* pointer);

void print_memory(void* pointer, size_t size);
#endif /* UTIL_H_ */
