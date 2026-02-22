/*
 * renderable.c
 *
 *  Created on: 17 févr. 2026
 *      Author: moliver
 */
#include "renderable.h"

renderable_i renderable_init(renderable_render_f render_function)
{
    renderable_i renderable = {render_function};
    return renderable;
}
