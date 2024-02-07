/*
 * filter.h
 *
 *  Created on: Jan 15, 2020
 *      Author: moliver
 */

#ifndef FILTER_H_
#define FILTER_H_

#include "image_utility.h"

void flou(image_t* image_p, int radius);
void symetry(image_t* image_p);
void random_colour_shift(image_t* image_p);
#endif /* FILTER_H_ */
