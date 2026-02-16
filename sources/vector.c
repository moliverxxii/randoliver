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

#include "utility.h"
#include "vector.h"
#include "operator.h"

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

void
vector_print(vector_t vector)
{
    printf(" (% 10.3f, % 10.3f, % 10.3f)\n",vector.x, vector.y, vector.z);
}

int
vector_is_equal(vector_t a, vector_t b)
{
    return memcmp(a.array, b.array, sizeof(a.array)) == 0;
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
    static vector_t previous_normal;
    OLI_UNUSED(previous_normal);
    if(vector_norm(vector_product(vector, normal)) == 0)
    {
        return vector;
    }
    //calcul du changements de base
    //rot_z // normal
    vector_t rot_z = vector_normalise(normal);

    //rot_x = vector - rot_z * <vector|rot_z>;
    float vector_rot_z = vector_scalar(vector, rot_z);
    vector_t rot_x = vector_subtract(vector, vector_scale(rot_z, vector_rot_z));
    rot_x = vector_normalise(rot_x);

    vector_t rot_y = vector_product(rot_z, rot_x);

    operator_t* base_change_0r = operator_init_columns(rot_x, rot_y, rot_z);

    static float previous_angle = 0;
    OLI_UNUSED(previous_angle);
    matrix_t* matrix_base_0r_p = matrix_init(VECTOR_AXIS_COUNT, VECTOR_AXIS_COUNT);
    matrix_set(matrix_base_0r_p, operator_data(base_change_0r));
    operator_free(base_change_0r);

    matrix_t* matrix_base_r0_p = matrix_init(VECTOR_AXIS_COUNT, VECTOR_AXIS_COUNT);
    matrix_transpose(matrix_base_r0_p, matrix_base_0r_p);

    //calcul de la matrice de rotation
    operator_t* rotation_p = operator_init_lines(
        vector_init(cos(angle), -sin(angle), 0),
        vector_init(sin(angle),  cos(angle), 0),
        vector_init(         0,           0, 1)
    );
    matrix_t* matrix_rotation_p = matrix_init(VECTOR_AXIS_COUNT, VECTOR_AXIS_COUNT);
    matrix_set(matrix_rotation_p, operator_data(rotation_p));
    operator_free(rotation_p);

    matrix_t* matrix_temp_p = matrix_init(VECTOR_AXIS_COUNT, VECTOR_AXIS_COUNT);
    matrix_multiply(matrix_temp_p, matrix_rotation_p, matrix_base_0r_p);

    matrix_free(matrix_rotation_p);
    matrix_free(matrix_base_0r_p);

    matrix_t* matrix_rotation_total_p = matrix_init(VECTOR_AXIS_COUNT, VECTOR_AXIS_COUNT);
    matrix_multiply(matrix_rotation_total_p, matrix_base_r0_p, matrix_temp_p);
    matrix_free(matrix_base_r0_p);
    matrix_free(matrix_temp_p);

    matrix_t* matrix_vector_p = matrix_init(VECTOR_AXIS_COUNT, 1);
    matrix_set(matrix_vector_p, vector.array);

    matrix_t* matrix_vector_r_p = matrix_init(VECTOR_AXIS_COUNT, 1);

    matrix_multiply(matrix_vector_r_p, matrix_rotation_total_p, matrix_vector_p);
    matrix_free(matrix_rotation_total_p);
    const vector_pointer_t pointer = {(vector_axis_t*) matrix_data(matrix_vector_r_p)};

    vector_t vector_r = *pointer.vector_p;
    matrix_free(matrix_vector_p);
    matrix_free(matrix_vector_r_p);
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
    return vector_scale(vector, 1.0f/vector_norm(vector));
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

    vector_t vector_ap = vector_subtract(*vector_p, axis_a);

    vector_t vector_ap_rot = vector_rotate(vector_ap, z_1, angle);

    vector_t vector_p_rot = vector_add(axis_a, vector_ap_rot);

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
