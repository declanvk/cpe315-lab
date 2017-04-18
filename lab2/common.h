#ifndef COMMON_H
#define COMMON_H

typedef union _float_components {
    float value;
    struct {
        unsigned int frac : 23;
        unsigned int exp  : 8;
        unsigned int sign : 1;
    } fields;
} float_components;

typedef struct _intfloat {
    int exponent; 
    int fraction;
} INTFLOAT, *INTFLOAT_PTR;

#define TYPE_CAST(type, value)  ((type) * (type *) &value);
#define NEGATE_TWOS(x) ((~x) + 1)
#define LSB(x) (0x1 & x)
#define MSB(x) (0x80000000 & x)

#endif
