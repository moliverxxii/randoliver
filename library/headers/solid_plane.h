/*
 * solid_plane.h
 *
 *  Created on: 15 juin 2026
 *      Author: moliver
 */

#ifndef HEADERS_SOLID_PLANE_H_
#define HEADERS_SOLID_PLANE_H_

#include "solid.h"

solid_t* solid_plane_init(uint32_t L, uint32_t H, vector_axis_t l_u, vector_axis_t h_u);

#endif /* HEADERS_SOLID_PLANE_H_ */
