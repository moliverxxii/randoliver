/*
 * performance.c
 *
 *  Created on: 9 févr. 2026
 *      Author: moliver
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "performance.h"

const performance_t PERFORMANCE_NULL =
{
   NULL,
   0, 0, 0, 0
};

performance_t
performance_init(const char* name_p)
{
    performance_t performance = PERFORMANCE_NULL;
    char* performance_name_p = malloc(strlen(name_p) + 1);
    strcpy(performance_name_p, name_p);
    performance.name_p = performance_name_p;
    return performance;
}


void
performance_free(performance_t* perfomance_p)
{
    free(perfomance_p->name_p);
    *perfomance_p = PERFORMANCE_NULL;
}

void
performance_try_start(performance_t* performance_p)
{
    performance_p->try_start = clock();
}

void
performance_try_add(performance_t* performance_p)
{
    performance_p->try_end = clock();
    clock_t operation_time = performance_p->try_end - performance_p->try_start;
    uint64_t operation_time_ms = (operation_time * MILLISECONDS_PER_SECOND)/CLOCKS_PER_SEC;
    performance_p->total_time += operation_time_ms;
    ++performance_p->total_operations;
}

void
performance_print(const performance_t* performance_p)
{
    printf("%s: %8.2f ms/operation\n",
           performance_p->name_p,
           (float) performance_p->total_time/performance_p->total_operations);
}



