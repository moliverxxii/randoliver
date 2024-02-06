/*
 * transform.c
 *
 *  Created on: 6 déc. 2023
 *      Author: moliver
 */

#include "main.h"



void
transform_figure(figure_t* figure_p, point_transformer_t transformer_p)
{

}

void
translate_vector(vector_t* vector_p, vector_t direction)
{
	add_vectors(vector_p, *vector_p, direction);
}

void
rotate_point(vector_t* vector_p, vector_t axis_a, vector_t axis_b, float angle)
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
    project_point(&vector_axis_point, axis_a, axis_b);

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
project_point(vector_t* vector_p, vector_t axis_a, vector_t axis_b)
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
radial_scale_point(vector_t* vector_p, vector_t reference, float scale)
{
    vector_t vector_temp = VECTOR_0;

	subtract_vectors(&vector_temp, *vector_p, reference);
	scale_vector(&vector_temp, vector_temp, scale);
	add_vectors(vector_p, vector_temp, reference);
}

void
axial_scale_point(vector_t* vector_p, vector_t axis_a, vector_t axis_b, float scale)
{
    vector_t vector_r = *vector_p;
	project_point(&vector_r, axis_a, axis_b);

	vector_t vect_rp;
	subtract_vectors(&vect_rp, *vector_p, vector_r);
	scale_vector(&vect_rp, vect_rp, scale);

	add_vectors(vector_p, vect_rp, *vector_p);

}

void
planar_scale_point(vector_t* point_p, vector_t plane_a, vector_t normal_b, float scale)
{
}

void
rand_coord_point(vector_t* point_p, int height, int width)
{
    int randX = rand() % width;
    int randY = rand() % height;
    point_p->x = randX;
    point_p->y = randY;
}

void
rand_delta_point(vector_t* point_p, int amplitude, int width, int height)
{
    point_p->x += rand() % (2 * amplitude + 1) - amplitude;
    modulo(point_p->x, width);
    point_p->y += rand() % (2 * amplitude + 1) - amplitude;
    modulo(point_p->y, height);
}
