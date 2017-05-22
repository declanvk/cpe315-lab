#include "execute_instructions.h"

#include <stdint.h>

int execute_instructions(cpu_context *context)
{
    int cycle_counter = 0;
    int return_status = 0;

    while (return_status == 0)
    {
        switch(context->state) {
        case FETCH:
            return_status = fetch(context);
            break;
        case DECODE:
            return_status = decode(context);
            break;
        case EXECUTE:
            break;
        case MEM:
            break;
        case WRITEBACK:
            break;
        }
    }

    context->state = transition_state(context);
    cycle_counter++;

    return cycle_counter;
}

cpu_state transition_state(cpu_context *context)
{
    switch(context->state) {
    case FETCH:
        return DECODE;
    case DECODE:
        return EXECUTE;
    case EXECUTE:
        return FETCH;
    case MEM:
        return WRITEBACK;
    case WRITEBACK:
        return FETCH;
    }
}

int fetch(cpu_context *context)
{
    context->ir.value = context->memory[context->pc];
    context->pc += 4;

    return 0;
}

int decode(cpu_context *context)
{
    context->load_a = context->registers[context->ir.r_type.rs];
    context->load_b = context->registers[context->ir.r_type.rt];
    context->alu_out = context->pc + (sign_extend(16, context->ir.i_type.immediate) << 2);
    return 0;
}

int execute_r_type(cpu_context *context)
{
    return 0;
}

int execute_immediate(cpu_context *context)
{
    return 0;
}

int execute_branch(cpu_context *context)
{
    return 0;
}

int execute_load_store(cpu_context *context)
{
    return 0;
}

int memory_fetch(cpu_context *context)
{
    return 0;
}

int memory_store(cpu_context *context)
{
    return 0;
}

int write_back_r_immediate(cpu_context *context)
{
    return 0;
}

int write_back_load(cpu_context *context)
{
    return 0;
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
