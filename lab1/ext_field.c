#include <stdio.h>

int detect_carry(unsigned char a, unsigned char b)
{
    unsigned char result = a + b;

    if (result < a + b)
    {
        return (a + b) - result;
    }
    else
    {
        return 0;
    }
}

unsigned int arbitrary_byte_add(
    unsigned char *result,
    unsigned char *a1,
    unsigned char *a2,
    int size,
    unsigned int carry_in)
{
    int idx;
    unsigned int carry = carry_in;
    for (idx = 0; i < size; idx++)
    {
        
    }
}

int main(int argc, char const *argv[])
{
    unsigned char test1a[] = {
        0x44, 0x00, 0x00, 0x00, 0x01
    };
    unsigned char test1b[] = {
        0x30, 0xFF, 0xFF, 0x00, 0x0
    };

    unsigned char test2a[] = {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };
    unsigned char test2b[] = {
        0x00, 0x00, 0x00, 0x00, 0x01
    };
    unsigned int carry = 0;
    int size = 5;

    unsigned char result[5];

    return 0;
}