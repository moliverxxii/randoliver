/*
 * transform.c
 *
 *  Created on: 6 déc. 2023
 *      Author: moliver
 */

#include "../headers/vector.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "utility.h"

const vector_t VECTOR_X = {{1, 0, 0}};
const vector_t VECTOR_Y = {{0, 1, 0}};
const vector_t VECTOR_Z = {{0, 0, 1}};
const vector_t VECTOR_0 = {{0, 0, 0}};


vector_t
vector_init(vector_axis_t x, vector_axis_t y, vector_axis_t z)
{
    vector_t vector = {{x, y, z}};
    return vector;
}

vector_t vector_init_array(const vector_axis_t* array)
{
    vector_t vector;
    memcpy(vector.array, array, sizeof(vector.array));
    return vector;
}

void vector_print(vector_t vector)
{
    printf(" (% 10.3f, % 10.3f, % 10.3f)\n",vector.x, vector.y, vector.z);
}


float
vector_norm(vector_t vector_p)
{
    return (vector_axis_t) sqrt(vector_scalar(vector_p, vector_p));
}

float
vector_scalar(vector_t vector_a, vector_t vector_b)
{
    float scalar = 0;
    for(int axis = 0; axis < VECTOR_AXIS_COUNT; ++axis)
    {
         scalar += vector_a.array[axis]*vector_b.array[axis];
    }
    return scalar;
}

vector_t vector_product(vector_t vector_a, vector_t vector_b)
{
    return vector_init(
            vector_a.y * vector_b.z - vector_a.z * vector_b.y,
            vector_a.z * vector_b.x - vector_a.x * vector_b.z,
            vector_a.x * vector_b.y - vector_a.y * vector_b.x);
}


vector_t
vector_add(vector_t vector_a, vector_t vector_b)
{
    vector_t vector_r = vector_a;
    for(int axis = 0; axis < VECTOR_AXIS_COUNT; ++axis)
    {
         vector_r.array[axis] += vector_b.array[axis];
    }
    return vector_r;
}

vector_t
vector_subtract(vector_t vector_a, vector_t vector_b)
{
    return vector_add(vector_a, vector_negative(vector_b));
}

vector_t
vector_scale(vector_t vector, float scale)
{
    vector_t vector_r = vector;
    for(int axis = 0; axis < VECTOR_AXIS_COUNT; ++axis)
    {
         vector_r.array[axis] *= scale;
    }
    return vector_r;

}

/** tourne le vecteur vector dans le sens direct angle dans le plan normal au vecteur normal.
 *
 */
vector_t
vector_rotate(vector_t vector, vector_t normal, float angle)
{
    //base de roation
    //rot_z // normal
    vector_t rot_z = vector_normalise(normal);

    //rot_x = vector - rot_z * <vector|rot_z>;
    float vector_rot_z = vector_scalar(vector, rot_z);
    vector_t rot_x = vector_subtract(vector, vector_scale(rot_z, vector_rot_z));
    rot_x = vector_normalise(rot_x);

    vector_t rot_y = vector_product(rot_z, rot_x);

    matrix_3x3_t base_change_r_0;

    memcpy(base_change_r_0[VECTOR_AXIS_X], &rot_x, sizeof(rot_x));
    memcpy(base_change_r_0[VECTOR_AXIS_Y], &rot_y, sizeof(rot_y));
    memcpy(base_change_r_0[VECTOR_AXIS_Z], &rot_y, sizeof(rot_z));

    matrix_3x3_t base_change_0_r;
    operator_transpose(base_change_0_r, base_change_r_0);

    matrix_3x3_t rotation =
    {
        {cos(angle), -sin(angle), 0},
        {sin(angle),  cos(angle), 0},
        {         0,           0, 1}
    };

    vector_t vector_r;

    return vector_r;
}


vector_t
vector_negative(vector_t vector)
{
    return vector_scale(vector, -1);
}

vector_t
vector_normalise(vector_t vector)
{
    return vector_scale(vector, vector_norm(vector));
}

void
operator_print(const matrix_3x3_t operator)
{
    for(int n=0; n<VECTOR_AXIS_COUNT; ++n)
    {
        for(int m=0; m<VECTOR_AXIS_COUNT; ++m)
        {
            printf("% 2.3f |", operator[n][m]);
        }
        printf("\n");
    }
    printf("\n");
}


void
operator_operation(vector_t* result_p,
                const matrix_3x3_t operator,
                const vector_t* vectors_p,
                uint32_t vector_count)
{
    for (uint32_t column = 0; column < vector_count; ++column)
    {
        for(uint32_t row=0; row<VECTOR_AXIS_COUNT; row++)
        {
            vector_t operator_row = *(vector_t*) operator[row];

            result_p[column].array[row] = vector_scalar(operator_row, vectors_p[column]);
        }
    }
}

void
operator_transpose(matrix_3x3_t matrix_trans, const matrix_3x3_t matrix)
{
    if(!matrix_trans || !matrix)
    {
        return;
    }
    for(int row = 0; row <VECTOR_AXIS_COUNT; ++row)
    {
        for(int column = 0; column<VECTOR_AXIS_COUNT; ++column)
        {
            matrix_trans[row][column] = matrix[column][row];
        }
    }
}


void get_rotation(matrix_3x3_t rotation, vector_t vector_a, vector_t vector_b)
{
    vector_t vector_z1 = vector_subtract(vector_b, vector_a);
    vector_z1 = vector_scale(vector_z1, 1/vector_norm(vector_z1));

    float cos_a1 = vector_scalar(vector_z1, VECTOR_Z);
    float sin_a1 = sqrt(1 - pow(cos_a1, 2));

    vector_t vector_z2 = vector_z1;
    vector_z2.z = 0;
    vector_z2 = vector_scale(vector_z2, 1/vector_norm(vector_z2));

    float cos_a2 = cos_a1 == 1.0f ? 1.0f : vector_scalar(vector_z2, VECTOR_X);
    float sin_a2 = sqrt(1 - pow(cos_a2, 2));

    float sin_sign = vector_scalar(vector_z2, VECTOR_Y);
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
vector_translate(vector_t* vector_p, vector_t direction)
{
	*vector_p = vector_add(*vector_p, direction);
}

void
vector_rotate_axial(vector_t* vector_p, vector_t axis_a, vector_t axis_b, float angle)
{
    //Calcul de la base de l'axe de rotation
    vector_t z_1 = vector_subtract(axis_b, axis_a);
    z_1 = vector_scale(z_1, 1/vector_norm(z_1));

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
    operator_operation((vector_t*) &xy_1,
                    base_rotation,
                    (vector_t*) &xy_0,
                    sizeof(struct vector_xy)/sizeof(vector_t));


    matrix_3x3_t base_1_to_0;
    *(struct vector_xy*) &base_1_to_0[0] = xy_1;
    *(vector_t*)         &base_1_to_0[2] = z_1;
    matrix_3x3_t base_0_to_1;
    operator_transpose(base_0_to_1, base_1_to_0);


    vector_t vector_axis_point;
    vector_project(&vector_axis_point, axis_a, axis_b);

    vector_t vector_pr0 = vector_subtract(*vector_p, vector_axis_point);

    //base 0->1
    vector_t vector_pr1;
    operator_operation(&vector_pr1, base_0_to_1, &vector_pr0, 1);

    matrix_3x3_t rotation =
    {
        {cos(angle), -sin(angle), 0},
        {sin(angle),  cos(angle), 0},
        {         0,           0, 1}
    };

    //rotation
    vector_t vector_pr1_rot;
    operator_operation(&vector_pr1_rot, rotation, &vector_pr1, 1);

    //base 1->0
    vector_t vector_pr0_rot;
    operator_operation(&vector_pr0_rot, base_1_to_0, &vector_pr1_rot, 1);

    vector_t vector_p_rot = vector_add(vector_pr0_rot, vector_axis_point);

    *vector_p = vector_p_rot;
}


void
vector_project(vector_t* vector_p, vector_t axis_a, vector_t axis_b)
{
    vector_t vect_ab = vector_subtract(axis_b, axis_a);
	vector_t vect_ap = vector_subtract(*vector_p, axis_a);

	float scale = vector_scalar(vect_ab, vect_ap)/vector_scalar(vect_ab, vect_ab);

	*vector_p = vector_add(axis_a, vector_scale(vect_ab, scale));
}


void
vector_scale_radial(vector_t* vector_p, vector_t reference, float scale)
{
	*vector_p = vector_add(
	                       vector_scale(
	                                    vector_subtract(*vector_p, reference),
	                                    scale
	                                   ),
	                       reference
	                      );
}

void
vector_scale_axial(vector_t* vector_p, vector_t axis_a, vector_t axis_b, float scale)
{
    vector_t vector_r = *vector_p;
	vector_project(&vector_r, axis_a, axis_b);

	vector_scale_radial(vector_p, vector_r, scale);
}

void
vector_scale_planar(vector_t* point_p, vector_t plane_a, vector_t normal_b, float scale)
{
    OLI_UNUSED(point_p);
    OLI_UNUSED(plane_a);
    OLI_UNUSED(normal_b);
    OLI_UNUSED(scale);
}

void
vector_random(vector_t* point_p, int height, int width)
{
    int randX = rand() % width;
    int randY = rand() % height;
    point_p->x = randX;
    point_p->y = randY;
}

void
vector_random_delta(vector_t* point_p, int amplitude, int width, int height)
{
    point_p->x += rand() % (2 * amplitude + 1) - amplitude;
    modulo(point_p->x, width);
    point_p->y += rand() % (2 * amplitude + 1) - amplitude;
    modulo(point_p->y, height);
}
