#include "../drivers/screen.h"
#include "interrupts.h"
#include "util.h"
#include "low_level.h"
#include "system_clock.h"
#include "../drivers/cmos.h"
#include "memory.h"

#include "cpu.h"

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

void main(void)
{
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
		
	debug_print_free_regions();	
	
	struct cpu_info cpu;
	
	get_cpu_info(&cpu);
	
	print(cpu.vendor_id);
	
	for(;;) {
		__asm__ __volatile__("hlt");
	}
}
