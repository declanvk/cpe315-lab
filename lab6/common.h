#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

typedef uint32_t MIPS, *MIPS_PTR;

typedef union {
    struct {
        unsigned int func : 6;
        unsigned int shamt : 5;
        unsigned int rd : 5;
        unsigned int rt : 5;
        unsigned int rs : 5;
        unsigned int op_code : 6;
    } r_type;
    struct {
        unsigned int immediate : 16;
        unsigned int rt : 5;
        unsigned int rs : 5;
        unsigned int opcode : 6;
    } i_type;
    struct {
        unsigned int word_index : 26;
        unsigned int opcode : 6;
    } j_type;
    MIPS value;
} instruction;

#endif
