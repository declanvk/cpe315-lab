#include <stdio.h>

#include "load_instructions.h"

int main(int argc, char const *argv[])
{
    cpu_context *context = NULL;

    if (argc > 2)
    {
        fprintf(stderr, "usage: %s <binary_file_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    context = initialize_cpu(argv[1]);

    free_cpu(context);
    
    fclose(binary_file);
    return 0;
}