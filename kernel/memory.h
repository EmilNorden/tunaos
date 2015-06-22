#ifndef MEMORY_H_
#define MEMORY_H_

void memory_init();

void *malloc(unsigned int size);
void free(void *p);

void memory_zero(void *p, unsigned int size);

void debug_print_free_regions(void);

#endif