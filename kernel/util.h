#ifndef KERNEL_UTIL_H_
#define KERNEL_UTIL_H_

/* Copies 'count' number of bytes pointed to by 'source' to the memory pointed to by 'dest' */
void memory_copy(char *source, char *dest, int count);

void int_to_string(int value, char *input);

unsigned int strlen(const char *str);

#endif