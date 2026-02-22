#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "vector.h"
#include "colours.h"

#include "renderable.h"

typedef struct
{
    vector_t* array[3];
    colour_t colour;
} triangle_t;

triangle_t triangle_init(vector_t* a_p, vector_t* b_p, vector_t* c_p,
                         colour_t colour);

void camera_render_triangle(const camera_t* camera_p,
                            image_t* image_p,
                            triangle_t triangle);


#endif //TRIANGLE_H_
