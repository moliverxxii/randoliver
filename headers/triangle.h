#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "point.h"

typedef struct
{
    point_t* array[3];
    colour_t colour;
} triangle_t;


#endif //TRIANGLE_H_
