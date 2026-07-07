/*
 * preset_lists.c
 *
 *  Created on: 7 juil. 2026
 *      Author: moliver
 */
#include <stdlib.h>
#include <stdio.h>

#include "list.h"
#include "preset.h"


static void oli_test_lists();
static void test_list_print(const void* value_p);
static float test_list_value(const void* value_p);

void
preset_add_lists()
{
    preset_add("test listes et tri",   &oli_test_lists            , NULL, NULL, NULL, 1);
}


static void
oli_test_lists()
{
    //liste courte
    uint32_t list_length = 10;
    list_t* list_p = NULL;
    for(uint32_t item = 0; item < list_length; ++item)
    {
        int rand_int = rand() >> 16;
        list_append(&list_p, &rand_int, sizeof(rand_int));
    }

    list_print(list_p, &test_list_print);
    printf("%s\n", list_is_sorted((const list_t**) &list_p, &test_list_value) ? "trie" : "pas trie!");

    list_sort(&list_p, &test_list_value, SORT_ORDER_ASCENDING);
    list_print(list_p, &test_list_print);
    printf("%s\n", list_is_sorted((const list_t**) &list_p, &test_list_value) ? "trie" : "pas trie!");
    list_free(list_p);

    list_p = NULL;
    list_length = 250000;
    printf("on rempli la liste\n");
    for(uint32_t item = 0; item < list_length; ++item)
    {
        int rand_int = rand();
        list_insert(&list_p, &rand_int, sizeof(rand_int));
    }
    printf("liste remplie\n");

    printf("%s\n", list_is_sorted((const list_t**) &list_p, &test_list_value) ? "trie" : "pas trie!");
    list_sort(&list_p, &test_list_value, SORT_ORDER_ASCENDING);
    printf("%s\n", list_is_sorted((const list_t**) &list_p, &test_list_value) ? "trie" : "pas trie!");
    list_free(list_p);
}


static void
test_list_print(const void* value_p)
{
    const int* number_p = value_p;
    printf("%10d\n", *number_p);
}

static float
test_list_value(const void* value_p)
{
    return (float) *(const int*) value_p;
}
