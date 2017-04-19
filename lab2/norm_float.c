#include "norm_float.h"

#include <stdio.h>
#include <stdlib.h>

void question_4()
{
    int arg_idx;
    int test_exps[] = {  0x00000001, 0x00000000, 0x0000000C, 0xFFFFFFF8 };
    int test_fracs[] = { 0x40000000, 0x00200000, 0xFFC00000, 0x02000000 };
    INTFLOAT_PTR result = (INTFLOAT_PTR) malloc(sizeof(INTFLOAT));

    printf("=========Part 4==========\n");
    for (arg_idx = 0; arg_idx < (sizeof(test_exps) / sizeof(test_exps[0])); arg_idx++)
    {
        *result = (INTFLOAT) {0};
        result->fraction = test_fracs[arg_idx];
        result->exponent = test_exps[arg_idx];

        normalize(result);

        printf("4%c.  Exp = %d, Frac = 0x%x\n", 'a' + arg_idx, result->exponent, result->fraction);
    }
    printf("=========================\n");
}

void normalize(INTFLOAT_PTR x)
{
    if (x->fraction == 0)
    {
        x->exponent = 0;
        return;
    }

    while (((x->fraction & 0x80000000) ^ ((x->fraction & 0x40000000) << 1)) == 0)
    {
        x->fraction <<= 1;
        x->exponent--;
    }
}
