/*
 * animation.h
 *
 *  Created on: 8 févr. 2024
 *      Author: moliver
 */

#ifndef HEADERS_ANIMATION_H_
#define HEADERS_ANIMATION_H_

#include <stdint.h>

#include "image.h"

typedef struct animation_t
{
    uint32_t count;
    image_t* image_p;
} animation_t;

typedef struct void_pointer_t
{
    uint32_t count;
    void* pointer_p;
} void_pointer_t;

typedef union pointer_t
{
    void_pointer_t void_p;
    animation_t    animation_p;
} pointer_t;

#endif /* HEADERS_ANIMATION_H_ */
