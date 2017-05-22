#ifndef MIPS_CONTEXT_H
#define MIPS_CONTEXT_H

#include <stdint.h>

#include "common.h"

typedef struct cpu_context_t cpu_context;

struct cpu_context_t {
    MIPS_PTR memory;
    MIPS registers[32];
    MIPS alu_out;
    instruction ir_register;
    MIPS memory_data_register;
    MIPS load_a_register;
    MIPS load_b_register;
    uint32_t program_counter;
};

#define MEMORY_SIZE 10000000

#define zero 0
#define at 1
#define v0 2
#define v1 3
#define a0 4
#define a1 5
#define a2 6
#define a3 7
#define t0 8
#define t1 9
#define t2 10
#define t3 11
#define t4 12
#define t5 13
#define t6 14
#define t7 15
#define s0 16
#define s1 17
#define s2 18
#define s3 19
#define s4 20
#define s5 21
#define s6 22
#define s7 23
#define t8 24
#define t9 25
#define k0 26
#define k1 27
#define gp 28
#define sp 29
#define fp 30
#define ra 31

#endif
