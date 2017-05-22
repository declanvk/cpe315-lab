#include <stdio.h>
#include <stdlib.h>

#include "load_instructions.h"
#include "execute_instructions.h"

int main(int argc, char *argv[])
{
    cpu_context *context = NULL;
    int cycle_count = 0;

    if (argc > 2)
    {
        fprintf(stderr, "usage: %s <binary_file_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    context = initialize_cpu(argv[1]);

    cycle_count = execute_instructions(context);
    printf("Program took %d cycles.\n", cycle_count);

    free_cpu(context);
    return 0;
}
