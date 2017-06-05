#ifndef MIPS_CONTEXT_H
#define MIPS_CONTEXT_H

#include <stdint.h>
#include <stdbool.h>

#include "common.h"

typedef uint8_t BYTE, *BYTE_PTR;

struct fetch_decode_buffer {
    bool skip_cycle;
    bool decode_busy;
    uint32_t inc_pc;
    instruction ir;
};

struct decode_execute_buffer {
    bool skip_cycle;
    bool execute_busy;
    uint32_t inc_pc;
    int32_t load_a;
    int32_t load_b;
    uint32_t immediate;
    int32_t extended_immediate;
    uint32_t opcode;
    uint32_t shamt;
    uint32_t func;
    uint32_t rd;
    uint32_t rt;
};

struct execute_memory_buffer {
    bool skip_cycle;
    bool memory_busy;
    uint32_t branched_pc;
    MIPS alu_out;
    int32_t load_b;
    uint32_t opcode;
    uint32_t func;
    uint32_t immediate;
    uint32_t rd;
    uint32_t rt;
};

struct memory_writeback_buffer {
    bool skip_cycle;
    bool writeback_busy;
    MIPS memory_data;
    MIPS alu_out;
    uint32_t opcode;
    uint32_t func;
    uint32_t rd;
    uint32_t rt;
};

typedef struct cpu_context_t {
    BYTE_PTR memory;
    MIPS registers[32];
    bool registers_valid[32];
    uint32_t pc;
    struct fetch_decode_buffer if_id;
    struct decode_execute_buffer id_ex;
    struct execute_memory_buffer ex_mem;
    struct memory_writeback_buffer mem_wb;
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
