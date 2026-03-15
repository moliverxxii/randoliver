/*
 * engine.c
 *
 *  Created on: 9 nov. 2025
 *      Author: moliver
 */
#include <stdlib.h>
#include <string.h>

#include "engine.h"

typedef struct engine_t
{
    int argc;
    const char** argv;
    const char* input_name_p;
    const char* output_name_p;
} engine_t;

engine_t*
engine_init(int argc, const char* argv[])
{
    engine_t* engine_p = malloc(sizeof(engine_t));

    if(engine_p != NULL)
    {
        char** arguments_p = malloc(argc * sizeof(const char*));

        if(arguments_p != NULL)
        {
            for(int argument = 0; argument < argc; ++argument)
            {
                arguments_p[argument] = malloc(strlen(argv[argument]) + 1);
                strcpy(arguments_p[argument], argv[argument]);
            }

            engine_t engine =
            {
                argc,
                (const char**) arguments_p,
                NULL,
                NULL
            };

            *engine_p = engine;
        }
        else
        {
            free(engine_p);
            engine_p = NULL;
        }
    }

    return NULL;
}

void
engine_free(engine_t* engine_p)
{
    free(engine_p);
}

