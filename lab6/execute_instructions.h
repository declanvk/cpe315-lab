#ifndef EXECUTE_INSTRUCTIONS_H
#define EXECUTE_INSTRUCTIONS_H

#include <stdint.h>
#include <stdio.h>

#include "mips_context.h"

typedef enum run_mode_e {
    STEP,
    RUN
} run_mode;

int execute_instructions(char *program_name,
                        cpu_context *context,
                        program_stats *stats,
                        FILE *output_stream);

cpu_state transition_state(cpu_context *context);

int fetch(cpu_context *context);
int decode(cpu_context *context);
int execute_r_type(cpu_context *context);
int execute_immediate(cpu_context *context);
int execute_syscall(cpu_context *context, FILE *output_stream);
int execute_branch(cpu_context *context);
int execute_jump(cpu_context *context);
int execute_load_store(cpu_context *context);
int memory_load(cpu_context *context);
int memory_store(cpu_context *context);
int write_back_r_immediate(cpu_context *context);
int write_back_load_store(cpu_context *context);

uint32_t sign_extend(int n_bits, uint32_t value);
uint32_t read_n_bytes(BYTE_PTR memory, int n_bytes, uint32_t address);
void write_n_bytes(BYTE_PTR memory, int n_bytes, uint32_t address, uint32_t value);

#endif
