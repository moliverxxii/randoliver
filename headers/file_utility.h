/*
 * fileUtil.h
 *
 *  Created on: 13 févr. 2019
 *      Author: moliver
 */

#ifndef FILEUTIL_H_
#define FILEUTIL_H_

#include "main.h"

#define HEADER_SIZE 54

FILE* initImageFile(char* name, image_t* image);
void initHeader(FILE* file, int width, int height);
void writeImage(image_t* image, FILE* imageFile);
int getInt(FILE* file);
int getWidth(FILE* imageFile);
int getHeight(FILE* imageFile);
char* bmpExtension(char* input);
char* numExtension(char* input, int number);

#endif /* FILEUTIL_H_ */
