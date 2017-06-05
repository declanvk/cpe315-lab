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

run_mode interact(char *program_name, cpu_context *context, program_stats *stats, run_mode current_mode);
void add_output_offset(FILE *output_stream, int space_sections);

#endif
