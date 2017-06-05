#include "execute_instructions.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

#include "cpu_utils.h"
#include "summarise_context.h"
#include "mips_pipes.h"

#define DEBUG_OUTPUT stderr

int execute_instructions(char *program_name, cpu_context *context, program_stats *stats, FILE *output_stream)
{
    int return_status = 0;
    run_mode mode = STEP;

    while (return_status == 0 && context->pc < MEMORY_SIZE)
    {
        stats->clock_count++;
        if (mode == STEP)
        {
            mode = interact(program_name, context, stats, mode);
            print_clock_header(DEBUG_OUTPUT);
        }

        add_output_offset(DEBUG_OUTPUT, 4);
        return_status |= write_back(context, stats);
        add_output_offset(DEBUG_OUTPUT, 3);
        return_status |= memory(context, stats);
        add_output_offset(DEBUG_OUTPUT, 2);
        return_status |= execute(context, stats, DEBUG_OUTPUT);
        add_output_offset(DEBUG_OUTPUT, 1);
        return_status |= decode(context, stats);
        add_output_offset(DEBUG_OUTPUT, 0);
        return_status |= fetch(context, stats);

        fprintf(DEBUG_OUTPUT, "\n");
    }

    if (context->pc == MEMORY_SIZE)
    {
        fprintf(DEBUG_OUTPUT, "Fell off memory. Exiting.\n");
    }

    return return_status;
}

run_mode interact(char *program_name, cpu_context *context, program_stats *stats, run_mode current_mode)
{
    run_mode new_mode = STEP;
    char input = 0;

    fprintf(stdout, "\n");
    print_registers(stdout, context, true);
    print_stats(stdout, program_name, stats, false);

    printf("Step(s) or Run(r): ");
    do
        input = getchar();
    while (isspace(input));
    new_mode = ((input == 's') ? STEP : RUN);

    return new_mode;
}

void add_output_offset(FILE *output_stream, int space_sections)
{
    int num_spaces = 24 * space_sections;
    char *format_format = "\r%%%ds|";
    char *output_format = NULL;

    if (space_sections == 0)
    {
        fprintf(output_stream, "\r|");
        return;
    }

    output_format = calloc(5 + ((int) ceil(log10(num_spaces))) + 1, 1);
    sprintf(output_format, format_format, num_spaces);

    fprintf(output_stream, output_format, "");

    free(output_format);
}
