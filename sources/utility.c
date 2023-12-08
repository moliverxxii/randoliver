/*
 * util.c
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */
#include "main.h"

char marker_g = 0;

const vector_t VECTOR_X = {1, 0, 0};
const vector_t VECTOR_Y = {0, 1, 0};
const vector_t VECTOR_Z = {0, 0, 1};

float
norm_vector(vector_t vector_p)
{
	return (float) sqrt(scalar_vector(vector_p, vector_p));
}

float
scalar_vector(vector_t vector_a, vector_t vector_b)
{
	return vector_a.x*vector_b.x + vector_a.y*vector_b.y + vector_a.z*vector_b.z;
}


vector_t*
add_vectors(vector_t* vector_r_p, vector_t vector_a, vector_t vector_b)
{
    if(!vector_r_p)
    {
        return NULL;
    }
    memcpy(vector_r_p, &vector_a, sizeof(vector_t));
    vector_r_p->x += vector_b.x;
    vector_r_p->y += vector_b.y;
    vector_r_p->z += vector_b.z;
    return vector_r_p;
}

vector_t*
subtract_vectors(vector_t* vector_r_p, vector_t vector_a, vector_t vector_b)
{
    vector_t minus_vector_b;
    return add_vectors(vector_r_p, vector_a, *negative_vector(&minus_vector_b, vector_b));
}

vector_t*
scale_vector(vector_t* vector_r_p, vector_t vector, float scale)
{
    if(!vector_r_p)
    {
        return vector_r_p;
    }
    memcpy(vector_r_p, &vector, sizeof(vector));
    vector_r_p->x *= scale;
    vector_r_p->y *= scale;
    vector_r_p->z *= scale;
    return vector_r_p;

}

vector_t*
negative_vector(vector_t* vector_r_p, vector_t vector)
{
    return scale_vector(vector_r_p, vector, -1);
}



int
saturator(int input, int min, int max)
{
    if(input < min)
    {
        return min;
    }
    else if(input > max)
    {
        return max;
    }
    else
    {
        return input;
    }
}

int
modulo(int input, int modulo)
{
    int output = input;
    int multiplier = modulo;
    if(((modulo > 0) & (input > 0)) | ((modulo < 0) & (input < 0)))
    {
        multiplier *= -1;
    }

    while(output >= modulo | output < 0)
    {
        output += multiplier;
    }
    return output;

}

void
ok(int num)
{
    printf("OK n°%d\n", num);
}

void
ok_p(int num, void* pointer)
{

    if(pointer == NULL)
    {
        printf("merde n°%d\n", num);
    }
    else
    {
        ok(num);
    }
}

void
print_memory(void* pointer, size_t size)
{
	uint8_t* byte_p;
	for(byte_p = pointer; byte_p < (uint8_t*) pointer + size; ++byte_p)
	{
		printf("%02X ", *byte_p);

	}
	printf("\n");
}
