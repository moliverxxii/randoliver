#include <stdio.h>

#include "operator.h"
#include "preset.h"
#include "vector.h"

static void oli_test_vectors();

void preset_add_vectors()
{
    preset_add("test vectors",
               &oli_test_vectors,
               NULL,
               NULL,
               NULL,
               1);
}

static void
oli_test_vectors()
{
    printf("operator init");
    operator_t* a_p = operator_init_null();
    operator_print(a_p);
    operator_free(a_p);

    a_p = operator_init_id();
    operator_print(a_p);
    operator_free(a_p);

    a_p = operator_init_translation(vector_init(0.5, 0.5, 0.5));
    operator_print(a_p);

    operator_t* b_p = operator_init_id();
    operator_print(b_p);

    printf("operator row\n");
    for(vector_axis_e row = 0; row < VECTOR_AXIS_COUNT; ++row)
    {
        vector_print(operator_row_get(b_p, row));
    }

    printf("operator column\n");
    for(vector_axis_e column = 0; column < VECTOR_AXIS_COUNT; ++column)
    {
        vector_print(operator_column_get(b_p, column));
    }

    printf("new_table\n");
    vector_t table[3] =
    {
        vector_init(1,2,3),
        vector_init(4,5,6),
        vector_init(7,8,9)
    };
    for(int vector = 0; vector < 3; ++vector)
    {
        vector_print(table[vector]);
    }

    printf("vector scalar\n"
           "3D %f\n"
           "4D %f\n",
           vector_scalar(table[0], VECTOR_X),
           vector_scalar_full(table[0], VECTOR_X));

    printf("operator operation\n");
    vector_t* result_p = operator_operation(b_p, table, 3);

    for(int vector = 0; vector < 3; ++vector)
    {
        vector_print(result_p[vector]);
    }
    free(result_p);

    printf("operator translation\n");
    result_p = operator_operation(a_p, table, 3);

    for(int vector = 0; vector < 3; ++vector)
    {
        vector_print(result_p[vector]);
    }
    free(result_p);


    printf("operator multiply translation x id\n");
    operator_t* c_p = operator_multiply(a_p, b_p);
    operator_print(c_p);

    operator_free(a_p);
    operator_free(b_p);
    operator_free(c_p);
}

