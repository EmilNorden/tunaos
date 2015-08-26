#ifndef TASK_STRUCT_H_
#define TASK_STRUCT_H_

#include <stdint.h>

typedef void (*task_func)(void);

struct __attribute__((packed)) task_info {
	uint8_t *stack_ptr;
	uint8_t *stack_base;
	uint8_t *current_sp;
	task_func program;
};

#endif