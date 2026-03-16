/*
 * solid_file.c
 *
 *  Created on: 13 mars 2026
 *      Author: moliver
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "solid_file.h"
#include "list.h"

#define SOLID_LINE_LENGTH 50

enum solid_file_line_e
{
    SOLID_FILE_LINE_VERTEX = 0,
    SOLID_FILE_LINE_FACE,
    SOLID_FILE_LINE_COUNT,
    SOLID_FILE_LINE_NONE,
};

static const char* const SOLID_FILE_LINE_KEY_TABLE[SOLID_FILE_LINE_COUNT] =
{
    "solid",
    "faces"
};

struct solid_constructor_t
{
    vector_t* vertices_p;
    uint32_t vertex_count;
    uint32_t** vertex_index_p;
    uint32_t face_count;
    list_t* list_p;
};

static int solid_file_is_key(const char* line_p, const char* key_p);
static enum solid_file_line_e solid_file_parse_line_type(const char* line_p);
static int solid_file_parse_step_title(enum solid_file_line_e line_type,
                                       struct solid_constructor_t* solid_struct_p);
static void solid_file_parse_step_content(enum solid_file_line_e line_type,
                                          const char* line_p,
                                          struct solid_constructor_t* solid_struct_p);


solid_t*
solid_file_open(const char* file_name_p)
{
    FILE* file_p = fopen(file_name_p, "r");

    solid_t* solid_p = NULL;

    struct solid_constructor_t solid_struct =
    {
        NULL,
        0,
        NULL,
        0,
        NULL,
    };

    if(file_p != NULL)
    {
        char line[SOLID_LINE_LENGTH + 1] = {0};
        int is_ok = 1;
        enum solid_file_line_e line_type = SOLID_FILE_LINE_NONE;

        do
        {
            const char* read_result_p = fgets(line, SOLID_LINE_LENGTH, file_p);

            if(read_result_p == NULL)
            {
                is_ok = 0;
            }
            else
            {
                line_type = solid_file_parse_line_type(line);

                int is_new_step_found = solid_file_parse_step_title(line_type, &solid_struct);

                if(is_new_step_found)
                {
                    continue;
                }

                solid_file_parse_step_content(line_type, line, &solid_struct);

            }
        } while(is_ok);

        if(solid_struct.list_p != NULL)
        {
            printf("%u faces\n", solid_struct.face_count);

            solid_struct.vertex_index_p = list_array(solid_struct.list_p);
            list_free(solid_struct.list_p);
        }

        solid_p = solid_init(solid_struct.vertex_count,
                             solid_struct.vertices_p,
                             solid_struct.face_count,
                             (const uint32_t**) solid_struct.vertex_index_p);

        for(uint32_t face = 0; face < solid_struct.face_count; ++face)
        {
            free(solid_struct.vertex_index_p[face]);
        }
        free(solid_struct.vertex_index_p);
        free(solid_struct.vertices_p);


        fclose(file_p);
    }

    return solid_p;
}


static int
solid_file_is_key(const char* line_p, const char* key_p)
{
    return strncmp(line_p, key_p, strlen(key_p)) == 0;
}

static enum solid_file_line_e
solid_file_parse_line_type(const char* line_p)
{
    enum solid_file_line_e line_type = SOLID_FILE_LINE_NONE;

    for(int index = 0; index < SOLID_FILE_LINE_COUNT; ++index)
    {
        if(solid_file_is_key(line_p, SOLID_FILE_LINE_KEY_TABLE[index]))
        {
            line_type = index;
        }
    }
    return line_type;
}

static int
solid_file_parse_step_title(enum solid_file_line_e line_type,
                            struct solid_constructor_t* solid_struct_p)
{
    int is_new_step_found = 0;
    switch(line_type)
    {
    case SOLID_FILE_LINE_VERTEX:
        printf("solid\n");
        if(solid_struct_p->list_p != NULL)
        {
            //nouveau solide mais une operation est en cours;
            //erreur
        }
        is_new_step_found = 1;
        break;
    case SOLID_FILE_LINE_FACE:
        line_type = SOLID_FILE_LINE_FACE;
        if(solid_struct_p->list_p == NULL)
        {
            //on passe aux faces mais nous n'avons pas de sommets!
            //erreur
        }
        else
        {
            printf("%u vertices\n", solid_struct_p->vertex_count);
            solid_struct_p->vertices_p = list_array(solid_struct_p->list_p);
            list_free(solid_struct_p->list_p);
            solid_struct_p->list_p = NULL;
        }
        is_new_step_found = 1;
        break;
    default:
        break;

    }

    return is_new_step_found;
}
static void
solid_file_parse_step_content(enum solid_file_line_e line_type,
                              const char* line_p,
                              struct solid_constructor_t* solid_struct_p)
{

    switch(line_type)
    {
    case SOLID_FILE_LINE_VERTEX:
        //lecture points
        {
            vector_t vector = VECTOR_0;
            int conversion_count = sscanf(line_p,
                                          "%f %f %f",
                                          &vector.x,
                                          &vector.y,
                                          &vector.z);
            if(conversion_count == VECTOR_AXIS_COUNT)
            {
                list_append(&solid_struct_p->list_p, &vector, sizeof(vector));
                solid_struct_p->vertex_count++;
            }
        }
        break;
    case SOLID_FILE_LINE_FACE:
        //fabrication des triangles
        {
            uint32_t faces[TRIANGLE_POINT_COUNT] = {0, 0, 0};
            int conversion_count = sscanf(line_p,
                                          "%u %u %u",
                                          &faces[0],
                                          &faces[1],
                                          &faces[2]);

            if(conversion_count == TRIANGLE_POINT_COUNT)
            {
                uint32_t* element_p = malloc(sizeof(faces));
                memcpy(element_p, faces, sizeof(faces));
                list_append(&solid_struct_p->list_p, &element_p, sizeof(element_p));
                solid_struct_p->face_count++;
            }
        }
        break;
    default:
        break;
    }
}


