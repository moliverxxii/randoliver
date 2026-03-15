/*
 * list.h
 *
 *  Created on: 14 mars 2026
 *      Author: moliver
 */

#ifndef HEADERS_LIST_H_
#define HEADERS_LIST_H_

#include <stdint.h>
#include <stdlib.h>

typedef struct list_t list_t;

list_t*  list_init(const void* element_p, size_t size);
void     list_free(list_t* list_p);
uint32_t list_length(const list_t* list_p);
void*    list_array(const list_t* list_p);

//    new
//    vvv
// n, n+1, n+2
//
// n, new, n+1, n+2
list_t*  list_insert(list_t** element_pp, const void* next_p, size_t size);
//              new
//              vvv
// n, n+1, n+2
//
// n, n+1, n+2, new
list_t*  list_append(list_t** element_pp, const void* next_p, size_t size);
list_t*  list_fetch_last(list_t* list_p);


#endif /* HEADERS_LIST_H_ */
