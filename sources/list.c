/*
 * list.c
 *
 *  Created on: 14 mars 2026
 *      Author: moliver
 */
#include "list.h"

#include <stdio.h>
#include <string.h>

typedef struct list_t
{
    list_t* next_p;
    void*   element_p;
    size_t  size;
} list_t;

typedef void (*list_print_f)(const void*);

list_t*
list_init(const void* element_p, size_t size)
{
    list_t* list_p = malloc(sizeof(list_t));
    if(element_p != NULL)
    {
        list_t element =
        {
            NULL,
            malloc(size),
            size
        };
        if(element.element_p != NULL)
        {
            memcpy(element.element_p, element_p, size);
        }
        *list_p = element;
    };
    return list_p;
}

void
list_free(list_t* list_p)
{
    list_t* head_p = list_p;
    while(head_p != NULL)
    {
        list_t* next_p = head_p->next_p;
        free(head_p->element_p);
        free(head_p);
        head_p = next_p;
    }
}

uint32_t
list_length(const list_t* list_p)
{
    uint32_t length = 0;
    const list_t* head_p = list_p;
    while(head_p != NULL)
    {
        length++;
        const list_t* next_p = head_p->next_p;
        head_p = next_p;
    }
    return length;
}

size_t
list_element_size(const list_t* element_p)
{
    return element_p->size;
}

void*
list_value(list_t* list_p)
{
    return list_p->element_p;
}

void
list_print(const list_t* list_p, list_print_f print_p)
{
    for(const list_t* item_p = list_p;
        item_p != NULL;
        item_p = item_p->next_p) //const cast
    {
        (*print_p)(item_p->element_p);
    }
}

void*
list_array(const list_t* list_p)
{
    size_t size = list_p->size * list_length(list_p);
    void* array_p = malloc(size);
    const list_t* head_p = list_p;

    uint8_t* array_head_p = array_p;
    while(head_p != NULL)
    {
        memcpy(array_head_p, head_p->element_p, head_p->size);
        list_t* next_p = head_p->next_p;
        array_head_p += head_p->size;
        head_p = next_p;
    }
    return array_p;
}

list_t*
list_insert(list_t** element_pp, const void* new_p, size_t size)
{
    list_t* element_new_p = list_init(new_p, size);
    list_t* element_current_p = *element_pp;
    *element_pp = element_new_p;
    element_new_p->next_p = element_current_p;
    return element_new_p;
}

list_t*
list_append(list_t** element_pp, const void* new_p, size_t size)
{
    list_t* last_element_p = *list_fetch_last(element_pp);
    list_t** write_element_pp = NULL;
    if(last_element_p == NULL)
    {
        write_element_pp = element_pp;
    }
    else
    {
        write_element_pp = &last_element_p->next_p;
    }

    return list_insert(write_element_pp, new_p, size);
}

list_t**
list_next(list_t* list_p)
{
    list_t** next_pp = NULL;
    if(list_p != NULL)
    {
        next_pp = &list_p->next_p;
    }
    return next_pp;
}

list_t**
list_fetch(list_t** list_pp, uint32_t index)
{
    list_t** position_pp = list_pp;
    for(uint32_t position = 0; position < index; ++position)
    {
        if(*position_pp == NULL)
        {
            break;
        }
        position_pp = &(*position_pp)->next_p;
    }
    return position_pp;
}

list_t**
list_fetch_last(list_t** list_pp)
{
    list_t** position_pp = list_pp;
    if(*position_pp != NULL)
    {
        while((*position_pp)->next_p != NULL)
        {
            position_pp = &(*position_pp)->next_p;
        }
    }

    return position_pp;
}

list_t*
list_pop(list_t** item_pp)
{
    list_t* popped_p = *item_pp;
    if(popped_p != NULL)
    {
        *item_pp = popped_p->next_p;
        popped_p->next_p = NULL;
    }
    return popped_p;
}

void
list_push(list_t** list_pp, list_t* element_p)
{
    if(element_p != NULL)
    {
        element_p->next_p = *list_pp;
        *list_pp = element_p;
    }
}

void
list_stick(list_t** list_a_pp, list_t* list_b_p)
{
    list_t** last_pp = list_fetch_last(list_a_pp);
    if(*last_pp != NULL)
    {
        (*last_pp)->next_p = list_b_p;
    }
    else
    {
        *last_pp = list_b_p;
    }
}

void
list_partition(list_t** inferior_pp, list_t** superior_pp, sort_value_access_f sort_value)
{
    uint32_t length = list_length(*inferior_pp);
    uint32_t random_threshold_index = (uint32_t) ((uint64_t) (length - 1) * rand() / (uint64_t) RAND_MAX);

    list_t** pivot_pp = list_fetch(inferior_pp, random_threshold_index);
    float pivot_value = (*sort_value)(list_value(*pivot_pp));

    list_t* list_inferior_p = NULL;
    list_t* list_superior_p = list_pop(pivot_pp);
    for(list_t* element_p = list_pop(inferior_pp);
        element_p != NULL;
        element_p = list_pop(inferior_pp))
    {
        float value = (*sort_value)(list_value(element_p));
        if(value <= pivot_value)
        {
            list_push(&list_inferior_p, element_p);
        }
        else
        {
            list_push(&list_superior_p, element_p);
        }
    }
    *inferior_pp = list_inferior_p;
    *superior_pp = list_superior_p;
}

void
list_sort(list_t** head_pp,
          sort_value_access_f sort_value, enum list_sort_order_e order)
{
    list_t* list_inferior_p = *head_pp;
    list_t* list_superior_p = NULL;

    list_partition(&list_inferior_p, &list_superior_p, sort_value);
    if(list_length(list_inferior_p) > 1)
    {
        list_sort(&list_inferior_p, sort_value, order);
    }
    if(list_length(list_superior_p) > 1)
    {
        list_sort(&list_superior_p, sort_value, order);
    }

    list_stick(&list_inferior_p, list_superior_p);

    *head_pp = list_inferior_p;
}

int
list_is_sorted(const list_t** head_pp, sort_value_access_f sort_value)
{
    float previous_value = 0;
    int is_sorted = 1;
    for(const list_t* element_p = *head_pp;
        element_p != NULL;
        element_p = *list_next((list_t*)element_p))
    {
        float value = (*sort_value)(list_value((list_t*) element_p));

        if(element_p != *head_pp)
        {
            if(value < previous_value)
            {
                is_sorted = 0;
                break;
            }
        }
        previous_value = value;
    }
    return is_sorted;
}


void
list_sort_array(list_t** head_pp,
                const void* array_p, size_t element_size, uint32_t element_count,
                sort_value_access_f sort_value, enum list_sort_order_e order)
{
    list_t* list_head_vector_p = NULL;
    //tri des vecteurs de la liste.
    for(uint32_t index = 0; index < element_count; ++index)
    {
        const void* current_p = (const char*) array_p + element_size * index;
        float current_compare_value = sort_value(current_p);

        list_t** compared_element_pp = NULL;
        for(compared_element_pp = &list_head_vector_p;
            *compared_element_pp != NULL;
            compared_element_pp = list_next(*compared_element_pp))
        {
            float compared = sort_value(list_value(*compared_element_pp));
            int sort_b = current_compare_value <= compared;
            sort_b = (order == SORT_ORDER_ASCENDING) ? sort_b : !sort_b;
            if(sort_b)
            {
                break;
            }
        }

        list_insert(compared_element_pp, current_p, element_size);
    }

    *head_pp = list_head_vector_p;
}
