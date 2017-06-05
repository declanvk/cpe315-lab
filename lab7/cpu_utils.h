#ifndef CPU_UTILS_H
#define CPU_UTILS_H

#include <stdint.h>

#include "mips_context.h"

uint32_t sign_extend(int n_bits, uint32_t value);
uint32_t read_n_bytes(BYTE_PTR memory, int n_bytes, uint32_t address);
void write_n_bytes(BYTE_PTR memory, int n_bytes, uint32_t address, uint32_t value);

#endif
