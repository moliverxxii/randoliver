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

typedef unsigned char uchar;
typedef unsigned int uint;
typedef struct
{
    float x;
    float y;
    float z;
} vector_t;

int saturator(int input, int satMin, int satMax);
int modulo(int input, int modulo);
void ok(int num);
void okP(int num, void *pointer);

#endif /* UTIL_H_ */
