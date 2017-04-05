#include <stdio.h>

int detectCarry(unsigned char a, unsigned char b)
{
   if(b > ((unsigned char)(a+b)))
   {
      return 1;
   }
   return 0;
}

int main()
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

