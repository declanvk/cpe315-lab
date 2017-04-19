#include "unpack_float.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
    Part 2 - Unpack IEEE 754 floating point single precision number into
    int, float struct.
*/
void question_2()
{
    int arg_idx;
    int tests[] = { 0x40C80000, 0xC3000000, 0x3E000000, 0x3EAAAAAB, 0xC1080000 };
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

/*
    Checking if the input is zero for a special case,
    set the union's float value to the value to decompose. Using
    the union allows direct named access to specific bits in the 
    float. Extract the exponent and remove the bias. Extract the 
    fraction shifted over 2 places, and add 1 to the exponent to 
    account for one of those shifts. The other place in the shift
    is reserved for the sign bit. Then set the hidden bit, and clear
    the sign bit. If the number turned out to be negative, set the
    fraction to the the two's complement negation.
*/
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
