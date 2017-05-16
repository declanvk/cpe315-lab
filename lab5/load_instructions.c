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

int instruction_index[] = {
    22, 22, 0, 1, 2, 3, 4, 5, 6, 7, 8,
    9, 10, 11, 12, 13, 22, 22, 22, 22,
    22, 22, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 22, 14, 15, 16, 22, 17, 18,
    22, 22, 19, 20, 21
};

char * instruction_names[] = {
    "j", "jal", "beq", "bne", "blez", "bgtz", "addi",
    "addiu", "slti", "sltiu", "andi", "ori", "xori",
    "lui", "lb", "lh", "lw", "lbu", "lhu", "sb", "sh",
    "sw", "INVALID"
};

int instruction_format_parameters[] = {
    6, 6, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2,
    4, 3, 3, 3, 3, 3, 5, 5, 5, 7
};

char * instruction_format[] = {
    "PC = %010d",
    "R[31] = PC + 8; PC = %010d",
    "if(R[$%02d] = R[$%02d]) PC = %010d",
    "if(R[$%02d] != R[$%02d]) PC = %010d",
    "if(R[$%02d] <= 0) PC = %010d",
    "if(R[$%02d] > 0) PC = %010d",
    "R[$%02d] = R[$%02d] + %5d",
    "R[$%02d] = R[$%02d] + %5d",
    "R[$%02d] = R[$%02d] < %5d",
    "R[$%02d] = R[$%02d] < %5d",
    "R[$%02d] = R[$%02d] & %5d",
    "R[$%02d] = R[$%02d] | %5d",
    "R[$%02d] = R[$%02d] ^ %5d",
    "R[$%02d] = {%5d, 0 x 16}",
    "R[$%02d] = SignExt(Mem[R[$%02d] + %5d])",
    "R[$%02d] = SignExt(Mem[R[$%02d] + %5d])",
    "R[$%02d] = Mem[R[$%02d] + %5d]",
    "R[$%02d] = {0 x 24, Mem[R[$%02d] + %5d]}",
    "R[$%02d] = {0 x 16, Mem[R[$%02d] + %5d]}",
    "Mem[R[$%02d] + %5d] = (R[$%02d])[7:0]",
    "Mem[R[$%02d] + %5d] = (R[$%02d])[15:0]",
    "Mem[R[$%02d] + %5d] = R[$%02d]", "INVALID"
};

int function_index[] = {
    0, 27, 1, 2, 3, 27, 4, 5, 6, 7, 27,
    27, 8, 27, 27, 27, 9, 10, 11, 12, 27,
    27, 27, 27, 13, 14, 15, 16, 27, 27, 27,
    27, 17, 18, 19, 20, 21, 22, 23, 24, 27,
    27, 25, 26
};

char * function_names[] = {
    "sll", "srl", "sra", "sllv", "srlv", "srav", "jr",
    "jalr", "syscall", "mfhi", "mthi", "mflo",
    "mtlo", "mult", "multu", "div", "divu", "add", "addu",
    "sub", "subu", "and", "or", "xor", "nor", "slt", "sltu",
    "INVALID"
};

int function_format_parameters[] = {
    2, 2, 2, 1, 1, 1, 6, 3, 9, 8, 6, 8, 6,
    4, 4, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 10
};

char * function_format[] = {
    "R[$%02d] = R[$%02d] << %02d",
    "R[$%02d] = R[$%02d] >> %02d",
    "R[$%02d] = R[$%02d] >> %02d",
    "R[$%02d] = R[$%02d] << R[$%02d]",
    "R[$%02d] = R[$%02d] >> R[$%02d]",
    "R[$%02d] = R[$%02d] >> R[$%02d]",
    "PC = R[$%02d]",
    "R[$%02d] = PC + 8; PC = R[$%02d]",
    "%s",
    "R[$%02d] = HI", "HI = R[$%02d]",
    "R[$%02d] = LO", "LO = R[$%02d]",
    "{HI, LO} = R[$%02d] * R[$%02d]",
    "{HI, LO} = R[$%02d] * R[$%02d]",
    "LO = R[$%02d] / R[$%02d]; HI = R[$%02d] %% R[$%02d]" ,
    "LO = R[$%02d] / R[$%02d]; HI = R[$%02d] %% R[$%02d]" ,
    "R[$%02d] = R[$%02d] + R[$%02d]",
    "R[$%02d] = R[$%02d] + R[$%02d]",
    "R[$%02d] = R[$%02d] - R[$%02d]",
    "R[$%02d] = R[$%02d] - R[$%02d]",
    "R[$%02d] = R[$%02d] & R[$%02d]",
    "R[$%02d] = R[$%02d] | R[$%02d]",
    "R[$%02d] = R[$%02d] ^ R[$%02d]",
    "R[$%02d] = ~(R[$%02d] | R[$%02d])",
    "R[$%02d] = R[$%02d] < R[$%02d]",
    "R[$%02d] = R[$%02d] < R[$%02d]",
    "INVALID"
};

void print_instruction(uint32_t address, uint32_t instruction_value)
{
    instruction instr;
    instr.value = instruction_value;
    int op_code = instr.r_type.op_code;

    if (op_code == 0)
    {
        print_r_type(instr);

    }
    else 
    {
        print_other_type(address, instr);
    }

    printf("\n");
}

void print_r_type(instruction instr)
{
    int func_code = instr.r_type.func;
    int func_index = function_index[func_code];
    char * func_name = function_names[func_index];
    char * func_format = function_format[func_index];
    int func_format_params = function_format_parameters[func_index];

    printf("R 00/%02d| %-7s ", func_code, func_name);

    switch(func_format_params) {
    case 0:
        printf(func_format, instr.r_type.rd, instr.r_type.rs, instr.r_type.rt);
        break;
    case 1:
        printf(func_format, instr.r_type.rd, instr.r_type.rt, instr.r_type.rs);
        break;
    case 2:
        printf(func_format, instr.r_type.rd, instr.r_type.rt, instr.r_type.shamt);
        break;
    case 3:
        printf(func_format, instr.r_type.rs, instr.r_type.rd);
        break;
    case 4:
        printf(func_format, instr.r_type.rs, instr.r_type.rt);
        break;
    case 5:
        printf(func_format, instr.r_type.rs, instr.r_type.rt, instr.r_type.rs, instr.r_type.rt);
        break;
    case 6:
        printf(func_format, instr.r_type.rs);
        break;
    case 7:
        printf(func_format, instr.r_type.rt);
        break;
    case 8:
        printf(func_format, instr.r_type.rd);
    case 9:
        printf(func_format, "");
        break;
    case 10:
        printf(func_format);
        break;
    default:
        printf("%s", func_format);
    }
}

void print_other_type(uint32_t address, instruction instr)
{
    int op_code = instr.r_type.op_code;
    int op_index = instruction_index[op_code];
    char * op_name = instruction_names[op_index];
    char * op_format = instruction_format[op_index];
    int op_format_params = instruction_format_parameters[op_index];
    char op_type = (op_code == 0x02 || op_code == 0x03) ? 'J' : 'I';

    printf("%c %5d| %-7s ", op_type, op_code, op_name);

    switch(op_format_params) {
    case 0:
        printf(op_format, instr.i_type.rs, instr.i_type.rt,
            address + 4 + sign_extend(18, instr.i_type.immediate));
        break;
    case 1:
        printf(op_format, instr.i_type.rs,
            address + 4 + sign_extend(18, instr.i_type.immediate));
        break;
    case 2:
    case 3:
        printf(op_format, instr.i_type.rt, instr.i_type.rs,
            sign_extend(16, instr.i_type.immediate));
        break;
    case 4:
        printf(op_format, instr.i_type.rt, instr.i_type.immediate);
        break;
    case 5:
        printf(op_format, instr.i_type.rs, sign_extend(16, instr.i_type.immediate),
            instr.i_type.rt);
        break;
    case 6:
        printf(op_format, instr.j_type.word_index);
        break;
    case 7:
        printf(op_format);
    default:
        printf("%s", op_format);
    }
}

int sign_extend(int n_bits, uint32_t value)
{
    uint32_t mask = ~(-1 << n_bits);
    uint32_t selected = value & mask;
    uint32_t result = -1 << n_bits;

    if (n_bits >= 32)
    {   
        return value;
    }

    if (((selected >> (n_bits - 1)) & 1) == 0)
    {   
        return selected;
    }
    else
    {
        result |= selected;
        return result;
    }
}
