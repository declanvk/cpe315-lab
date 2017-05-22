#ifndef LOAD_INSTRUCTIONS_H
#define LOAD_INSTRUCTIONS_H

#include <stdio.h>
#include <stdint.h>

#include "mips_asm_header.h"
#include "common.h"
#include "mips_context.h"

cpu_context *initialize_cpu(char *filename);
void free_cpu(cpu_context *context);
FILE *open_file_check(char *filename, MB_HDR_PTR header);
int load_into_memory(FILE *binary_file, MIPS_PTR memory, MB_HDR_PTR header);

#endif
