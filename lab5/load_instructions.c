#include "load_instructions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "mips_asm_header.h"

int main(int argc, char *argv[])
{
    FILE *binary_file = NULL;
    MB_HDR mb_header;
    MIPS_PTR memory = NULL;
    int num_read_bytes = 0;

    if (argc > 2)
    {
        fprintf(stderr, "usage: ./lab5 <binary_file_name>\n");
        exit(EXIT_FAILURE);
    }

    binary_file = open_file_check(argv[1], &mb_header);
    memory = (MIPS_PTR) malloc(mb_header.size);
    num_read_bytes = load_into_memory(binary_file, memory, &mb_header);
    if (num_read_bytes != mb_header.size)
    {
        fprintf(stderr, "Could not read whole file, only %d bytes\n", num_read_bytes);
        exit(EXIT_FAILURE);
    }

    print_instructions(memory, mb_header.size);

    free(memory);
    fclose(binary_file);
    exit(EXIT_SUCCESS);
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
    if (!strcmp(header->signature, "~MB") == 0)
    {
        printf("\nThis isn't really a mips_asm binary file - quitting.\n");
        exit(EXIT_FAILURE);
    }

    return binary_file;
}

int load_into_memory(FILE *binary_file, MIPS_PTR memory, MB_HDR_PTR header)
{
    int mem_ptr;
    int input_byte;

    mem_ptr = 0; /* This is the memory pointer, a byte offset */
    do {
        /* note div/4 to make word index */
        input_byte = fread((void *) &memory[mem_ptr/4], 4, 1, binary_file); 
        if (input_byte)
        {
            mem_ptr += 4;  /* Increment byte pointer by size of instr */
        } else {
            break;
        }
    } while (mem_ptr < header->size);

    return mem_ptr;
}

int print_instructions(MIPS_PTR memory, unsigned int program_size)
{
    for (int i = 0; i < program_size; i += 4)
    {
        print_instruction(i, memory[i / 4]);
    }

    return EXIT_SUCCESS;
}

#define OP_CODE(instr) (instr & 0xFC000000)

void print_instruction(uint32_t address, uint32_t instruction)
{
    int op_code = OP_CODE(instruction);
    if (op_code == 0)
    {
        printf("R Type ");
    }
    else if(op_code == 0x02 || op_code == 0x03)
    {
        printf("J Type ");
    }
    else
    {
        printf("I Type ");
    }

    printf("Instruction@%08X : %08X\n", address, instruction);
}