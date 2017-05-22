#include "execute_instructions.h"

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
