#ifndef MEMORY_H_
#define MEMORY_H_

void memory_init();

void *malloc(unsigned int size);

void memory_zero(void *p, unsigned int size);

#endif