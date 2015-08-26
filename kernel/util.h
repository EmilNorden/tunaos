#ifndef KERNEL_UTIL_H_
#define KERNEL_UTIL_H_

#include <stddef.h>
#include <stdint.h>

void int_to_string(int value, char *input);

size_t strlen(const char *str);

int snprintf(char *str, size_t n, const char *format, int nargs, ...);

int hex8(uint8_t value, char *buf);
int hex16(uint16_t value, char *buf);

void hex_dump(void *data, size_t len);

#endif