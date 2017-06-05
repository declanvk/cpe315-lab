#include "mips_pipes.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "cpu_utils.h"

#define DEBUG_OUTPUT stderr

int fetch(cpu_context *context, program_stats *stats)
{
    if (context->if_id.decode_busy)
    {
        fprintf(DEBUG_OUTPUT, "%7s(nothing)", "");
        return 0;
    }

    context->if_id.ir.value = read_n_bytes(context->memory, 4, context->pc);
    fprintf(DEBUG_OUTPUT, "%08x -> %08x", context->pc, context->if_id.ir.value);

    context->pc += 4;
    context->if_id.inc_pc = context->pc;

    stats->fetch_count++;
    return 0;
}

int decode(cpu_context *context, program_stats *stats)
{
    if (context->id_ex.execute_busy)
    {
        fprintf(DEBUG_OUTPUT, "%7s(nothing)", "");
        return 0;
    }

    if ((IS_R_TYPE(context->if_id.ir.r_type.opcode, context->if_id.ir.r_type.func) &&
        (!context->registers_valid[context->if_id.ir.r_type.rs] || !context->registers_valid[context->if_id.ir.r_type.rt])) ||
        ((IS_IMMEDIATE(context->if_id.ir.r_type.opcode, context->if_id.ir.r_type.func) &&
            !(IS_STORE(context->if_id.ir.r_type.opcode, context->if_id.ir.r_type.func))) &&
        !context->registers_valid[context->if_id.ir.r_type.rs]) || 
        (IS_SYSCALL(context->if_id.ir.r_type.opcode, context->if_id.ir.r_type.func) &&
            !context->registers_valid[v0]))
    {
        fprintf(DEBUG_OUTPUT, "Invalid ");
        if (!context->registers_valid[context->if_id.ir.r_type.rs])
        {
            fprintf(DEBUG_OUTPUT, "RS:%02x ", context->if_id.ir.r_type.rs);
        }
        
        if (!context->registers_valid[context->if_id.ir.r_type.rt])
        {
            fprintf(DEBUG_OUTPUT, "RT:%02x ", context->if_id.ir.r_type.rt);
        }

        if (!context->registers_valid[v0])
        {
            fprintf(DEBUG_OUTPUT, "V0");
        }

        context->if_id.skip_cycle = true;
        context->if_id.decode_busy = true;
    }

    if (context->if_id.skip_cycle)
    {
        if (!(!context->registers_valid[context->if_id.ir.r_type.rs] || 
            !context->registers_valid[context->if_id.ir.r_type.rt]))
        {
            fprintf(DEBUG_OUTPUT, "%7s(nothing)", "");
        }

        context->if_id.skip_cycle = false;
        context->id_ex.skip_cycle = true;
        return 0;
    }
    context->if_id.decode_busy = true;

    context->id_ex.load_a = context->registers[context->if_id.ir.r_type.rs];
    context->id_ex.load_b = context->registers[context->if_id.ir.r_type.rt];
    context->id_ex.inc_pc = context->if_id.inc_pc;
    context->id_ex.extended_immediate = sign_extend(16, context->if_id.ir.i_type.immediate);

    context->id_ex.opcode = context->if_id.ir.r_type.opcode;
    context->id_ex.func = context->if_id.ir.r_type.func;
    context->id_ex.shamt = context->if_id.ir.r_type.shamt;
    context->id_ex.immediate = context->if_id.ir.i_type.immediate;
    context->id_ex.rd = context->if_id.ir.r_type.rd;
    context->id_ex.rt = context->if_id.ir.r_type.rt;


    if (IS_R_TYPE(context->if_id.ir.r_type.opcode, context->if_id.ir.r_type.func))
    {
        if (context->if_id.ir.r_type.rd != 0)
        {
            context->registers_valid[context->if_id.ir.r_type.rd] = false;
        }
    }
    else if (IS_IMMEDIATE(context->if_id.ir.r_type.opcode, context->if_id.ir.r_type.func) && !(IS_STORE(context->if_id.ir.r_type.opcode, context->if_id.ir.r_type.func) || IS_BRANCH(context->if_id.ir.r_type.opcode, context->if_id.ir.r_type.func)))
    {
        if (context->if_id.ir.r_type.rt != 0)
        {
            context->registers_valid[context->if_id.ir.r_type.rt] = false;
        }
    }

    if (IS_JUMP(context->if_id.ir.r_type.opcode, context->if_id.ir.r_type.func))
    {
        if (IS_JUMP_LITERAL(context->if_id.ir.r_type.opcode, context->if_id.ir.r_type.func))
        {
            fprintf(stderr, "Jump to %08x", (context->if_id.inc_pc & 0xF0000000) | ((context->if_id.ir.j_type.word_index << 2) & 0x0FFFFFFF));
        }
        else if (IS_JUMP_REGISTER(context->if_id.ir.r_type.opcode, context->if_id.ir.r_type.func))
        {
            fprintf(stderr, "Jump to %08x", context->registers[context->if_id.ir.r_type.rs]);
        }
    }
    else
    {
        fprintf(stderr, "%02d:%08x,%02d:%08x", context->if_id.ir.r_type.rs, context->id_ex.load_a, context->if_id.ir.r_type.rt, context->id_ex.load_b);
    }

    if (IS_JUMP_LITERAL(context->if_id.ir.r_type.opcode, context->if_id.ir.r_type.func))
    {
        context->pc = (context->if_id.inc_pc & 0xF0000000) | \
                            ((context->if_id.ir.j_type.word_index << 2) & 0x0FFFFFFF);
        
    }
    else if (IS_JUMP_REGISTER(context->if_id.ir.r_type.opcode, context->if_id.ir.r_type.func))
    {
        context->pc = context->registers[context->if_id.ir.r_type.rs];
    }

    if (IS_JUMP(context->if_id.ir.r_type.opcode, context->if_id.ir.r_type.func))
    {
        context->id_ex = (const struct decode_execute_buffer) {0};
        context->id_ex.skip_cycle = true;
    }

    if (context->if_id.ir.j_type.opcode == 0x03 || (context->if_id.ir.r_type.opcode == 0x00 && context->if_id.ir.r_type.func == 0x09))
    {
        context->registers[ra] = context->if_id.inc_pc;
    }

    stats->decode_count++;
    context->if_id.decode_busy = false;
    return 0;
}

int execute(cpu_context *context, program_stats *stats, FILE *output_stream)
{
    int return_status = 0;

    if (context->ex_mem.memory_busy)
    {
        fprintf(DEBUG_OUTPUT, "%7s(nothing)", "");
        return return_status;
    }

    if (context->id_ex.skip_cycle)
    {
        context->id_ex.skip_cycle = false;
        context->ex_mem.skip_cycle = true;

        fprintf(DEBUG_OUTPUT, "%7s(nothing)", "");
        return 0;
    }
    context->id_ex.execute_busy = true;

    context->ex_mem.branched_pc = context->id_ex.inc_pc + (context->id_ex.extended_immediate << 2);
    context->ex_mem.opcode = context->id_ex.opcode;
    context->ex_mem.immediate = context->id_ex.immediate;
    context->ex_mem.rd = context->id_ex.rd;
    context->ex_mem.rt = context->id_ex.rt;
    context->ex_mem.func = context->id_ex.func;
    context->ex_mem.load_b = context->id_ex.load_b;

    if (IS_SYSCALL(context->id_ex.opcode, context->id_ex.func))
    {
        return_status = execute_syscall(context, output_stream);
    }
    else if (IS_R_TYPE(context->id_ex.opcode, context->id_ex.func))
    {
        return_status = execute_r_type(context);
    }
    else if (IS_LOAD(context->id_ex.opcode, context->id_ex.func) || IS_STORE(context->id_ex.opcode, context->id_ex.func))
    {
        return_status = execute_load_store(context);
    }
    else if (IS_BRANCH(context->id_ex.opcode, context->id_ex.func))
    {
        return_status = execute_branch(context, stats);
    }
    else if (IS_IMMEDIATE(context->id_ex.opcode, context->id_ex.func))
    {
        return_status = execute_immediate(context);
    }

    stats->execute_count++;
    context->id_ex.execute_busy = false;
    return return_status;
}

int memory(cpu_context *context, program_stats *stats)
{
    int return_status = 0;

    if (context->mem_wb.writeback_busy)
    {
        fprintf(DEBUG_OUTPUT, "%7s(nothing)", "");
        return return_status;
    }

    if (context->ex_mem.skip_cycle)
    {
        context->ex_mem.skip_cycle = false;
        context->mem_wb.skip_cycle = true;

        fprintf(DEBUG_OUTPUT, "%7s(nothing)", "");
        return 0;
    }
    context->ex_mem.memory_busy = true;

    if (IS_STORE(context->ex_mem.opcode, context->ex_mem.func))
    {
        return_status = memory_store(context);
    }
    else if (IS_LOAD(context->ex_mem.opcode, context->ex_mem.func))
    {
        return_status = memory_load(context);
    }
    else
    {
        fprintf(DEBUG_OUTPUT, "Passing %02x", context->ex_mem.alu_out);
    }

    context->mem_wb.opcode = context->ex_mem.opcode;
    context->mem_wb.func = context->ex_mem.func;
    context->mem_wb.rd = context->ex_mem.rd;
    context->mem_wb.rt = context->ex_mem.rt;
    context->mem_wb.alu_out = context->ex_mem.alu_out;

    // not lui then inc
    if (context->ex_mem.opcode != 0x0F)
    {
        stats->memory_ref_count++;
    }

    stats->memory_cycle_count++;
    context->ex_mem.memory_busy = false;
    return return_status;
}

int write_back(cpu_context *context, program_stats *stats)
{
    int return_status = 0;

    if (context->mem_wb.skip_cycle)
    {
        context->mem_wb.skip_cycle = false;

        fprintf(DEBUG_OUTPUT, "%7s(nothing)", "");
        return 0;
    }

    if ((IS_R_TYPE(context->mem_wb.opcode, context->mem_wb.func) && context->mem_wb.rd == 0x00) ||
        (IS_IMMEDIATE(context->mem_wb.opcode, context->mem_wb.func) && context->mem_wb.rt == 0x00))
    {
        fprintf(DEBUG_OUTPUT, "%7s(nothing)", "");
        return return_status;
    }

    context->mem_wb.writeback_busy = true;

    if (IS_LOAD(context->mem_wb.opcode, context->mem_wb.func))
    {
        return_status = write_back_load_store(context);
    }
    else if (IS_R_TYPE(context->mem_wb.opcode, context->mem_wb.func) ||
        IS_IMMEDIATE(context->mem_wb.opcode, context->mem_wb.func))
    {
        return_status = write_back_r_immediate(context);
    }

    if (IS_R_TYPE(context->mem_wb.opcode, context->mem_wb.func))
    {
        context->registers_valid[context->mem_wb.rd] = true;
    }
    else if (IS_IMMEDIATE(context->mem_wb.opcode, context->mem_wb.func) && !(IS_STORE(context->mem_wb.opcode, context->mem_wb.func)))
    {
        context->registers_valid[context->mem_wb.rt] = true;
    }

    stats->writeback_count++;
    context->mem_wb.writeback_busy = false;
    return return_status;
}

int execute_r_type(cpu_context *context)
{
    fprintf(DEBUG_OUTPUT, "R: ");
    char *fun_name = NULL;
    switch(context->id_ex.func)
    {
    //add
    case 0x20:
        fun_name = "add";
        context->ex_mem.alu_out = context->id_ex.load_a + context->id_ex.load_b;
        break;
    //addu
    case 0x21:
        fun_name = "addu";
        context->ex_mem.alu_out = ((unsigned int) context->id_ex.load_a) + ((unsigned int)context->id_ex.load_b);
        break;
    //and
    case 0x24:
        fun_name = "and";
        context->ex_mem.alu_out = context->id_ex.load_a & context->id_ex.load_b;
        break;
    //nor
    case 0x27:
        fun_name = "nor";
        context->ex_mem.alu_out = ~(context->id_ex.load_a | context->id_ex.load_b);
        break;
    //xor
    case 0x26:
        fun_name = "xor";
        context->ex_mem.alu_out = context->id_ex.load_a ^ context->id_ex.load_b;
        break;
    //or
    case 0x25:
        fun_name = "or";
        context->ex_mem.alu_out = context->id_ex.load_a | context->id_ex.load_b;
        break;
    //slt
    case 0x2A:
        fun_name = "slt";
        context->ex_mem.alu_out = context->id_ex.load_a < context->id_ex.load_b;
        break;
    //sltu
    case 0x2B:
        fun_name = "sltu";
        context->ex_mem.alu_out = ((unsigned int) context->id_ex.load_a) < ((unsigned int) context->id_ex.load_b);
        break;
    //sll
    case 0x00:
        fun_name = "sll";
        context->ex_mem.alu_out = context->id_ex.load_b << context->id_ex.shamt;
        break;
    //srl
    case 0x02:
        fun_name = "srl";
        context->ex_mem.alu_out = ((unsigned int) context->id_ex.load_b) >> context->id_ex.shamt;
        break;
    //sra
    case 0x03:
        fun_name = "sra";
        context->ex_mem.alu_out = context->id_ex.load_b >> context->id_ex.shamt;
        break;
    //sllv
    case 0x04:
        fun_name = "sllv";
        context->ex_mem.alu_out = context->id_ex.load_b << context->id_ex.load_a;
        break;
    //srlv
    case 0x06:
        fun_name = "srlv";
        context->ex_mem.alu_out = ((unsigned int) context->id_ex.load_b) >> context->id_ex.load_a;
        break;
    //srav
    case 0x07:
        fun_name = "srav";
        context->ex_mem.alu_out = context->id_ex.load_b >> context->id_ex.load_a;
        break;
    //sub
    case 0x22:
        fun_name = "sub";
        context->ex_mem.alu_out = context->id_ex.load_a - context->id_ex.load_b;
        break;
    //subu
    case 0x23:
        fun_name = "subu";
        context->ex_mem.alu_out = ((unsigned int) context->id_ex.load_a) - ((unsigned int)context->id_ex.load_b);
        break;
    default:
        fprintf(stderr, "Func: %02x\n", context->id_ex.func);
        assert(false);
    }
    fprintf(DEBUG_OUTPUT, "%7s ", fun_name);
    return 0;
}

int execute_immediate(cpu_context *context)
{
    char *fun_name = NULL;
    fprintf(DEBUG_OUTPUT, "I: ");
    switch(context->id_ex.opcode)
    {
    //addi
    case 0x08:
        fun_name = "addi";
        context->ex_mem.alu_out = context->id_ex.load_a + context->id_ex.extended_immediate;
        break;
    //addiu
    case 0x09:
        fun_name = "addiu";
        context->ex_mem.alu_out = ((unsigned int) context->id_ex.load_a) + context->id_ex.extended_immediate;
        assert(false);
        break;
    //andi
    case 0x0C:
        fun_name = "andi";
        context->ex_mem.alu_out = context->id_ex.load_a & context->id_ex.immediate;
        break;
    //ori
    case 0x0D:
        fun_name = "ori";
        context->ex_mem.alu_out = context->id_ex.load_a | context->id_ex.immediate;
        break;
    //slti
    case 0x0A:
        fun_name = "slti";
        context->ex_mem.alu_out = context->id_ex.load_a < context->id_ex.extended_immediate;
        break;
    //sltiu
    case 0x0B:
        fun_name = "stliu";
        context->ex_mem.alu_out = ((unsigned int )context->id_ex.load_a) < ((unsigned int) context->id_ex.extended_immediate);
        break;
    //xori
    case 0x0E:
        fun_name = "xori";
        context->ex_mem.alu_out = context->id_ex.load_a ^ context->id_ex.immediate;
        break;
    default:
        fprintf(stderr, "Op Code: %02x\n", context->id_ex.opcode);
        assert(false);
    }
    fprintf(DEBUG_OUTPUT, "%7s", fun_name);
    return 0;
}

int execute_syscall(cpu_context *context, FILE *output_stream)
{
    fprintf(DEBUG_OUTPUT, "S: %5d", context->registers[v0]);
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
        fprintf(stderr, "Syscall: %02x\n", context->registers[v0]);
        assert(false);
    }
    return 0;
}

int execute_load_store(cpu_context *context)
{
    fprintf(DEBUG_OUTPUT, "%20s", "Loading or storing ");
    context->ex_mem.alu_out = context->id_ex.load_a + context->id_ex.extended_immediate;
    return 0;
}

int execute_branch(cpu_context *context, program_stats *stats)
{
    bool followed = false;
    if (context->id_ex.opcode == 0x04 && context->id_ex.load_a == context->id_ex.load_b)
    {
        context->pc = context->id_ex.inc_pc + (context->id_ex.extended_immediate << 2);
        followed = true;

        fprintf(DEBUG_OUTPUT, "B: eq -> %08x", context->pc);
    }
    else if (context->id_ex.opcode == 0x05 && context->id_ex.load_a != context->id_ex.load_b)
    {
        context->pc = context->id_ex.inc_pc + (context->id_ex.extended_immediate << 2);
        followed = true;

        fprintf(DEBUG_OUTPUT, "B: neq -> %08x", context->pc);
    }
    else
    {
        fprintf(DEBUG_OUTPUT, "No branch");
    }

    if (followed)
    {
        context->if_id = (const struct fetch_decode_buffer) {0};
        context->id_ex = (const struct decode_execute_buffer) {0};
        context->ex_mem.skip_cycle = true;

        stats->flush_count++;
    }

    return 0;
}

int memory_load(cpu_context *context)
{
    int n_bytes = 4;

    fprintf(DEBUG_OUTPUT, "MEM load ");
    if (context->ex_mem.opcode == 0x20 || context->ex_mem.opcode == 0x24)
    {
        n_bytes = 1;
    }
    else if (context->ex_mem.opcode == 0x21 || context->ex_mem.opcode == 0x25)
    {
        n_bytes = 2;
    }

    if (context->ex_mem.opcode == 0x20 || context->ex_mem.opcode == 0x21)
    {
        context->mem_wb.memory_data = sign_extend(n_bytes * 8, read_n_bytes(context->memory, n_bytes, context->ex_mem.alu_out));
    }
    else
    {
        context->mem_wb.memory_data = read_n_bytes(context->memory, n_bytes, context->ex_mem.alu_out);
    }

    if (context->ex_mem.opcode == 0x0F)
    {
        context->mem_wb.memory_data = context->ex_mem.immediate << 16;
    }

    fprintf(DEBUG_OUTPUT, "%8x %d", context->mem_wb.memory_data, n_bytes);

    return 0;
}

int memory_store(cpu_context *context)
{
    int n_bytes = 4;

    fprintf(DEBUG_OUTPUT, "MEM store ");
    if (context->ex_mem.opcode == 0x28)
    {
        n_bytes = 1;
    }
    else if (context->ex_mem.opcode == 0x2b)
    {
        n_bytes = 2;
    }

    write_n_bytes(context->memory, n_bytes, context->ex_mem.alu_out, context->id_ex.load_b);

    fprintf(DEBUG_OUTPUT, "%5x -> %04x\n", context->id_ex.load_b, context->ex_mem.alu_out);
    return 0;
}

int write_back_r_immediate(cpu_context *context)
{
    fprintf(DEBUG_OUTPUT, "Writing ");
    if (IS_R_TYPE(context->mem_wb.opcode, context->mem_wb.func))
    {
        fprintf(DEBUG_OUTPUT, "%2d:", context->mem_wb.rd);
        if (context->mem_wb.rd != 0)
        {
            context->registers[context->mem_wb.rd] = context->mem_wb.alu_out;
        }
        fprintf(DEBUG_OUTPUT, "%8x", context->mem_wb.alu_out);
    }
    else if (IS_IMMEDIATE(context->mem_wb.opcode, context->mem_wb.func))
    {
        fprintf(DEBUG_OUTPUT, "%2d:", context->mem_wb.rt);
        if (context->mem_wb.rt != 0)
        {
            context->registers[context->mem_wb.rt] = context->mem_wb.alu_out;
        }
        fprintf(DEBUG_OUTPUT, "%8x", context->mem_wb.alu_out);
    }
    
    return 0;
}

int write_back_load_store(cpu_context *context)
{
    if (IS_LOAD(context->mem_wb.opcode, context->mem_wb.func))
    {
        fprintf(DEBUG_OUTPUT, "Writing %2d: %8x", context->mem_wb.rt, context->mem_wb.memory_data);
        context->registers[context->mem_wb.rt] = context->mem_wb.memory_data;
    }

    return 0;
}
