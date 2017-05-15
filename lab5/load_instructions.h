#ifndef LOAD_INSTRUCTIONS_H
#define LOAD_INSTRUCTIONS_H

#include <stdio.h>
#include <stdint.h>

#include "mips_asm_header.h"

typedef unsigned int MIPS, *MIPS_PTR;

typedef enum {
    R_TYPE,
    I_TYPE,
    J_TYPE
} instruction_fmt;

FILE *open_file_check(char *filename, MB_HDR_PTR header);
int load_into_memory(FILE *binary_file, MIPS_PTR memory, MB_HDR_PTR header);
int print_instructions(MIPS_PTR memory, unsigned int program_size);
void print_instruction(uint32_t address, uint32_t instruction);

#endif