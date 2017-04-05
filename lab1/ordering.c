#include <stdio.h>

unsigned char bytestring[] = {
0x41, // 0100 0001 - 0x82
0x33, // 0011 0011 - 0xCC
0x54, // 0101 0100 - 0x2A
0x80, // 1000 0000 - 0x01
0xFF, // 1111 1111 - 0xFF
0x99, // 1001 1001 - 0x99
0x01, // 0000 0001 - 0x80
0x78, // 0111 1000 - 0x1E
0x55, // 0101 0101 - 0xAA
0x20, // 0010 0000 - 0x04
0xFE, // 1111 1110 - 0x7F
0xEE, // 1110 1110 - 0x77
0x00, // 0000 0000 - 0x00
0x00, // 0000 0000 - 0x00
0x00, // 0000 0000 - 0x00
0x00  // 0000 0000 - 0x00
}; 

int main(int argc, char const *argv[])
{
    short int *short_int_ptr = (short int *) bytestring;
    int *int_ptr = (int *) bytestring;
    long int *long_int_ptr = (long int *) bytestring;

    int byte_array_idx;

    printf("Little Endian:\n");

    printf("2a.  Byte Values: ");
    for (byte_array_idx = 0; byte_array_idx < 16; byte_array_idx++)
    {
        printf("%d ", bytestring[byte_array_idx]);
    }
    printf("\n");

    printf("2b. Short Values: ");
    for (byte_array_idx = 0; byte_array_idx < 4; byte_array_idx++)
    {
        printf("%d ", *(short_int_ptr + byte_array_idx));
    }
    printf("\n");

    printf("2c.   Int Values: ");
    for (byte_array_idx = 0; byte_array_idx < 2; byte_array_idx++)
    {
        printf("%d ", *(int_ptr + byte_array_idx));
    }
    printf("\n");

    printf("2d.  Long Values: ");
    for (byte_array_idx = 0; byte_array_idx < 1; byte_array_idx++)
    {
        printf("%ld ", *(long_int_ptr + byte_array_idx));
    }
    printf("\n");

    printf("Big Endian:\n");

    printf("2e.  Byte Values: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
        0x82, 0xCC, 0x2A, 0x01, 0xFF, 0x99, 0x80, 0x1E, 0xAA, 0x04, 0x7F, 0x77, 0x00, 0x00, 0x00, 0x0);
    printf("2f. Short Values: %d %d %d %d\n", 0xCC82, 0x012A, 0x99FF, 0x1E80);
    printf("2g.   Int Values: %d %d\n", (int) 0x012ACC82, (int) 0x1E8099FF);
    printf("2h.  Long Values: %ld\n", (long) 0x1E8099FF012ACC82);

    return 0;
}
