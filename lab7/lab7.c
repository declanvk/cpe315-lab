#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "common.h"
#include "load_instructions.h"
#include "execute_instructions.h"
#include "summarise_context.h"

int main(int argc, char *argv[])
{
    cpu_context *context = NULL;
    program_stats stats;
    int cycle_count = 0;
    FILE *output_stream = NULL;

    if (argc != 2 && argc != 3)
    {
        fprintf(stderr, "usage: %s <binary_file_name> <optional_output_file_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    context = initialize_cpu(argv[1]);
    if (argc == 2)
    {
        output_stream = stderr;
    }
    else
    {
        output_stream = fopen(argv[2], "w");
    }

    stats = (const program_stats) {0};

    cycle_count = execute_instructions(argv[1], context, &stats, output_stream);
    print_registers(stdout, context, false);
    print_stats(stdout, argv[1], &stats, true);

    if (argc == 3)
    {
        fclose(output_stream);
    }

    free_cpu(context);
    return 0;
}
