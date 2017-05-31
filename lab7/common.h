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

#define IS_R_TYPE(instr) (instr.r_type.opcode == 0)
#define IS_BRANCH(instr) (instr.i_type.opcode == 0x04 || instr.i_type.opcode == 0x05)
#define IS_NOP(instr) (instr.value == 0x00000000)
#define IS_STORE(instr) (instr.i_type.opcode == 0x28 || \
                            instr.i_type.opcode == 0x29 || \
                            instr.i_type.opcode == 0x2B)
#define IS_LOAD(instr) (instr.i_type.opcode == 0x20 || \
                        instr.i_type.opcode == 0x24 || \
                        instr.i_type.opcode == 0x21 || \
                        instr.i_type.opcode == 0x25 || \
                        instr.i_type.opcode == 0x0F || \
                        instr.i_type.opcode == 0x23)
#define IS_JUMP(instr) (instr.j_type.opcode == 0x02 || \
                        instr.j_type.opcode == 0x03 || \
                        (IS_R_TYPE(instr) &&\
                            (instr.r_type.func == 0x08 ||\
                             instr.r_type.func == 0x09)))
#define IS_IMMEDIATE(instr) (!(IS_R_TYPE(instr) || IS_JUMP(instr)))
#define IS_SYSCALL(instr) (IS_R_TYPE(instr) && (instr.r_type.func == 0x0C))

typedef struct mips_program_stats {
    unsigned int clock_count;
    unsigned int instruction_count;
    unsigned int memory_ref_count;
} program_stats;

#endif
