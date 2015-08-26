#ifndef TASK_H_
#define TASK_H_

#include <stddef.h>

typedef void (*task_func)(void);

struct task_info;

//int create_task(void *program, size_t stack_size, struct task_info **task);
int create_task(task_func program, size_t stack_size, struct task_info **task);

#endif