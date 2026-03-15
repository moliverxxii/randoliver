/*
 * list.c
 *
 *  Created on: 14 mars 2026
 *      Author: moliver
 */
#include "list.h"

#include <string.h>

typedef struct list_t
{
    list_t* next_p;
    void*   element_p;
    size_t  size;
} list_t;

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
        list_t* next_p = head_p->next_p;
        head_p = next_p;
    }
    return length;
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
    list_t* element_present_p = *element_pp;
    *element_pp = element_new_p;
    element_new_p->next_p = element_present_p;
    return element_new_p;
}

list_t*
list_append(list_t** element_pp, const void* new_p, size_t size)
{
    list_t* last_element_p = list_fetch_last(*element_pp);
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


list_t*
list_fetch_last(list_t* list_p)
{
    list_t* position_p = list_p;
    list_t* previous_p = NULL;
    while(position_p != NULL)
    {
        previous_p = position_p;
        position_p = position_p->next_p;
    }

    return previous_p;

}
