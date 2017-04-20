#include "float_sub.h"

#include <stdio.h>

#include "float_add.h"

/*
    Part 6 - Manual floating point subtraction
*/
void question_6()
{
    int op_a[] = {  0x40400000, 0x40400000, 0x40000000 };
    int op_b[] = { 0x3F800000, 0xBF800000, 0x40000000 };
    int ops_len = sizeof(op_a) / sizeof(op_a[0]);
    int arg_idx;
    float result = 0.0f;

    printf("=========Part 6==========\n");
    for (arg_idx = 0; arg_idx < ops_len; arg_idx++)
    {
        result = fsub(TYPE_CAST(float, op_a[arg_idx]), TYPE_CAST(float, op_b[arg_idx]));
        printf("5%c.  0x%x and 0x%x (%.3f - (%.3f))\n  Diff:0x%x  (%.3f)\n",
            'a' + arg_idx, op_a[arg_idx], op_b[arg_idx],
            TYPE_CAST(float, op_a[arg_idx]), TYPE_CAST(float, op_b[arg_idx]),
            TYPE_CAST(int, result), result);
    }
    printf("=========================\n");
}

/*
    No need to reimplement all the same structures, negating the 
    second operand is sufficient.
*/
float fsub(float a, float b)
{
    return fadd(a, -b);
}
