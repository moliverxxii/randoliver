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
    SOLID_FILE_LINE_NONE = 0,
    SOLID_FILE_LINE_VERTEX,
    SOLID_FILE_LINE_FACE
};

static int solid_file_is_key(const char* line_p, const char* key_p);

solid_t*
solid_file_open(const char* file_name_p)
{
    FILE* file_p = fopen(file_name_p, "r");

    solid_t* solid_p = NULL;

    if(file_p != NULL)
    {
        static const char* const KEY_SOLID = "solid";
        static const char* const KEY_FACES = "faces";

        char line[SOLID_LINE_LENGTH + 1] = {0};

        int      is_ok        = 1;
        uint32_t vertex_count = 0;
        uint32_t face_count   = 0;
        list_t* list_p = NULL;
        vector_t* vertices_p = NULL;
        uint32_t **vertex_index_p = NULL;
        enum solid_file_line_e line_type = SOLID_FILE_LINE_NONE;

        do
        {
            const char* read_result_p = fgets(line, SOLID_LINE_LENGTH, file_p);

            if(read_result_p == NULL)
            {
                is_ok = 0;
            }
            else if(solid_file_is_key(line, KEY_SOLID))
            {
                line_type = SOLID_FILE_LINE_VERTEX;
                printf("solid\n");
                if(list_p != NULL)
                {
                    break;
                }
            }
            else if(solid_file_is_key(line, KEY_FACES))
            {
                line_type = SOLID_FILE_LINE_FACE;
                printf("%u vertices\n", vertex_count);
                if(list_p == NULL)
                {
                    break;
                }
                else
                {
                    vertices_p = list_array(list_p);
                    list_free(list_p);
                    list_p = NULL;
                }
            }
            else switch(line_type)
            {
            case SOLID_FILE_LINE_VERTEX:
                {
                    vector_t vector = VECTOR_0;
                    int conversion_count = sscanf(line,
                                                  "%f %f %f",
                                                  &vector.x,
                                                  &vector.y,
                                                  &vector.z);
                    if(conversion_count == VECTOR_AXIS_COUNT)
                    {
                        list_append(&list_p, &vector, sizeof(vector));
                        vertex_count++;
                    }
                }
                break;
            case SOLID_FILE_LINE_FACE:
                {
                    uint32_t faces[TRIANGLE_POINT_COUNT] = {0, 0, 0};
                    int conversion_count = sscanf(line,
                                                  "%u %u %u",
                                                  &faces[0],
                                                  &faces[1],
                                                  &faces[2]);

                    if(conversion_count == TRIANGLE_POINT_COUNT)
                    {
                        uint32_t* element_p = malloc(sizeof(faces));
                        memcpy(element_p, faces, sizeof(faces));
                        list_append(&list_p, &element_p, sizeof(element_p));
                        face_count++;
                    }
                }
                break;
            default:
                break;
            }

        } while(is_ok);

        if(list_p != NULL)
        {
            printf("%u faces\n", face_count);

            vertex_index_p = list_array(list_p);
            list_free(list_p);
        }

        solid_p = solid_init(vertex_count, vertices_p,
                             face_count, (const uint32_t**) vertex_index_p);

        for(uint32_t face = 0; face < face_count; ++face)
        {
            free(vertex_index_p[face]);
        }
        free(vertex_index_p);

        //lecture points

        fclose(file_p);
        //fabrication des triangles
    }


    return solid_p;
}

static int
solid_file_is_key(const char* line_p, const char* key_p)
{
    return strncmp(line_p, key_p, strlen(key_p)) == 0;
}
