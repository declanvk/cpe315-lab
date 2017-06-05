#include "summarise_context.h"

#include <stdio.h>
#include <stdbool.h>

char *register_names[] = {
    "zero", "at",
    "v0", "v1",
    "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "t8", "t9", "k0", "k1",
    "gp", "sp", "fp", "ra"
};

int register_groups[] = {
    2, 2, 4, 8, 8, 4, 4
};

void print_registers(FILE *output_stream, cpu_context *context, bool hex_output)
{
    int register_idx = 0;
    int group_offset = 0;
    int group_idx = 0;

    char *register_format = hex_output ? "%08x  ": "%8d  ";
    
    for (group_idx = 0; group_idx < 7; group_idx++)
    {
        for (register_idx = 0; register_idx < register_groups[group_idx]; register_idx++)
        {
            fprintf(output_stream, "%-4s(%2d)  ", register_names[register_idx + group_offset],
                register_idx + group_offset);
        }
        fprintf(output_stream, "\n");
        for (register_idx = 0; register_idx < register_groups[group_idx]; register_idx++)
        {
            fprintf(output_stream, register_format, context->registers[register_idx + group_offset]);
        }
        fprintf(output_stream, "\n");

        group_offset += register_groups[group_idx];
    }
}

char *stat_end_format = "%s totalled %d cycles, %d instructions, %d memory references.\nFlushes: %d, Fetches: %d, Decodes: %d, Executes: %d, Memories: %d, Writebacks: %d\n";
char *stat_mid_format = "%s executed %d cycles, %d instructions, %d memory references.\nFlushes: %d, Fetches: %d, Decodes: %d, Executes: %d, Memories: %d, Writebacks: %d\n";

void print_stats(FILE *output_stream, char *program_name, program_stats *stats, bool end)
{
    fprintf(output_stream, end ? stat_end_format : stat_mid_format, program_name,
        stats->clock_count, stats->completed_instruction_count, stats->memory_ref_count,
        stats->flush_count, stats->fetch_count, stats->decode_count, stats->execute_count,
        stats->memory_cycle_count, stats->writeback_count);
    return;
}

void print_clock_header(FILE *output_stream)
{
    fprintf(output_stream, "|%-23s|%-23s|%-23s|%-23s|%s\n", "Fetch", "Decode", "Execute", "Memory", "Write Back");
}
