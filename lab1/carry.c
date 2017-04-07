#include "carry.h"

#include <stdio.h>

/*prints the answers to part 4 of the lab.
 * Uses a function that detects if a carry bit
 * would result from an adddition of two bytes of data*/
void lab1_part4()
{
   int carry;
   printf("Part 4: Detecting a Carry condition\n");
   printf("========================\n");
   carry = detectCarry(0x20, 0x35);
   printf("0x20 + 0x35 Carry: %d\n", carry);
   carry = detectCarry(0x80, 0x7F);
   printf("0x80 + 0x7F Carry: %d\n", carry);
   carry = detectCarry(0x80, 0xFF);
   printf("0x80 + 0xFF Carry: %d\n", carry);
   carry = detectCarry(0xFF, 0x01);
   printf("0xFF + 0x01 Carry: %d\n", carry);
   printf("========================\n");
}

/*takes in two unsigned ints and returns if a carry bits was made
 *
 * @param two unsigned chars(or equivalent) order does not matter
 *
 * @return 1 or 0 representing a carry bit
*/
int detectCarry(unsigned char a, unsigned char b)
{
   if(b > ((unsigned char)(a+b)))
   {
      return 1;
   }
   return 0;
}
