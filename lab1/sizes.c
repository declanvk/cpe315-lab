#include "sizes.h"

#include <stdio.h>

/*
    Uses builtin function to return the byte sizes of each data type.
    Prints sizes to stdout
*/
void lab1_part1()
{
    printf("Part 1: Data Types and their Sizes\n");
    printf("========================\n");
    printf("Byte Size: %lu\n", sizeof(char));
    printf("Short Int Size: %lu\n", sizeof(short));
    printf("Integer Size: %lu\n", sizeof(int));
    printf("Long Integer Size: %lu\n", sizeof(long));
    printf("========================\n");
}
