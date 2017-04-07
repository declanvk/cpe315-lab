#include "sadd_over.h"

#include <stdio.h>

void lab1_part5()
{
    printf("Part 5: Signed Addition and Overflow\n");
    printf("========================\n");
    printf("0x20 + 0x35 Overflow: %d\n", detect_overflow(0x20, 0x35));
    printf("0x80 + 0x7F Overflow: %d\n", detect_overflow(0x80, 0x7F));
    printf("0x80 + 0xFF Overflow: %d\n", detect_overflow(0x80, 0xFF));
    printf("0xFF + 0x01 Overflow: %d\n", detect_overflow(0xFF, 0x01));
    printf("========================\n");
}

int detect_overflow(signed char a, signed char b)
{
    signed char result = a + b;

    if (a > 0 && b > 0 && result < 0)
    {
        return 1;
    }

    if (a < 0 && b < 0 && result > 0)
    {
        return 1;
    }

    return 0;
}
