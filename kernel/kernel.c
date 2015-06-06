#include "../drivers/screen.h"
#include "../drivers/timer.h"
#include "interrupts.h"
#include <stdint.h>
#include "util.h"
#include "low_level.h"
#include "system.h"

int ticks = 0;
int secs = 0;
void timer_handler(struct regs *r)
{
	ticks += 1;
	
	if(ticks % 100 == 0) {
		secs++;
		char *buf = "          ";
		int_to_string(secs, buf);
		print_at(buf, 0, 3);
	}
}

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
	irq_set_handler(0, timer_handler);
	irq_set_handler(1, keyboard_handler);
	print("done\n");
	
	pit_set_counter(PIT_BCD_OFF, PIT_MODE_3, PIT_RW_LSB_MSB, PIT_SC_0);
	
	for(;;) {
		__asm__ __volatile__("hlt");
	}
}
