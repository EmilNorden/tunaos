#include "../drivers/screen.h"
#include "../drivers/timer.h"
#include "interrupts.h"
#include <stdint.h>
#include "util.h"
#include "low_level.h"

void keyboard_handler(struct regs *r)
{
	print("keyboard data: ");
	int data = port_byte_in(0x60);
	
	char *buf = "     ";
	int_to_string(data & 0x7F, buf); // Mask away top bit
	
	print(buf);
	if(data & 0x80)
		print(" released\n");
	else
		print(" pressed\n");
}


void main(void)
{
	clear_screen();
	print("Booting potatismOS 0.0.1-pre-alpha...\n");
	print("Init IRQ subsystem...");
	irq_initialize();
	irq_set_handler(1, keyboard_handler);
	print("done\n");
	
	for(;;) {
		__asm__ __volatile__("hlt");
	}
}
