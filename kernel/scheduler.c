#include "scheduler.h"
#include "system.h"
#include "task_struct.h"
#include "../drivers/timer.h"
#include "interrupts.h"
#include "util.h"
#include "../drivers/screen.h"

static struct task_info *current_task;

extern void switch_stack_asm(uint32_t *new_ebp, uint32_t *new_esp);
extern void update_task_sp(uint32_t **sp);

extern void switch_stack_asm3(struct task_info *current, struct task_info *prev);
extern void switch_stack_asm3_b(struct task_info *current, struct task_info *prev);

static void debug_print_task_info(struct task_info *task)
{
	uint32_t ptr = (uint32_t)task;
	uint32_t *ebp = (uint32_t*)task->stack_base;
	uint32_t *esp = (uint32_t*)task->current_sp;
	char buf[128];
	snprintf(buf, 128, "ptr: %d, ebp: %d, esp: %d\n", 3, ptr, ebp, esp);
	print(buf);
}


static struct task_info *get_next_task()
{
	struct task_info *task = list_at(&task_queue, 0);
	if(task)
		list_remove_at(&task_queue, 0);
		
	return task;
}

static void scheduler_tick()
{
	struct task_info *task = list_at(&task_queue, 0);
	if(task) {
		list_remove_at(&task_queue, 0);
		list_add_last(&task_queue, task);
	}
}

int enqueue_task(struct task_info *task)
{
	char buf[64];
	uint32_t *ebp = (uint32_t*)task->stack_base;
	uint32_t *esp = (uint32_t*)task->current_sp;
	snprintf(buf, 64, "enqueue ptr: %d, base: %d, sp: %d\n", 3, (uint32_t)task, (uint32_t)ebp, (uint32_t)esp);
	print(buf);
	
	int res = list_add_last(&task_queue, task);
	PANIC_ON(res != 0, "Unable to enqueue task");
	return 0;
}

void scheduler_init()
{
	int result = new_linked_list(&task_queue);
	PANIC_ON(result != 0, "Unable to create task queue");
}

void switch_stack(struct task_info *task, struct task_info *prev_task)
{	/*
	print("switching from:\n");
	if(prev_task == NULL)
		print("<nothing>\n");
	else
		debug_print_task_info(prev_task);
	print("switching to:\n");
	debug_print_task_info(task);
	*/
	switch_stack_asm3(task, prev_task);
}

void scheduler_run()
{
	struct task_info *task = get_next_task();
	
	char buf[128];
	snprintf(buf, 128, "switch_stack_asm3: %d\nswitch_stack: %d\n", 2, (int)switch_stack_asm3, (int)switch_stack);
	print(buf);
	
	switch_to_task(task, NULL);
}




void switch_to_task(struct task_info *task, struct task_info *prev_task)
{
	char buf[96];
	snprintf(buf, 96, "\n\nSwitching to task FLAGS: %d, CODESEG: %d, EIP: %d\n", 3,
	 ((uint32_t*)task->current_sp)[8],
	 ((uint32_t*)task->current_sp)[7],
	 ((uint32_t*)task->current_sp)[6]);
	print(buf);

	snprintf(buf, 96, "EBP: %d, ESP: %d\n", 2, (int)task->stack_base, (int)task->current_sp);
	print(buf);
	
	current_task = task;
	
	print("\n");
	hex_dump(task->current_sp, 80);
	print("\n");
	
	switch_stack(task, prev_task);
}

void yield(void)
{
	irq_disable();
	struct task_info *prev_task = current_task;
	
	list_add_last(&task_queue, current_task);
	
	switch_to_task(get_next_task(), prev_task);
}