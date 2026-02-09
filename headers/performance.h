/*
 * performance.h
 *
 *  Created on: 9 févr. 2026
 *      Author: moliver
 */
//bibliotheque pour mesurer les performances d'executions, sur boucles en
//particulier.
#ifndef HEADERS_PERFORMANCE_H_
#define HEADERS_PERFORMANCE_H_

#include <stdint.h>
#include <time.h>

#define MILLISECONDS_PER_SECOND 1000

typedef struct
{
    char* name_p;
    uint64_t total_operations;
    uint64_t total_time; //ms
    clock_t try_start;
    clock_t try_end;
} performance_t;

performance_t performance_init(const char* name_p);
void performance_free(performance_t* performance_p);

void performance_try_start(performance_t* performance_p);
void performance_try_add(performance_t* performance_p);
void performance_print(const performance_t* performance_p);

#endif /* HEADERS_PERFORMANCE_H_ */
