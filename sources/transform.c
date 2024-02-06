/*
 * transform.c
 *
 *  Created on: 6 déc. 2023
 *      Author: moliver
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "transform.h"
#include "utility.h"

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
    return vector_a.x*vector_b.x
         + vector_a.y*vector_b.y
         + vector_a.z*vector_b.z;
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
    return add_vectors(vector_r_p,
                       vector_a,
                       *negative_vector(&minus_vector_b, vector_b));
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
print_operator(const matrix_3x3_t operator)
{
    for(int n=0; n<3; ++n)
    {
        for(int m=0; m<3; ++m)
        {
            printf("% 2.3f |", operator[n][m]);
        }
        printf("\n");
    }
    printf("\n");
}


void
space_operation(vector_t* result_p,
                const matrix_3x3_t operator,
                const vector_t* vectors_p,
                size_t vector_count)
{
    int column;
    int row;
    for (column = 0; column < vector_count; ++column)
    {
        for(row=0; row<3; row++)
        {
             ((float*) (result_p + column))[row]
           = scalar_vector(*(vector_t*) operator[row], vectors_p[column]);
        }
    }
}

void
transpose_operator(matrix_3x3_t matrix_trans, const matrix_3x3_t matrix)
{
    if(!matrix_trans || !matrix)
    {
        return;
    }
    for(int row = 0; row <3; ++row)
    {
        for(int column = 0; column<3; ++column)
        {
            matrix_trans[row][column] = matrix[column][row];
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

    matrix_3x3_t temp_rotation =
    {
        {cos_a2 * cos_a1, -sin_a2, cos_a2 * sin_a1},
        {sin_a2 * cos_a1,  cos_a2, sin_a2 * sin_a1},
        {        -sin_a1,       0,          cos_a1}
    };

    memcpy(rotation, temp_rotation, sizeof(matrix_3x3_t));
}


void
translate_vector(vector_t* vector_p, vector_t direction)
{
	add_vectors(vector_p, *vector_p, direction);
}

void
rotate_vector(vector_t* vector_p, vector_t axis_a, vector_t axis_b, float angle)
{
    //Calcul de la base de l'axe de rotation
    vector_t z_1;
    subtract_vectors(&z_1, axis_b, axis_a);
    scale_vector(&z_1, z_1, 1/norm_vector(z_1));

    matrix_3x3_t base_rotation;
    get_rotation(base_rotation, axis_a, axis_b);

    struct vector_xy
    {
        vector_t x_y;
        vector_t y_1;
    };

    const struct vector_xy xy_0 =
    {
        VECTOR_X,
        VECTOR_Y
    };

    struct vector_xy xy_1;
    space_operation((vector_t*) &xy_1,
                    base_rotation,
                    (vector_t*) &xy_0,
                    sizeof(struct vector_xy)/sizeof(vector_t));


    matrix_3x3_t base_1_to_0;
    *(struct vector_xy*) &base_1_to_0[0] = xy_1;
    *(vector_t*)         &base_1_to_0[2] = z_1;
    matrix_3x3_t base_0_to_1;
    transpose_operator(base_0_to_1, base_1_to_0);


    vector_t vector_axis_point;
    project_vector(&vector_axis_point, axis_a, axis_b);

    vector_t vector_pr0;

    subtract_vectors(&vector_pr0, *vector_p, vector_axis_point);

    //base 0->1
    vector_t vector_pr1;
    space_operation(&vector_pr1, base_0_to_1, &vector_pr0, 1);

    matrix_3x3_t rotation =
    {
        {cos(angle), -sin(angle), 0},
        {sin(angle),  cos(angle), 0},
        {         0,           0, 1}
    };

    //rotation
    vector_t vector_pr1_rot;
    space_operation(&vector_pr1_rot, rotation, &vector_pr1, 1);

    //base 1->0
    vector_t vector_pr0_rot;
    space_operation(&vector_pr0_rot, base_1_to_0, &vector_pr1_rot, 1);

    vector_t vector_p_rot;
    add_vectors(&vector_p_rot, vector_pr0_rot, vector_axis_point);

    *vector_p = vector_p_rot;
}


void
project_vector(vector_t* vector_p, vector_t axis_a, vector_t axis_b)
{
    vector_t vect_ab;
	vector_t vect_ap;
	subtract_vectors(&vect_ab, axis_b, axis_a);
	subtract_vectors(&vect_ap, *vector_p, axis_a);

	float scale = scalar_vector(vect_ab, vect_ap)/scalar_vector(vect_ab, vect_ab);

	vector_t vect_temp;
	scale_vector(&vect_temp, vect_ab, scale);
	add_vectors(vector_p, axis_a, vect_temp);
}


void
radial_scale_vector(vector_t* vector_p, vector_t reference, float scale)
{
    vector_t vector_temp = VECTOR_0;

	subtract_vectors(&vector_temp, *vector_p, reference);
	scale_vector(&vector_temp, vector_temp, scale);
	add_vectors(vector_p, vector_temp, reference);
}

void
axial_scale_vector(vector_t* vector_p, vector_t axis_a, vector_t axis_b, float scale)
{
    vector_t vector_r = *vector_p;
	project_vector(&vector_r, axis_a, axis_b);

	vector_t vect_rp;
	subtract_vectors(&vect_rp, *vector_p, vector_r);
	scale_vector(&vect_rp, vect_rp, scale);

	add_vectors(vector_p, vect_rp, *vector_p);

}

void
planar_scale_vector(vector_t* point_p, vector_t plane_a, vector_t normal_b, float scale)
{
}

void
rand_coord_vector(vector_t* point_p, int height, int width)
{
    int randX = rand() % width;
    int randY = rand() % height;
    point_p->x = randX;
    point_p->y = randY;
}

void
rand_delta_vector(vector_t* point_p, int amplitude, int width, int height)
{
    point_p->x += rand() % (2 * amplitude + 1) - amplitude;
    modulo(point_p->x, width);
    point_p->y += rand() % (2 * amplitude + 1) - amplitude;
    modulo(point_p->y, height);
}
