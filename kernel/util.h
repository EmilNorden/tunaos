#ifndef KERNEL_UTIL_H_
#define KERNEL_UTIL_H_

#include <stddef.h>

void int_to_string(int value, char *input);

size_t strlen(const char *str);

int snprintf(char *str, size_t n, const char *format, int nargs, ...);

#endif