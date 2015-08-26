#include "../drivers/screen.h"
#include "interrupts.h"
#include "util.h"
#include "low_level.h"
#include "system_clock.h"
#include "../drivers/cmos.h"
#include "memory.h"
#include "system.h"
#include "cpu.h"
#include "../common/linked_list.h"
#include "task.h"
#include "scheduler.h"
#include "../common/definitions.h"
#include "task_struct.h"

void keyboard_handler(struct regs *r)
{
	int data = port_byte_in(0x60);
	
	if((data & 0x80) == 0) {
		int uptime = clock_get_uptime();
		char *buf = "               ";
		int_to_string(uptime, buf);
		
		print("The current time is ");
		int_to_string(cmos_get_hours(), buf);
		print(buf);
		print(":");
		int_to_string(cmos_get_minutes(), buf);
		print(buf);
		print(":");
		int_to_string(cmos_get_seconds(), buf);
		print(buf);
	}
}

void prog1()
{
	print("inside prog1\n");
	while(TRUE)
	{
		print("1");
		yield();
		
	}
}

void prog2()
{
	print("inside prog2\n");
	while(TRUE)
	{
		print("2");
		yield();
	}
}

void print_list(struct linked_list *l, size_t count)
{
	char buf[32];
	for(size_t i = 0; i < count; ++i) {
		int *value = (int*)list_at(l, i);
		snprintf(buf, 32, "%d -  value: %d\n", 2, i, *value);
		print(buf);
	}
}

void main(void)
{
	char buf[32];
	clear_screen();
	print("Booting tunaOS 0.0.1-pre-alpha...\n");
	
	print("Init IRQ subsystem...");
	irq_initialize();
	irq_set_handler(1, keyboard_handler);
	print("done\n");
	
	print("Init system clock...");
	clock_init();
	print("done\n");
	
	cmos_init();
	
	memory_init();
		
	//debug_print_free_regions();	
	
	struct task_info *task1;
	struct task_info *task2;
	
	scheduler_init();
	
	int res = create_task(prog1, 4096, &task1);
	PANIC_ON(res != 0, "create_task failed!");
	
	res = create_task(prog2, 4096, &task2);
	PANIC_ON(res != 0, "create_task failed!");

	enqueue_task(task1);
	enqueue_task(task2);

	scheduler_run();
	
	
	for(;;) {
		__asm__ __volatile__("hlt");
	}
}
