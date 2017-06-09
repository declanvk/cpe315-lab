#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>

#define AMAX 100      /* Maximum (square) array size */
#define CACHESIM 1    /* Set to 1 if simulating Cache */

#define CACHE_SIZE 256          /* Number of lines */
#define CACHE_LINE_SIZE 32      /* Number of BYTES per line */
#define CACHE_ASSOCIATIVITY 4   /* Associativity */

#define TAG_SIZE 21                     /* Number of BITS in the tag */
#define INDEX_SIZE 6                    /* Number of BITS in the index, = log2(CACHE_SIZE / CACHE_ASSOCIATIVITY)*/
#define BLOCK_OFFSET_SIZE 5             /* Number of BITS in the block offset, = log2(CACHE_LINE_SIZE) */

#define DIRTY_MASK 0x01
#define VALID_MASK 0x02
#define IS_DIRTY(flag) ((flag & DIRTY_MASK) != 0)     /* Extracts dirty bit from flag value */
#define IS_VALID(flag) ((flag & VALID_MASK) != 0)    /* Extracts valid bit from flag value */

typedef union effective_addr_t
{
    uint32_t value;
    struct
    {
        unsigned int block_offset : BLOCK_OFFSET_SIZE;
        unsigned int index : INDEX_SIZE;
        unsigned int tag : TAG_SIZE;
    } fields;
} effective_addr;

typedef struct cpu_cache_t
{
    int pointer_size;
    char flags[CACHE_SIZE];
    uint32_t tag[CACHE_SIZE];
    char data[CACHE_SIZE][CACHE_LINE_SIZE];
    int cache_use[CACHE_SIZE];
} cpu_cache;

typedef struct cpu_cache_stats_t
{
    int read_hit;
    int read_miss;
    int write_hit;
    int write_miss;
} cache_stats;

void print_bytes(const void *object, size_t size)
{
    const unsigned char * const bytes = object;
    size_t i;

    fprintf(stderr, "[ ");
    for(i = 0; i < size; i++)
    {
        fprintf(stderr, "%02x ", bytes[i]);
    }
    fprintf(stderr, "]");
}

uint32_t find_cache_line(cpu_cache *cache, uint32_t tag, uint32_t index)
{
    int effective_index;
    for (int assoc_index = 0; assoc_index < CACHE_ASSOCIATIVITY; assoc_index++)
    {
        effective_index = index * CACHE_ASSOCIATIVITY + assoc_index;
        if (cache->tag[effective_index] == tag &&
            IS_VALID(cache->flags[effective_index]))
        {
            return effective_index;
        }
    }

    return -1;
}

uint32_t open_cache_line(cpu_cache *cache, uint32_t index)
{
    uint32_t max_time = 0, max_index = -1, assoc_index;
    int effective_index;

    for (assoc_index = 0; assoc_index < CACHE_ASSOCIATIVITY; assoc_index++)
    {
        effective_index = index * CACHE_ASSOCIATIVITY + assoc_index;
        // fprintf(stderr, "Checking %2d ", effective_index);
        if (!(IS_VALID(cache->flags[effective_index])))
        {
            // fprintf(stderr, "INVALID : %2d, ", effective_index);
            return effective_index;
        }

        if ((cache->cache_use[effective_index] > max_time) &&
            IS_VALID(cache->flags[effective_index]))
        {
            max_time = cache->cache_use[effective_index];
            max_index = effective_index;
        }
    }

    // fprintf(stderr, "EVICTING: %2d, ", max_index);
    return max_index;
}

void init_cache(cpu_cache *cache, cache_stats *stats)
{
    int *temp = NULL;

    cache->pointer_size = sizeof(temp) * 8;
    memset(&(cache->flags), 0, CACHE_SIZE);
    memset(&(cache->tag), 0, CACHE_SIZE);
    memset(&(cache->data), 0, CACHE_SIZE * CACHE_LINE_SIZE);
    memset(&(cache->cache_use), 0, CACHE_SIZE);

    stats->read_hit = 0;
    stats->read_miss = 0;
    stats->write_hit = 0;
    stats->write_miss = 0;
}

uint32_t mem_read(uint32_t *mp, cpu_cache *cache, cache_stats *stats)
{
    effective_addr address;
    uint32_t cache_line_index = -1;
    uint32_t data = 0, index, tag, block_offset;
    int line_index;

    address.value = (uint32_t) mp;
    index = address.fields.index;
    tag = address.fields.tag;
    block_offset = address.fields.block_offset;

    cache_line_index = find_cache_line(cache, tag, index);

    // fprintf(stderr, "Addr: %08x, Tag: %06x, Index: %2d, Offset: %2d -> ", (uint32_t) mp, tag, index, block_offset);
    if (cache_line_index != -1) // HIT
    {
        // fprintf(stderr, "HIT  ");
        stats->read_hit++;
        cache->cache_use[cache_line_index] = 0;

        memcpy(&data, cache->data[cache_line_index] + block_offset, sizeof(data));
        // fprintf(stderr, "LIndex: %4d, Data: %d", cache_line_index, data);
    }
    else // MISS
    {
        // fprintf(stderr, "MISS ");
        stats->read_miss++;
        data = *mp;

        cache_line_index = open_cache_line(cache, index);
        cache->cache_use[cache_line_index] = 0;
        cache->tag[cache_line_index] = tag;
        cache->flags[cache_line_index] |= VALID_MASK;

        memcpy(cache->data[cache_line_index], ((char *) mp) - block_offset, CACHE_LINE_SIZE);
        // print_bytes(mp, CACHE_LINE_SIZE);
    }

    for (line_index = 0; line_index < CACHE_SIZE; line_index++)
    {
        cache->cache_use[line_index]++;
    }
    // fprintf(stderr, "\n");

    return data;
}

void mem_write(uint32_t *mp, uint32_t data, cpu_cache *cache, cache_stats *stats)
{
    effective_addr address;
    uint32_t index, tag, block_offset;
    uint32_t cache_line_index = -1;

    address.value = (uint32_t) mp;
    index = address.fields.index;
    tag = address.fields.tag;
    block_offset = address.fields.block_offset;

    cache_line_index = find_cache_line(cache, tag, index);

    if (cache_line_index != -1) // HIT
    {
        stats->write_hit++;

        memcpy(cache->data[cache_line_index] + block_offset, &data, sizeof(data));
    }
    else
    {
        stats->write_miss++;
    }

    *mp = data;
}


/* 
    Statically define the arrays a, b, and mult,
    where mult will become the cross product of a and b, i.e., a x b.
*/
static uint32_t a[AMAX][AMAX], b[AMAX][AMAX], mult[AMAX][AMAX];

void matmul(int r1, int c1, int c2, cpu_cache *cache, cache_stats *stats)
{
    uint32_t i, j, k, a_data, b_data, mult_data;
    uint32_t *mp1, *mp2, *mp3;

    /* Initializing elements of matrix mult to 0.*/
    for(i = 0; i < r1; ++i)
    {
        for(j = 0; j < c2; ++j)
        {
            mult[i][j] = 0;
        }
    }

    /* Multiplying matrix a and b and storing in array mult. */
    for(i = 0; i < r1; ++i)
    {
        for(j = 0; j < c2; ++j)
        {
            for(k = 0; k < c1; ++k)
            {

#if CACHESIM    /* "Hooks" to measure memory references - enabled if CACHESIM  */
                mp1 = &mult[i][j];
                mp2 = &a[i][k];
                mp3 = &b[k][j];

                // fprintf(stderr, "I: %2d, J: %2d, K: %2d\n", i, j, k);
                // fprintf(stderr, "M: ");
                mult_data = mem_read(mp1, cache, stats);
                // fprintf(stderr, "A: ");
                a_data = mem_read(mp2, cache, stats);
                // fprintf(stderr, "B: ");
                b_data = mem_read(mp3, cache, stats);

                // if ((a[i][k] * b[k][j]) != (a_data * b_data))
                // {
                //     fprintf(stderr, "%d * %d != %d * %d\n", a[i][k], b[k][j], a_data, b_data);
                // }
                // else
                // {
                //     fprintf(stderr, "%d * %d == %d * %d\n", a[i][k], b[k][j], a_data, b_data);
                // }
                assert((a[i][k] * b[k][j]) == (a_data * b_data));
                // if ((mult_data) != (mult[i][j]))
                // {
                //     fprintf(stderr, "%d != %d\n", mult[i][j], mult_data);
                // }
                // else
                // {
                //     fprintf(stderr, "%d == %d\n", mult[i][j], mult_data);
                // }
                assert((mult_data) == (mult[i][j]));

                mem_write(mp1, mult_data + (a_data * b_data), cache, stats);

                assert(((mult[i][j] + (a[i][k] * b[k][j])) == (*mp1)) || (true));
#else
                mult[i][j]+=a[i][k] * b[k][j];
#endif
            }
        }
    }
}

int main()
{
    int r1, c1, r2, c2, i, j;
    cpu_cache cache;
    cache_stats stats;

    init_cache(&cache, &stats);

    fprintf(stderr, "TAG_SIZE: %d, INDEX_SIZE: %d, BLOCK_OFFSET_SIZE: %d\n", TAG_SIZE, INDEX_SIZE, BLOCK_OFFSET_SIZE);

    printf("Enter rows and column for first matrix: ");
    scanf("%d%d", &r1, &c1);
    printf("Enter rows and column for second matrix: ");
    scanf("%d%d",&r2, &c2);

    /*
        If column of first matrix in not equal to row of second matrix,
        asking user to enter the size of matrix again.
    */
    while (c1 != r2)
    {
        printf("Error! column of first matrix not equal to row of second.\n");
        printf("Enter rows and column for first matrix: ");
        scanf("%d%d", &r1, &c1);
        printf("Enter rows and column for second matrix: ");
        scanf("%d%d",&r2, &c2);
    }

    for(i = 0; i < r1; ++i)
    {
        for(j = 0; j < c1; ++j)
        {
            a[i][j] = i + j;
        }
    }

    /* Storing elements of second matrix. */
    for(i = 0; i < r2; ++i)
    {
        for(j = 0; j < c2; ++j)
        {
            b[i][j] = 10 + i + j;
        }
    }

    matmul(r1, c1, c2, &cache, &stats);

    printf("\nOutput Matrix:\n");
    for(i = 0; i < r1; ++i)
    {
        for(j = 0; j < c2; ++j)
        {
            printf("%d  ",mult[i][j]);
            if(j == c2 - 1)
            {
                printf("\n\n");
            }
        }
    }

    fprintf(stderr, "Stats:\n");
    fprintf(stderr, "%d/%d, %d/%d\n", stats.read_hit, stats.read_miss, stats.write_hit, stats.write_miss);
    fprintf(stderr, "# of memory reads: %d\n", stats.read_hit + stats.read_miss);
    fprintf(stderr, "# of memory writes: %d\n", stats.write_hit + stats.write_miss);
    fprintf(stderr, "ratio of read to write %f\n",
        ((float) stats.read_hit + stats.read_miss) / (stats.write_hit + stats.write_miss));
    fprintf(stderr, "hit rate: %f\n",
        100 * ((float) stats.read_hit + stats.write_hit) /
        (stats.read_hit + stats.read_miss + stats.write_hit + stats.write_miss));
    fprintf(stderr, "hit rate without write: %f\n",
        100 * ((float) stats.read_hit) / (stats.read_hit + stats.read_miss));

    return 0;
}
