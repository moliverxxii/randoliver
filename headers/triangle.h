#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "vector.h"
#include "colours.h"

#include "renderable.h"

typedef struct triangle_t triangle_t;

triangle_t* triangle_init(vector_t* a_p, vector_t* b_p, vector_t* c_p,
                         colour_t colour);
triangle_t* triangle_init_list(vector_t* list_p,
                               uint32_t a,
                               uint32_t b,
                               uint32_t c,
                               colour_t colour);
void        triangle_free(triangle_t* triangle_p);
void triangle_render(const void* this_p,
                     image_t* image_p,
                     const camera_t* camera_p);

renderable_i* triangle_renderable(triangle_t* triangle_p);


#endif //TRIANGLE_H_
