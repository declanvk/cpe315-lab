#ifndef COMMON_H
#define COMMON_H

/*
    This union is used to access various components of a floating
    point number in IEEE 754 format
*/
typedef union _float_components {
    float value;
    struct {
        unsigned int frac : 23;
        unsigned int exp  : 8;
        unsigned int sign : 1;
    } fields;
} float_components;

/*
    Integer representation of floating point number
*/
typedef struct _intfloat {
    int exponent; 
    int fraction;
} INTFLOAT, *INTFLOAT_PTR;

// Convert value to type
#define TYPE_CAST(type, value)  ((type) * (type *) &value)

// Two's complement negation
#define NEGATE_TWOS(x) ((~x) + 1)

#endif
