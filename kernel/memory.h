#ifndef MEMORY_H_
#define MEMORY_H_

#include <stddef.h>

void memory_init(void);

void *malloc(size_t size);
void free(void *p);

void memory_zero(void *p, size_t size);

void debug_print_free_regions(void);

#endif