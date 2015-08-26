#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "task.h"
#include "../common/linked_list.h"

struct linked_list task_queue;

int enqueue_task(struct task_info *task);

void scheduler_init();
void scheduler_run();

void switch_to_task(struct task_info *task, struct task_info *prev_task);

void yield(void);

#endif