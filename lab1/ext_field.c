#include "ext_field.h"

#include <stdio.h>
#include <string.h>
#include <limits.h>

/*
    This part is about implementing an arbitrary width adder and testing
    it with prepared inputs. 
*/
void lab1_part6()
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
    unsigned char result[5];
    int size = 5;

    printf("Part 6: Performing Extended Field Arithmetic\n");
    printf("========================\n");

    printf("6a)\n");
    output_add(result, test1a, test1b, size);

    printf("6b)\n");
    output_add(result, test2a, test2b, size);


    printf("========================\n");
}

/*
    Outputs to stdout the addition of the two passed operands in the specified format
    and output the carry bit after the completion of the additions. Also clears the 
    result array to zero before additions are performed.
*/
void output_add(
    unsigned char *result,
    unsigned char *a1,
    unsigned char *a2,
    int size)
{
    int idx;
    unsigned int carry = 0;

    memset(result, 0, size * sizeof(unsigned char));

    for (idx = 0; idx < size; idx++)
    {
        printf("\t0x%x", a1[idx]);
    }
    printf("\n");

    printf("+");
    for (idx = 0; idx < size; idx++)
    {
        printf("\t0x%x", a2[idx]);
    }
    printf("\n");

    carry = arbitrary_byte_add(result, a1, a2, size, carry);

    printf("=");
    for (idx = 0; idx < size; idx++)
    {
        printf("\t0x%x", result[idx]);
    }
    printf("\n\n");

    printf("Carry Out = %d\n", carry);
}

/*
    For each byte in the operands add them together and set the carry 
    equal to the amount the operation overflowed. Return the last
    carry out of the function.
*/
unsigned int arbitrary_byte_add(
    unsigned char *result,
    unsigned char *a1,
    unsigned char *a2,
    int size,
    unsigned int carry_in)
{
    int idx;
    unsigned int carry = carry_in;
    for (idx = size - 1; idx >= 0; idx--)
    {
        result[idx] = (unsigned char) (a1[idx] + a2[idx] + carry);
        carry = detect_carry(a1[idx], a2[idx], carry);
    }

    return carry;
}

/*
    Detects if a carry occured by seeing if the addition result is
    less than the unrestricted addition. If that is the case, subtracts
    the maximum value for the constrained type from the unconstrained result.
*/
int detect_carry(unsigned char a, unsigned char b, unsigned char c)
{
    unsigned char result = a + b + c;
    
    if (result < a + b + c)
    {
        return (a + b + c) - UCHAR_MAX;
    }
    else
    {
        return 0;
    }
}
