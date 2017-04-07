#ifndef EXT_FIELD_H
#define EXT_FIELD_H

void lab1_part6();
void output_add(unsigned char *result, unsigned char *a1, unsigned char *a2, int size);
unsigned int arbitrary_byte_add(
    unsigned char *result,
    unsigned char *a1,
    unsigned char *a2,
    int size,
    unsigned int carry_in);
int detect_carry(unsigned char a, unsigned char b, unsigned char c);

#endif
