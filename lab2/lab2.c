#include "lab2.h"
#include "float_add.h"
#include "float_sub.h"
#include "norm_float.h"
#include "unpack_float.h"
#include "float_mult.h"
#include "lab2.h"
#include "pack_float.h"
#include "unsigned_mult.h"

#include <stdio.h>
#include <stdlib.h>

void (*questions[7]) (void) = {
    &question_1,
    &question_2,
    &question_3,
    &question_4,
    &question_5,
    &question_6,
    &question_7
};

int main(int argc, char const *argv[])
{
    int question_idx, limit = (sizeof(questions) / sizeof(questions[0]));

    if (argc == 1)
    {
        for (question_idx = 0; question_idx < limit; question_idx++)
        {
            questions[question_idx]();
            
            if (question_idx != limit - 1)
            {
                printf("\n");
            }
        }
    }
    else if (argc == 2)
    {
        if ((question_idx = atoi(argv[1])) != 0)
        {
            question_idx -= 1;

            if (0 <= question_idx && question_idx < limit)
            {
                questions[question_idx]();
            }
            else
            {
                fprintf(stderr, "n must be in range 1-7\n");

                return EXIT_FAILURE;
            }
        }
        else
        {
            fprintf(stderr, "usage: %s [n]\n", argv[0]);

            return EXIT_FAILURE;
        }
    }
    else
    {
        fprintf(stderr, "usage: %s [n]\n", argv[0]);

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
