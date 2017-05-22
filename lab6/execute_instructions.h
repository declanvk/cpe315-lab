#ifndef EXECUTE_INSTRUCTIONS_H
#define EXECUTE_INSTRUCTIONS_H

#include <stdint.h>

#include "mips_context.h"

int execute_instructions(cpu_context *context);

cpu_state transition_state(cpu_context *context);

int fetch(cpu_context *context);
int decode(cpu_context *context);
int execute_r_type(cpu_context *context);
int execute_immediate(cpu_context *context);
int execute_branch(cpu_context *context);
int execute_load_store(cpu_context *context);
int memory_fetch(cpu_context *context);
int memory_store(cpu_context *context);
int write_back_r_immediate(cpu_context *context);
int write_back_load(cpu_context *context);

int sign_extend(int n_bits, uint32_t value);

#endif
