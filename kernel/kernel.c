#include "../drivers/screen.h"
#include "interrupts.h"
#include "util.h"
#include "low_level.h"
#include "system_clock.h"
#include <stdint.h>

void keyboard_handler(struct regs *r)
{
	int data = port_byte_in(0x60);
	
	if((data & 0x80) == 0) {
		int uptime = clock_get_uptime();
		char *buf = "               ";
		int_to_string(uptime, buf);
		
		print("Key pressed. The system has been running for ");
		print(buf);
		print(" seconds!\n");
	}
}

void main(void)
{
	clear_screen();
	print("Booting potatismOS 0.0.1-pre-alpha...\n");
	
	print("Init IRQ subsystem...");
	irq_initialize();
	irq_set_handler(1, keyboard_handler);
	print("done\n");
	
	print("Init system clock...");
	clock_init();
	print("done\n");
	
	
	
	for(;;) {
		__asm__ __volatile__("hlt");
	}
}
