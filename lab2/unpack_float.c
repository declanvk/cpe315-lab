#include "unpack_float.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void question_2()
{
    int arg_idx;
    int tests[] = { 0x40C80000, 0xC3000000, 0x3E000000, 0x3EAAAAAB };
    INTFLOAT_PTR result = (INTFLOAT_PTR) malloc(sizeof(INTFLOAT));
    float input = 0;

    printf("=========Part 2==========\n");
    for (arg_idx = 0; arg_idx < (sizeof(tests) / sizeof(tests[0])); arg_idx++)
    {
        *result = (INTFLOAT) {0};

        input = TYPE_CAST(float, tests[arg_idx]);
        extract_float(result, input);
        printf("2%c. Test case: 0x%x\n  Float: %f\n  Exponent: %d\n  Fraction: 0x%x\n",
            'a' + arg_idx, tests[arg_idx], input,
            result->exponent, result->fraction);
    }
    printf("=========================\n");

    free(result);
}

void extract_float(INTFLOAT_PTR x, float f)
{
    float_components components;

    if (f == 0)
    {
        x->exponent = 0;
        x->fraction = 0;

        return;
    }

    components.value = f;

    x->exponent = components.fields.exp - 127;

    x->fraction = components.fields.frac >> 2;
    x->exponent += 1;
    x->fraction = 0x40000000 | (0x3fffffff & x->fraction);

    if (components.fields.sign)
    {
        x->fraction = NEGATE_TWOS(x->fraction);
    }
}
