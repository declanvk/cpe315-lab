#include "cpu_utils.h"

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
