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
        unsigned int opcode : 6;
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

#define IS_R_TYPE(opcode, func) (opcode == 0x00)
#define IS_BRANCH(opcode, func) (opcode == 0x04 || opcode == 0x05)
#define IS_STORE(opcode, func) (opcode == 0x28 || opcode == 0x29 || opcode == 0x28)
#define IS_LOAD(opcode, func) (opcode == 0x20 || opcode == 0x24 || opcode == 0x21 || opcode == 0x25 || opcode == 0x0F || opcode == 0x23)
#define IS_JUMP(opcode, func) (IS_JUMP_LITERAL(opcode, func) || IS_JUMP_REGISTER(opcode, func))
#define IS_JUMP_LITERAL(opcode, func) (opcode == 0x02 || opcode == 0x03)
#define IS_JUMP_REGISTER(opcode, func) (IS_R_TYPE(opcode, func) && (func == 0x08 || func == 0x09))
#define IS_IMMEDIATE(opcode, func) (!(IS_R_TYPE(opcode, func) || IS_JUMP(opcode, func)))
#define IS_SYSCALL(opcode, func) (IS_R_TYPE(opcode, func) && (func == 0x0C))

typedef struct mips_program_stats {
    unsigned int clock_count;
    unsigned int flush_count;
    unsigned int fetch_count;
    unsigned int decode_count;
    unsigned int execute_count;
    unsigned int memory_cycle_count;
    unsigned int writeback_count;
    unsigned int completed_instruction_count;
    unsigned int memory_ref_count;
} program_stats;

#endif
