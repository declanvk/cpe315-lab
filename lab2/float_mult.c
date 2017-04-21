#include "float_mult.h"

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "unpack_float.h"
#include "pack_float.h"
#include "norm_float.h"

/*
    Part 7 - manual floating point multiplication
*/
void question_7()
{
    int op_a[] = { 0x40200000, 0xc1700000 };
    int op_b[] = { 0x40200000, 0x45800000 };
    int ops_len = sizeof(op_a) / sizeof(op_a[0]);
    int arg_idx;
    float result = 0.0f;

    printf("=========Part 7==========\n");
    for (arg_idx = 0; arg_idx < ops_len; arg_idx++)
    {
        result = fmul(TYPE_CAST(float, op_a[arg_idx]), TYPE_CAST(float, op_b[arg_idx]));
        printf("5%c.  0x%x and 0x%x (%.3f x %.3f)\n  Product:0x%x  (%.3f)\n",
            'a' + arg_idx, op_a[arg_idx], op_b[arg_idx],
            TYPE_CAST(float, op_a[arg_idx]), TYPE_CAST(float, op_b[arg_idx]),
            TYPE_CAST(int, result), result);
    }
    printf("=========================\n");
}

/*
    After extracting the exponents and fractions from the floating
    point representation and initializing the result, pass the values
    to a helper multiplication method that performs the adding. 
    After the result comes back normalize it and freeing the extracted
    arguments, create a float and return it.
*/
float fmul(float a, float b)
{
    float f_result = 0.0f;
    INTFLOAT_PTR a_extract = (INTFLOAT_PTR) malloc(sizeof(INTFLOAT));
    INTFLOAT_PTR b_extract = (INTFLOAT_PTR) malloc(sizeof(INTFLOAT));
    INTFLOAT_PTR result = (INTFLOAT_PTR) malloc(sizeof(INTFLOAT));

    extract_float(a_extract, a);
    extract_float(b_extract, b);
    extract_float(result, 0.0f);

    intfloat_mul(a_extract, b_extract, result);

    normalize(result);
    f_result = create_float(result);

    free(a_extract);
    free(b_extract);
    free(result);

    return f_result;
}

/*
    
*/
void intfloat_mul(INTFLOAT_PTR a, INTFLOAT_PTR b, INTFLOAT_PTR result)
{
    int64_t product = TYPE_CAST(int64_t, a->fraction) * TYPE_CAST(int64_t, b->fraction);

    product &= 0xffffffff00000000;
    result->fraction = product >> 32;

    result->exponent = a->exponent + b->exponent;
}
