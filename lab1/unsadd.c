#include "unsadd.h"

#include <stdio.h>

/*
    This part only needs additions and a correspong cast to truncate the results down.
*/
void lab1_part3()
{
    printf("Part 3: Unsigned Addition of Small Fields\n");
    printf("========================\n");
    printf("0x20 + 0x35 = 0x%hhx\n", (unsigned char) (0x20 + 0x35));
    printf("0x80 + 0x7F = 0x%hhx\n", (unsigned char) (0x80 + 0x7F));
    printf("0x80 + 0xFF = 0x%hhx\n", (unsigned char) (0x80 + 0xFF));
    printf("0xFF + 0x01 = 0x%hhx\n", (unsigned char) (0xFF + 0x01));
    printf("========================\n");
}
