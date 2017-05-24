#include "load_instructions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "mips_asm_header.h"
#include "common.h"
#include "mips_context.h"

cpu_context *initialize_cpu(char *filename)
{
    cpu_context *context = (cpu_context *) calloc(1, sizeof(cpu_context));
    FILE *binary_file = NULL;
    MB_HDR mb_header;
    int num_read_bytes = 0;

    context->memory = (BYTE_PTR) calloc(1, MEMORY_SIZE);
    context->state = FETCH;
    context->registers[31] = MEMORY_SIZE - 1;

    binary_file = open_file_check(filename, &mb_header);

    num_read_bytes = load_into_memory(binary_file, context->memory, &mb_header);
    if (num_read_bytes != mb_header.size)
    {
        fprintf(stderr, "Could not read whole file, only %d bytes\n", num_read_bytes);
        exit(EXIT_FAILURE);
    }

    context->pc = mb_header.entry;

    fclose(binary_file);

    return context;
}

void free_cpu(cpu_context *context)
{
    free(context->memory);
    free(context);
}

FILE *open_file_check(char *filename, MB_HDR_PTR header)
{
    FILE *binary_file = NULL;

    /* format the MIPS Binary header */
    if ((binary_file = fopen(filename, "rb")) == NULL)
    {
        fprintf(stderr, "Couldn't open assembly binary file: %s.\n", filename);
        exit(EXIT_FAILURE);
    }

    /* read the header and verify it. */
    fread((void *) header, sizeof(MB_HDR), 1, binary_file);
    if (!(strcmp(header->signature, "~MB") == 0))
    {
        printf("\nThis isn't really a mips_asm binary file - quitting.\n");
        exit(EXIT_FAILURE);
    }

    return binary_file;
}

int load_into_memory(FILE *binary_file, BYTE_PTR memory, MB_HDR_PTR header)
{
    int mem_ptr;
    int input_byte;

    mem_ptr = 0; /* This is the memory pointer, a byte offset */
    do {
        /* note div/4 to make word index */
        input_byte = fread((void *) (memory + mem_ptr), 4, 1, binary_file); 
        if (input_byte)
        {
            mem_ptr += 4;  /* Increment byte pointer by size of instr */
        } else {
            break;
        }
    } while (mem_ptr < header->size);

    return mem_ptr;
}
