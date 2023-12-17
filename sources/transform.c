/*
 * transform.c
 *
 *  Created on: 6 déc. 2023
 *      Author: moliver
 */

#include "main.h"

point_t*
add_points(point_t* point_r_p, point_t point_a, point_t point_b)
{
	if(!point_r_p)
	{
		return NULL;
	}
	memcpy(point_r_p, &point_a, sizeof(point_t));
	point_r_p->x += point_b.x;
	point_r_p->y += point_b.y;
	point_r_p->z += point_b.z;
	return point_r_p;
}

point_t*
subtract_points(point_t* point_r_p, point_t point_a, point_t point_b)
{
	point_t minus_point_b;
	return add_points(point_r_p,
	                  point_a,
	                  *negative_point(&minus_point_b, point_b));
}

point_t*
scale_point(point_t* point_r_p, point_t point, float scale)
{
	if(!point_r_p)
	{
		return point_r_p;
	}
	memcpy(point_r_p, &point, sizeof(point));
	point_r_p->x *= scale;
	point_r_p->y *= scale;
	point_r_p->z *= scale;
	return point_r_p;

}

point_t*
negative_point(point_t* point_r_p, point_t point)
{
	return scale_point(point_r_p, point, -1);
}


void
transform_figure(figure_t* figure_p, point_transformer_t transformer_p)
{

}

void
translate_point(point_t* point_p, point_t direction)
{
	add_points(point_p, *point_p, direction);
}

void
rotate_point(point_t* point_p, point_t axis_a, point_t axis_b, float angle)
{
    //Calcul de la base de l'axe de rotation
    vector_t vector_a;
    point_to_vector(&vector_a, axis_a);

    vector_t vector_b;
    point_to_vector(&vector_b, axis_b);

    vector_t z_1;
    subtract_vectors(&z_1, vector_b, vector_a);
    scale_vector(&z_1, z_1, 1/norm_vector(z_1));

    matrix_3x3_t base_rotation;
    get_rotation(base_rotation, vector_a, vector_b);

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


    point_t axis_point;
    project_point(&axis_point, axis_a, axis_b);
    vector_t vector_axis_point;
    point_to_vector(&vector_axis_point, axis_point);

    vector_t vector_pr0;

    vector_t vector;
    point_to_vector(&vector, *point_p);

    subtract_vectors(&vector_pr0, vector, vector_axis_point);

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

    vector_to_point(point_p, vector_p_rot);
}

float
norm_point(point_t point_p)
{
	return (float) sqrt(scalar(point_p, point_p));
}

float
scalar(point_t point_a, point_t point_b)
{
	return point_a.x*point_b.x + point_a.y*point_b.y + point_a.z*point_b.z;
}

void
project_point(point_t* point_p, point_t axis_a, point_t axis_b)
{
	point_t vect_ab;
	point_t vect_ap;
	subtract_points(&vect_ab, axis_b, axis_a);
	subtract_points(&vect_ap, *point_p, axis_a);

	float scale = scalar(vect_ab, vect_ap)/scalar(vect_ab, vect_ab);

	point_t vect_temp;
	scale_point(&vect_temp, vect_ab, scale);
	add_points(point_p, axis_a, vect_temp);
}


void
radial_scale_point(point_t* point_p, point_t reference, float scale)
{
	point_t point_temp = init_point();

	subtract_points(&point_temp, *point_p, reference);
	scale_point(&point_temp, point_temp, scale);
	add_points(point_p, point_temp, reference);
}

void
axial_scale_point(point_t* point_p, point_t axis_a, point_t axis_b, float scale)
{
	point_t point_r = *point_p;
	project_point(&point_r, axis_a, axis_b);

	point_t vect_rp;
	subtract_points(&vect_rp, *point_p, point_r);
	scale_point(&vect_rp, vect_rp, scale);

	add_points(point_p, vect_rp, *point_p);

}

void
planar_scale_point(point_t* point_p, point_t plane_a, point_t normal_b, float scale)
{
}

void
rand_coord_point(point_t* point_p, int height, int width)
{
    int randX = rand() % width;
    int randY = rand() % height;
    point_p->x = randX;
    point_p->y = randY;
}

void
rand_delta_point(point_t* point_p, int amplitude, int width, int height)
{
    point_p->x += rand() % (2 * amplitude + 1) - amplitude;
    modulo(point_p->x, width);
    point_p->y += rand() % (2 * amplitude + 1) - amplitude;
    modulo(point_p->y, height);
}
