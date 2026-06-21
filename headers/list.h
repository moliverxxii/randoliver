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

enum list_sort_order_e
{
  SORT_ORDER_ASCENDING = 0,
  SORT_ORDER_DESCENDING
};

typedef void (*list_print_f)(const void* value_p);

typedef float (*sort_value_access_f)(const void* value_p);

list_t*  list_init(const void* element_p, size_t size);
void     list_free(list_t* list_p);
uint32_t list_length(const list_t* list_p);
size_t   list_element_size(const list_t* element_p);
void*    list_value(list_t* list_p);
void     list_print(const list_t* list_p, list_print_f print_p);
void*    list_array(const list_t* list_p);
//    new
//    vvv
// n, n+1 (*element_pp), n+2
//
// n, new (*element_pp), n+1, n+2
list_t*  list_insert(list_t** element_pp, const void* next_p, size_t size);
//              new
//              vvv
// n, n+1, n+2
//
// n, n+1, n+2, new
list_t*  list_append(list_t** element_pp, const void* next_p, size_t size);
list_t** list_next(list_t* list_p);
list_t**  list_fetch(list_t** list_pp, uint32_t index);
list_t**  list_fetch_last(list_t** list_pp);

void list_sort(list_t** head_pp, sort_value_access_f sort_value, enum list_sort_order_e order);
int list_is_sorted(const list_t** head_pp, sort_value_access_f sort_value);

void list_sort_array(list_t** head_pp,
                     const void* array, size_t element_size, uint32_t element_count,
                     sort_value_access_f sort_value, enum list_sort_order_e order);

#endif /* HEADERS_LIST_H_ */
