#ifndef MEMORY_H_
#define MEMORY_H_

#include <stddef.h>

void memory_init(void);

void *malloc(size_t size);
void free(void *p);

size_t get_available_memory(void);
size_t get_total_memory(void);

void memory_zero(void *p, size_t size);

/* Copies 'count' number of bytes pointed to by 'source' to the memory pointed to by 'dest' */
void memory_copy(void *source, void *dest, size_t count);

void debug_print_free_regions(void);

#endif