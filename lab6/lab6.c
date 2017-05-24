#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "load_instructions.h"
#include "execute_instructions.h"
#include "summarise_context.h"

int main(int argc, char *argv[])
{
    cpu_context *context = NULL;
    program_stats stats;
    int cycle_count = 0;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <binary_file_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    context = initialize_cpu(argv[1]);

    cycle_count = execute_instructions(argv[1], context, &stats);
    print_registers(stdout, context, true);
    print_stats(stdout, argv[1], &stats, true);

    free_cpu(context);
    return 0;
}
