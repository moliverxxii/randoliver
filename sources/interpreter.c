/*
 * interpreter.c
 *
 *  Created on: 8 févr. 2024
 *      Author: moliver
 */

#include <stdlib.h>
#include <string.h>

#include "interpreter.h"

typedef struct
{
    const char* file_name_p;
    FILE* file_p;
} command_file_t;

const char* const PARAMETERS[] =
{
    "camera",
    "screen",
    "name",
    "frames",
    "size",
    "process"
};

static command_file_t* command_file_p = NULL;

static command_file_t* command_file_init(const char* file_name_p);

void
interpreter_parse_arguments(int argc, char* argv[])
{
    if(argc > 1)
    {
        command_file_p = command_file_init(argv[1]);
    }
}

static command_file_t*
command_file_init(const char* file_name_p)
{
    char* command_file_name_p = malloc((strlen(file_name_p) + 1) * sizeof(char));
    strcpy(command_file_name_p, file_name_p);
    FILE* file_p = fopen(command_file_name_p, "r");

    command_file_t* new_command_file_p = malloc(sizeof(command_file_t));
    *new_command_file_p = (command_file_t) {command_file_name_p, file_p};
    return new_command_file_p;
}

