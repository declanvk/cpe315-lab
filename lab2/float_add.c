#include "float_add.h"

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "unpack_float.h"
#include "pack_float.h"
#include "norm_float.h"

/*
    Part 5 - Manual floating point addition using previously created functions.
*/
void question_5()
{
    int op_a[] = { 0xBF800000, 0x3F800000, 0x40800000 };
    int op_b[] = { 0x3F800000, 0x3E800000, 0xBE000000 };
    int ops_len = sizeof(op_a) / sizeof(op_a[0]);
    int arg_idx;
    float result = 0.0f;

    printf("=========Part 5==========\n");
    for (arg_idx = 0; arg_idx < ops_len; arg_idx++)
    {
        result = fadd(TYPE_CAST(float, op_a[arg_idx]), TYPE_CAST(float, op_b[arg_idx]));
        printf("5%c.  0x%x and 0x%x (%.3f and %.3f)\n  Sum:0x%x  (%.3f)\n",
            'a' + arg_idx, op_a[arg_idx], op_b[arg_idx],
            TYPE_CAST(float, op_a[arg_idx]), TYPE_CAST(float, op_b[arg_idx]),
            TYPE_CAST(int, result), result);
    }
    printf("=========================\n");
}

/*
    After extracting the exponents and fractions from the floating
    point representation and initializing the result, pass the values
    to a helper add method that performs the adding. After the result
    comes back normalize it and free the extracted arguments, create
    a float and return it.
*/
float fadd(float a, float b)
{
    float f_result = 0.0f;
    INTFLOAT_PTR a_extract = (INTFLOAT_PTR) malloc(sizeof(INTFLOAT));
    INTFLOAT_PTR b_extract = (INTFLOAT_PTR) malloc(sizeof(INTFLOAT));
    INTFLOAT_PTR result = (INTFLOAT_PTR) malloc(sizeof(INTFLOAT));

    extract_float(a_extract, a);
    extract_float(b_extract, b);
    extract_float(result, 0.0f);

    intfloat_add(a_extract, b_extract, result);

    normalize(result);
    f_result = create_float(result);

    free(a_extract);
    free(b_extract);
    free(result);

    return f_result;
}

/*
    This method performs addition between two integer representations 
    of a floating point number. The first step is to find the larger
    exponent and normalize the smaller one's fraction with respect to
    the difference between the exponents. Then set the exponents equal
    to the highest value. After that protect the fraction values from 
    overflow by giving them buffer bit and updating the exponents accordingly.
    After that the fractions can be added together without any extra
    manipulation and the result is stored.
*/
void intfloat_add(INTFLOAT_PTR a, INTFLOAT_PTR b, INTFLOAT_PTR result)
{
    int exp_diff = 0;
    if (a->exponent < b->exponent)
    {
        exp_diff = b->exponent - a->exponent;
        a->fraction >>= exp_diff;
    }
    else if (a->exponent > b->exponent)
    {
        exp_diff = a->exponent - b->exponent;
        b->fraction >>= exp_diff;
    }

    a->exponent = b->exponent = (a->exponent > b->exponent) ? (a->exponent) : (b->exponent);

    // Overflow prevention
    a->fraction >>= 1;
    a->exponent++;
    b->fraction >>= 1;
    b->exponent++;

    result->fraction = a->fraction + b->fraction;
    result->exponent = a->exponent; // equal to b->exponent
}
