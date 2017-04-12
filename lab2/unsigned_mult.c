#include "unsigned_mult.h"

#include <stdio.h>

#define LSB(x) (0x1 & x)

void question_1()
{
    unsigned int multiplicands[] = { 0x0001, 0x0001, 0x8000, 0x4000, 0x8000 };
    unsigned int multipliers[]   = { 0x0001, 0xFFFF, 0x0001, 0x4000, 0x8000 };
    int mult_idx;

    printf("=========Part 1========== \n");
    for (mult_idx = 0; mult_idx < 5; mult_idx++)
    {
        printf("1%c. 0x%04x * 0x%04x = 0x%08x\n", 'a' + mult_idx,
            multiplicands[mult_idx], multipliers[mult_idx],
            umultiply(multiplicands[mult_idx], multipliers[mult_idx]));
    }
    printf("=========================\n");
}

unsigned int umultiply(unsigned int multiplicand, unsigned int multiplier)
{
    int shift_idx;
    unsigned int result = 0;

    for (shift_idx = 0; shift_idx < 16; shift_idx++)
    {
        if (LSB(multiplier))
        {
            result += multiplicand << shift_idx;
        }

        multiplier = multiplier >> 1;
    }

    return result;
}
