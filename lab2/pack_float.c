#include "pack_float.h"

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "unpack_float.h"

/*
    Part 3 - Taking the int float struct, recreate the float is was
    unpacked from.
*/
void question_3()
{
    int arg_idx;
    int tests[] = { 0x40C80000, 0xC3000000, 0x3E000000, 0x3EAAAAAB };
    INTFLOAT_PTR mid_result = (INTFLOAT_PTR) malloc(sizeof(INTFLOAT));
    float input = 0;
    float result = 0.0f;

    printf("=========Part 3==========\n");
    for (arg_idx = 0; arg_idx < (sizeof(tests) / sizeof(tests[0])); arg_idx++)
    {
        *mid_result = (INTFLOAT) {0};

        input = TYPE_CAST(float, tests[arg_idx]);
        extract_float(mid_result, input);

        result = create_float(mid_result);
        printf("3%c.  Test case: 0x%x\n  Float: %f\n", 'a' + arg_idx, tests[arg_idx], result);
    }    
    printf("=========================\n");
}

/*
    Zero the initial and result float value, then test the input
    fraction's sign for the sign bit. If the fraction is not zero,
    then shift off the sign bit and hidden bit, and decrement the 
    exponent to account for the loss of the hidden bit. Set the 
    fraction, and the exponent after accounting for th bias.
*/
float create_float(INTFLOAT_PTR extracted)
{
    float_components components;

    components.value = 0.0f;

    components.fields.sign = extracted->fraction < 0;
    if (extracted->fraction != 0)
    {
        extracted->fraction <<= 2;
        extracted->exponent--;

        components.fields.frac = extracted->fraction >> 9; // Adjust for bitfield
        components.fields.exp = extracted->exponent + 127;
    }

    return components.value;
}
