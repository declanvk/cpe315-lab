#include "execute_instructions.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

#include "summarise_context.h"

#define DEBUG_OUTPUT stderr

int execute_instructions(char *program_name, cpu_context *context,
                        program_stats *stats, FILE *output_stream)
{
    int return_status = 0;
    run_mode mode = STEP;
    char input = 0;

    while (return_status == 0 && context->pc < MEMORY_SIZE)
    {
        switch(context->state) {
        case FETCH:
            if (mode == STEP)
            {
                fprintf(stdout, "\n");
                print_registers(stdout, context, true);
                print_stats(stdout, program_name, stats, false);

                printf("Step(s) or Run(r): ");
                do
                    input = getchar();
                while (isspace(input));
                mode = ((input == 's') ? STEP : RUN);
            }
            return_status = fetch(context);
            stats->instruction_count++;
            break;
        case DECODE:
            return_status = decode(context);
            break;
        case EXECUTE:
            if (IS_BRANCH(context->ir))
            {
                return_status = execute_branch(context);
            }
            else if (IS_JUMP(context->ir))
            {
                return_status = execute_jump(context);
            }
            else if (IS_SYSCALL(context->ir))
            {
                return_status = execute_syscall(context, output_stream);
            }
            else if (IS_R_TYPE(context->ir))
            {
                return_status = execute_r_type(context);
            }
            else if (IS_LOAD(context->ir) || IS_STORE(context->ir))
            {
                return_status = execute_load_store(context);
            }
            else
            {
                return_status = execute_immediate(context);
            }
            break;
        case MEM:
            if (IS_STORE(context->ir))
            {
                return_status = memory_store(context);
            }
            else
            {
                return_status = memory_load(context);
            }

            // not lui then inc
            if (context->ir.i_type.opcode != 0x0F)
            {
                stats->memory_ref_count++;
            }
            break;
        case WRITEBACK:
            if (IS_LOAD(context->ir) || IS_STORE(context->ir))
            {
                return_status = write_back_load_store(context);
            }
            else
            {
                return_status = write_back_r_immediate(context);
            }
            break;
        }

        context->state = transition_state(context);
        stats->clock_count++;
    }

    if (context->pc == MEMORY_SIZE)
    {
        fprintf(DEBUG_OUTPUT, "Fell off memory. Exiting.\n");
    }
    else
    {
        fprintf(DEBUG_OUTPUT, "Exit called.\n");
    }

    return return_status;
}

cpu_state transition_state(cpu_context *context)
{
    switch(context->state) {
    case FETCH:
        return DECODE;
    case DECODE:
        return EXECUTE;
    case EXECUTE:
        if (IS_BRANCH(context->ir) || IS_NOP(context->ir) || IS_JUMP(context->ir))
        {
            return FETCH;
        }
        else if (IS_LOAD(context->ir) || IS_STORE(context->ir))
        {
            return MEM;
        }
        else
        {
            return WRITEBACK;
        }
    case MEM:
        return WRITEBACK;
    case WRITEBACK:
        return FETCH;
    }

    return FETCH;
}

int fetch(cpu_context *context)
{
    context->ir.value = *((MIPS_PTR) (context->memory + context->pc));
    fprintf(DEBUG_OUTPUT, "\n%8x: %08x, ", context->pc, context->ir.value);
    context->pc += 4;

    return 0;
}

int decode(cpu_context *context)
{
    context->load_a = context->registers[context->ir.r_type.rs];
    context->load_b = context->registers[context->ir.r_type.rt];
    if (IS_JUMP(context->ir))
    {
        context->alu_out = (context->pc & 0xF0000000) | \
                            ((context->ir.j_type.word_index << 2) & 0x0FFFFFFF);
        fprintf(DEBUG_OUTPUT, "%6s -> %8x, ", "JUMP", context->alu_out);
    }
    else
    {
        context->alu_out = context->pc + (sign_extend(16, context->ir.i_type.immediate) << 2);
        fprintf(DEBUG_OUTPUT, "%6s -> %8x, ", "BRANCH", context->alu_out);
    }
    return 0;
}

int execute_r_type(cpu_context *context)
{
    fprintf(DEBUG_OUTPUT, "Exec Rinst ");
    char *fun_name = NULL;
    switch(context->ir.r_type.func)
    {
    //add
    case 0x20:
        fun_name = "add";
        context->alu_out = context->load_a + context->load_b;
        break;
    //addu
    case 0x21:
        fun_name = "addu";
        context->alu_out = ((unsigned int) context->load_a) + ((unsigned int)context->load_b);
        break;
    //and
    case 0x24:
        fun_name = "and";
        context->alu_out = context->load_a & context->load_b;
        break;
    //nor
    case 0x27:
        fun_name = "nor";
        context->alu_out = ~(context->load_a | context->load_b);
        break;
    //xor
    case 0x26:
        fun_name = "xor";
        context->alu_out = context->load_a ^ context->load_b;
        break;
    //or
    case 0x25:
        fun_name = "or";
        context->alu_out = context->load_a | context->load_b;
        break;
    //slt
    case 0x2A:
        fun_name = "slt";
        context->alu_out = context->load_a < context->load_b;
        break;
    //sltu
    case 0x2B:
        fun_name = "sltu";
        context->alu_out = ((unsigned int) context->load_a) < ((unsigned int) context->load_b);
        break;
    //sll
    case 0x00:
        fun_name = "sll";
        context->alu_out = context->load_b << context->ir.r_type.shamt;
        break;
    //srl
    case 0x02:
        fun_name = "srl";
        context->alu_out = ((unsigned int) context->load_b) >> context->ir.r_type.shamt;
        break;
    //sra
    case 0x03:
        fun_name = "sra";
        context->alu_out = context->load_b >> context->ir.r_type.shamt;
        break;
    //sllv
    case 0x04:
        fun_name = "sllv";
        context->alu_out = context->load_b << context->registers[context->ir.r_type.rs];
        break;
    //srlv
    case 0x06:
        fun_name = "srlv";
        context->alu_out = ((unsigned int) context->load_b) >> context->registers[context->ir.r_type.rs];
        break;
    //srav
    case 0x07:
        fun_name = "srav";
        context->alu_out = context->load_b >> context->registers[context->ir.r_type.rs];
        break;
    //sub
    case 0x22:
        fun_name = "sub";
        context->alu_out = context->load_a - context->load_b;
        break;
    //subu
    case 0x23:
        fun_name = "subu";
        context->alu_out = ((unsigned int) context->load_a) - ((unsigned int)context->load_b);
        break;
    default:
        assert(false);
    }
    fprintf(DEBUG_OUTPUT, "%7s, ", fun_name);
    return 0;
}

int execute_immediate(cpu_context *context)
{
    char *fun_name = NULL;
    fprintf(DEBUG_OUTPUT, "Exec immed ");
    switch(context->ir.i_type.opcode)
    {
    //addi
    case 0x08:
        fun_name = "addi";
        context->alu_out = context->load_a + sign_extend(16, context->ir.i_type.immediate);
        break;
    //addiu
    case 0x09:
        fun_name = "addiu";
        context->alu_out = ((unsigned int) context->load_a) + \
                            sign_extend(16, context->ir.i_type.immediate);
        assert(false);
        break;
    //andi
    case 0x0C:
        fun_name = "andi";
        context->alu_out = context->load_a & context->ir.i_type.immediate;
        break;
    //ori
    case 0x0D:
        fun_name = "ori";
        context->alu_out = context->load_a | context->ir.i_type.immediate;
        break;
    //slti
    case 0x0A:
        fun_name = "slti";
        context->alu_out = context->load_a < sign_extend(16, context->ir.i_type.immediate);
        break;
    //sltiu
    case 0x0B:
        fun_name = "stliu";
        context->alu_out = ((unsigned int )context->load_a) < \
                            ((unsigned int) sign_extend(16, context->ir.i_type.immediate));
        break;
    //xori
    case 0x0E:
        fun_name = "xori";
        context->alu_out = context->load_a ^ context->ir.i_type.immediate;
        break;
    default:
        assert(false);
    }
    fprintf(DEBUG_OUTPUT, "%7s, ", fun_name);
    return 0;
}

int execute_syscall(cpu_context *context, FILE *output_stream)
{
    fprintf(stderr, "Exec syscall %5d, \n", context->registers[v0]);
    switch(context->registers[v0])
    {
    //print_int
    case 1:
        fprintf(output_stream, "%d", context->registers[a0]);
        break;
    //print_string
    case 4:
        fprintf(output_stream, "%s", context->memory + context->registers[a0]);
        break;
    //exit
    case 10:
        return 1;
    default:
        assert(false);
    }
    return 0;
}

int execute_branch(cpu_context *context)
{
    char *followed = "";
    fprintf(DEBUG_OUTPUT, "Exec branch ");
    if (context->ir.i_type.opcode == 0x04 && context->load_a == context->load_b)
    {
        followed = "was eq";
        context->pc = context->alu_out;
    }
    else if (context->ir.i_type.opcode == 0x05 && context->load_a != context->load_b)
    {
        followed = "was neq";
        context->pc = context->alu_out;
    }
    fprintf(DEBUG_OUTPUT, "%7s, ", followed);

    return 0;
}

int execute_jump(cpu_context *context)
{
    fprintf(DEBUG_OUTPUT, "Jumping to ");
    if (context->ir.j_type.opcode == 0x02) //j
    {
        context->pc = context->alu_out;
    }
    else if (context->ir.j_type.opcode == 0x03) //jal
    {
        context->registers[ra] = context->pc;
        context->pc = context->alu_out;
    }
    else if (context->ir.r_type.opcode == 0x00 && context->ir.r_type.func == 0x08) //jr
    {
        context->pc = context->registers[context->ir.r_type.rs];
    }
    else
    {
        context->registers[ra] = context->pc;
        context->pc = context->registers[context->ir.r_type.rs];
    }
    fprintf(DEBUG_OUTPUT, "%07x, ", context->pc);

    return 0;
}

int execute_load_store(cpu_context *context)
{
    fprintf(DEBUG_OUTPUT, "%20s", "Loading or storing, ");
    context->alu_out = context->load_a + sign_extend(16, context->ir.i_type.immediate);
    return 0;
}

int memory_load(cpu_context *context)
{
    int n_bytes = 4;

    fprintf(DEBUG_OUTPUT, "MEM load ");
    if (context->ir.r_type.opcode == 0x20 || context->ir.r_type.opcode == 0x24)
    {
        n_bytes = 1;
    }
    else if (context->ir.r_type.opcode == 0x21 || context->ir.r_type.opcode == 0x25)
    {
        n_bytes = 2;
    }

    if (context->ir.r_type.opcode == 0x20 || context->ir.r_type.opcode == 0x21)
    {
        context->memory_data = sign_extend(n_bytes * 8,
            read_n_bytes(context->memory, n_bytes, context->alu_out));
    }
    else
    {
        context->memory_data = read_n_bytes(context->memory, n_bytes, context->alu_out);
    }

    if (context->ir.r_type.opcode == 0x0F)
    {
        context->memory_data = context->ir.i_type.immediate << 16;
    }

    fprintf(DEBUG_OUTPUT, "%8x %d, ", context->memory_data, n_bytes);

    return 0;
}

int memory_store(cpu_context *context)
{
    int n_bytes = 4;

    fprintf(DEBUG_OUTPUT, "MEM store ");
    if (context->ir.i_type.opcode == 0x28)
    {
        n_bytes = 1;
    }
    else if (context->ir.i_type.opcode == 0x2b)

    {
        n_bytes = 2;
    }

    write_n_bytes(context->memory, n_bytes, context->alu_out, context->load_b);

    fprintf(DEBUG_OUTPUT, "%5x -> %04x\n", context->load_b, context->alu_out);
    return 0;
}

int write_back_r_immediate(cpu_context *context)
{
    fprintf(DEBUG_OUTPUT, "                Writing reg ");
    if (IS_R_TYPE(context->ir))
    {
        fprintf(DEBUG_OUTPUT, "%2d = ", context->ir.r_type.rd);
        context->registers[context->ir.r_type.rd] = context->alu_out;
    }
    else
    {
        fprintf(DEBUG_OUTPUT, "%2d = ", context->ir.r_type.rt);
        context->registers[context->ir.r_type.rt] = context->alu_out;
    }
    fprintf(DEBUG_OUTPUT, "%08x", context->alu_out);
    
    return 0;
}

int write_back_load_store(cpu_context *context)
{
    if (IS_LOAD(context->ir))
    {
        fprintf(DEBUG_OUTPUT, "Writing %2d: %8x", context->ir.i_type.rt, context->memory_data);
        context->registers[context->ir.i_type.rt] = context->memory_data;
    }
    else
    {
        fprintf(DEBUG_OUTPUT, "%20s", "");
    }
    return 0;
}

uint32_t sign_extend(int n_bits, uint32_t value)
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

uint32_t read_n_bytes(BYTE_PTR memory, int n_bytes, uint32_t address)
{
    int index;
    uint32_t result = 0;

    for (index = 0; index < n_bytes; index++)
    {
        result |= (uint32_t) (memory[address + index] << (8 * index));
    }

    return result;
}

void write_n_bytes(BYTE_PTR memory, int n_bytes, uint32_t address, uint32_t value)
{
    int index;

    for (index = 0; index < n_bytes; index++)
    {
        memory[address + index] = (0xFF & value);
        value >>= 8;
    }
}
