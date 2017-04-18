#include "float_add.h"

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "unpack_float.h"
#include "pack_float.h"
#include "norm_float.h"

void question_5()
{
    printf("=========Part 5==========\n");
    printf("=========================\n");
}

float fadd(float a, float b)
{
    INTFLOAT_PTR a_extract = (INTFLOAT_PTR) malloc(sizeof(INTFLOAT));
    INTFLOAT_PTR b_extract = (INTFLOAT_PTR) malloc(sizeof(INTFLOAT));
    INTFLOAT_PTR result = (INTFLOAT_PTR) malloc(sizeof(INTFLOAT));

    extract_float(a_extract, a);
    extract_float(b_extract, b);
    extract_float(result, 0.0f);

    intfloat_add(a_extract, b_extract, result);

    normalize(result);

    return create_float(result);
}

void intfloat_add(INTFLOAT_PTR a, INTFLOAT_PTR b, INTFLOAT_PTR result)
{

}
