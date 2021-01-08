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

typedef uchar color_t[3];
typedef color_t** image_t;
typedef struct point_t
{
    int x; 
    int y;
    int z;
    uchar blue;
    uchar green;
    uchar red;
} point_t;
typedef struct camera_t
{
    vector_t origine;
    vector_t direction;
    float distance;
} camera_t;
typedef struct figure_t
{
    uint nombre_Point; 
    point_t* sequence;
} figure_t;

/* Elementaires */
image_t initImage(int width, int height);
void setImage(image_t image, int width, int height);
void dispImage(image_t image, int width, int height);
void drawRect(color_t color ,int botLeftX,int botLeftY, int topRightX, int topRightY, image_t image);
void freeImage(image_t image, int width, int height);
void drawPoints(point_t** seq,int nPoints, image_t image);
figure_t initFigure(uint nombre_point);
point_t initPoint(void);

/* Processus */
void brownien1(image_t image,int width,int height,int iterations, int spread,int x0, int y0);
void barres1(image_t image, int width, int height, int spread);
void barres2(image_t image, int width, int height, int spread);
void initPoints(point_t** seq, int n);
void randCoord(point_t* m,int width, int height);
void randDeltaPoint(point_t* m, int amplitude,int width, int height);
void freePoints(point_t** seq, int n);

/* Rendu */
void renderFigure(image_t image, figure_t figure ,camera_t camera, uchar* color);

#endif /* IMGUTIL_H_ */
