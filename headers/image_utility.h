/*
 * imgUtil.h
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */

#ifndef IMGUTIL_H_
#define IMGUTIL_H_

#include "main.h"

#define RES 2835

typedef enum
{
	COLOUR_BLUE = 0,
	COLOUR_GREEN,
	COLOUR_RED,
	COLOUR_COUNT
} pixel_colour_t;

/** Couleur en données brutes BVR.
 *
 */
typedef uchar colour_t[COLOUR_COUNT];

/** Couleur sous formes de champs lisibles par un humain.
 *  Compatible pour la conversion avec color_t
 *
 */
typedef struct
{
    uchar blue;
    uchar green;
    uchar red;
} colour_struct_t;


/** Conteneur avec les donnees de bases d'une image.
 *  Doit être utilisé pour tous les traitements d'images.
 */
typedef struct
{
    unsigned int width;
    unsigned int height;
    colour_t** image;
} image_t;

/** Un point dans l'espace
 *
 */
typedef struct
{
    colour_struct_t colour;
    int x;
    int y;
    int z;
} point_t;

/** Une sequence de points
 *
 */
typedef struct
{
    uint amount;
    point_t* sequence;
} figure_t;

/** Une camera a utiliser pour le rendu 3D
 *
 */
typedef struct
{
    vector_t origin;
    vector_t direction;
    float distance;
} camera_t;


/* Elementaires */
image_t* init_image(int width, int height);
void set_image(image_t* image);
void disp_image(image_t* image);
void draw_rect(colour_t color, int botLeftX, int botLeftY, int topRightX,
			   int topRightY, image_t* image);
void free_image(image_t* image);
figure_t init_figure(uint nombre_point);
point_t init_point(void);
void draw_figure(image_t* image, figure_t* figure);

/* Processus */
void brownien1(image_t* image, int iterations, int spread, int x0, int y0);
void barres1(image_t* image, int spread);
void barres2(image_t* image, int spread);
void init_points(point_t** seq, int n);
void rand_coord(point_t* m, int width, int height);
void rand_delta_point(point_t* m, int amplitude, int width, int height);
void free_points(point_t** seq, int n);

/* Rendu */
void init_camera(camera_t* camera_p, float origin_x, float origin_y,
        float origin_z, float destin_x, float destin_y, float destin_z,
        float distance);
void render_figure(image_t* image, figure_t figure, camera_t camera);

#endif /* IMGUTIL_H_ */
