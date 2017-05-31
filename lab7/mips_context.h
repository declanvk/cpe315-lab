#ifndef MIPS_CONTEXT_H
#define MIPS_CONTEXT_H

#include <stdint.h>
#include <stdbool.h>

#include "common.h"

typedef uint8_t BYTE, *BYTE_PTR;
struct fetch_decode_buffer {
    bool status;
    uint32_t inc_pc;
    instruction ir;
};

struct decode_execute_buffer {
    bool status;
    uint32_t inc_pc;
    int32_t load_a;
    int32_t load_b;
    int32_t sign_imm;
};

struct execute_memory_buffer {
    bool status;
    uint32_t branched_pc;
    uint32_t zero;
    MIPS alu_out;
    int32_t load_b;
};

struct memory_writeback_buffer {
    bool status;
    MIPS memory_data;
    MIPS alu_out;
};

typedef struct cpu_context_t {
    BYTE_PTR memory;
    MIPS registers[32];
    uint32_t pc;
    fetch_decode_buffer if_id;
    decode_execute_buffer id_ex;
    execute_memory_buffer ex_mem;
    memory_writeback_buffer mem_wb;
} cpu_context;

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
