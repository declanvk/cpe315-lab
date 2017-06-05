#ifndef MIPS_PIPES_H
#define MIPS_PIPES_H

#include <stdio.h>

#include "common.h"
#include "mips_context.h"

int fetch(cpu_context *context, program_stats *stats);
int decode(cpu_context *context, program_stats *stats);
int execute(cpu_context *context, program_stats *stats, FILE *output_stream);
int memory(cpu_context *context, program_stats *stats);
int write_back(cpu_context *context, program_stats *stats);

int execute_r_type(cpu_context *context);
int execute_immediate(cpu_context *context);
int execute_syscall(cpu_context *context, FILE *output_stream);
int execute_branch(cpu_context *context, program_stats *stats);
int execute_load_store(cpu_context *context);
int memory_load(cpu_context *context);
int memory_store(cpu_context *context);
int write_back_r_immediate(cpu_context *context);
int write_back_load_store(cpu_context *context);

#endif
