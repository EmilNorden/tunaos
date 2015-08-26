#include "task.h"
#include "memory.h"
#include "system.h"
#include "task_struct.h"
#include "../common/definitions.h"
#include <stdint.h>
#include "../drivers/screen.h"
#include "util.h"

struct regs {
	/* General registers */
	uint32_t eax, ebx, ecx, edx;
	
	/* Stack registers */
	uint32_t esp, ebp;
	
	/* EFLAGS */
	uint32_t flags;
};

static void stack_push(struct task_info *task, uint32_t value)
{
	uint32_t *esp = (uint32_t*)task->current_sp;
	esp--;
	*esp = value;
	task->current_sp = (uint8_t*)esp;
}


int create_task(task_func program, size_t stack_size, struct task_info **task)
{
	PANIC_ON(program == NULL, "Program pointer cannot be NULL");
	PANIC_ON(stack_size == 0, "Cannot create task with stack size of 0.");
	
	*task = malloc(sizeof(struct task_info));
	
	if(!*task)
		return -1;
	
	uint8_t *stack = (uint8_t*)malloc(stack_size);
	uint8_t *stack_base = stack + stack_size;
	
	if(!stack) {
		free(*task);
		return -1;
	}
	
	memory_zero(stack, stack_size);
	
	(*task)->stack_ptr = stack;
	(*task)->stack_base = stack + stack_size;
	(*task)->current_sp = stack + stack_size;
	(*task)->program = program;
	
	stack_push(*task, 514); // EFLAGS (No idea what to put here, I inspected EFLAGS at some random location and found it had the value 514)
	stack_push(*task, 0x08); // code segment
	stack_push(*task, (uint32_t)program); // EIP
	stack_push(*task, 0x0); // edi
	stack_push(*task, 0x0); // esi
	stack_push(*task, 0x0); // ebx
	stack_push(*task, 0x0); // edx
	stack_push(*task, 0x0); // ecx
	stack_push(*task, 0x0); // eax
	
	char buf[92];
	uint32_t *ebp = (uint32_t*)(*task)->stack_base;
	uint32_t *esp = (uint32_t*)(*task)->current_sp;
	snprintf(buf, 92, "DEBUG: created new task with EBP: %d, ESP: %d\n", 2, ebp, esp);
	print(buf);
	
	return 0;
}