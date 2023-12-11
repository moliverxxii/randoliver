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
const vector_t VECTOR_0 = {0, 0, 0};



float
norm_vector(vector_t vector_p)
{
	return (vector_axis_t) sqrt(scalar_vector(vector_p, vector_p));
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



void
space_operation(vector_t* result_p, const matrix_3x3_t operator, const vector_t* vectors_p, size_t vector_count)
{
    int column;
    int row;
    for (column = 0; column < vector_count; ++column)
    {
        for(row=0; row<3; row++)
        {
             ((float*) (result_p + column))[row] = scalar_vector(*(vector_t*) operator[row], vectors_p[column]);
        }
    }
}

void get_rotation(matrix_3x3_t rotation, vector_t vector_a, vector_t vector_b)
{
    vector_t vector_z1;
    subtract_vectors(&vector_z1, vector_b, vector_a);
    scale_vector(&vector_z1, vector_z1, 1/norm_vector(vector_z1));

    float cos_a1 = scalar_vector(vector_z1, VECTOR_Z);
    float sin_a1 = sqrt(1 - pow(cos_a1, 2));
    printf("rotation a1 %.6f*pi rad\n", acos(cos_a1)/M_PI);

    vector_t vector_z2 = vector_z1;
    vector_z2.z = 0;
    scale_vector(&vector_z2, vector_z2, 1/norm_vector(vector_z2));

    float cos_a2 = cos_a1 == 1.0f ? 1.0f : scalar_vector(vector_z2, VECTOR_X);
    float sin_a2 = sqrt(1 - pow(cos_a2, 2));

    float sin_sign = scalar_vector(vector_z2, VECTOR_Y);
    if(sin_sign<0)
    {
        sin_a2 *= -1;
    }
    printf("rotation,cos(a2): %f \na2 %.6f*pi rad\n", cos_a2, (sin_sign>=0 ? 1 : -1) * acos(cos_a2)/M_PI);


    matrix_3x3_t temp_rotation =
    {
        {cos_a2 * cos_a1, -sin_a2, cos_a2 * sin_a1},
        {sin_a2 * cos_a1,  cos_a2, sin_a2 * sin_a1},
        {        -sin_a1,       0,          cos_a1}
    };
    for(int n=0; n<3; ++n)
    {
        for(int m=0; m<3; ++m)
        {
            printf("% 2.3f |", temp_rotation[n][m]);
        }
        printf("\n");
    }
    printf("\n");

    memcpy(rotation, temp_rotation, sizeof(matrix_3x3_t));
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

    while(output >= modulo || output < 0)
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
