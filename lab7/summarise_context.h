#ifndef SUMMARISE_CONTEXT_H
#define SUMMARISE_CONTEXT_H

#include <stdio.h>
#include <stdbool.h>

#include "common.h"
#include "mips_context.h"

void print_registers(FILE *output_stream, cpu_context *context, bool hex_output);
void print_stats(FILE *output_stream, char *program_name, program_stats *stats, bool end);
void print_clock_header(FILE *output_stream);

#endif
