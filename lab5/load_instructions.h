#ifndef LOAD_INSTRUCTIONS_H
#define LOAD_INSTRUCTIONS_H

#include <stdio.h>
#include <stdint.h>

#include "mips_asm_header.h"

typedef unsigned int MIPS, *MIPS_PTR;

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
	uint32_t value;
} instruction;

FILE *open_file_check(char *filename, MB_HDR_PTR header);
int load_into_memory(FILE *binary_file, MIPS_PTR memory, MB_HDR_PTR header);
int print_instructions(MIPS_PTR memory, unsigned int program_size);
void print_instruction(uint32_t address, uint32_t instruction);
void print_r_type(instruction instr);
void print_other_type(uint32_t address, instruction instr);

int sign_extend(int n_bits, uint32_t value);

#endif